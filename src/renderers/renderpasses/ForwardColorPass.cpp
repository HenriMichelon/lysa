/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.forward_color_pass;

import lysa.renderers.graphic_pipeline_data;

namespace lysa {
    ForwardColorPass::ForwardColorPass(
        const Context& ctx,
        const RendererConfiguration& config):
        Renderpass{ctx, config, "Forward Color"},
        materialManager(ctx.res.get<MaterialManager>()) {
        pipelineConfig.colorRenderFormats.push_back(config.colorRenderingFormat); // Color

        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.resources = ctx.vireo->createPipelineResources({
           ctx.globalDescriptorLayout,
           ctx.samplers.getDescriptorLayout(),
           SceneFrameData::sceneDescriptorLayout,
           GraphicPipelineData::pipelineDescriptorLayout,
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
           SceneFrameData::sceneDescriptorLayoutOptional1
#endif
        },
           SceneFrameData::instanceIndexConstantDesc, name);
        pipelineConfig.vertexInputLayout = ctx.vireo->createVertexLayout(sizeof(VertexData), VertexData::vertexAttributes);
        renderingConfig.colorRenderTargets[0].clearValue = {
            config.clearColor.r,
            config.clearColor.g,
            config.clearColor.b,
            1.0f};
        renderingConfig.clearDepthStencil = false;

        framesData.resize(ctx.config.framesInFlight);
    }

    void ForwardColorPass::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        for (const auto& [pipelineId, materials] : pipelineIds) {
            if (!pipelines.contains(pipelineId)) {
                const auto& material = materialManager[materials.at(0)];
                std::string vertShaderName = DEFAULT_VERTEX_SHADER;
                std::string fragShaderName = /*config.bloomEnabled ? DEFAULT_FRAGMENT_BLOOM_SHADER :*/ DEFAULT_FRAGMENT_SHADER;
                if (material.getType() == Material::SHADER) {
                    const auto& shaderMaterial = dynamic_cast<const ShaderMaterial&>(material);
                    if (!shaderMaterial.getVertFileName().empty()) {
                        vertShaderName = shaderMaterial.getVertFileName();
                    }
                    if (!shaderMaterial.getFragFileName().empty()) {
                        fragShaderName = shaderMaterial.getFragFileName();
                    }
                }
                pipelineConfig.cullMode = material.getCullMode();
                pipelineConfig.vertexShader = loadShader(vertShaderName);
                pipelineConfig.fragmentShader = loadShader(fragShaderName);
                pipelineConfig.msaa = config.msaa;
                pipelines[pipelineId] = ctx.vireo->createGraphicPipeline(pipelineConfig, vertShaderName + "+" + fragShaderName + ":" + std::to_string(pipelineId));
            }
        }
    }

    void ForwardColorPass::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const std::shared_ptr<vireo::RenderTarget>& multisampledDepthAttachment,
        const bool clearAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];renderingConfig.multisampledDepthStencilRenderTarget = multisampledDepthAttachment;

        renderingConfig.colorRenderTargets[0].clear = clearAttachment;
        renderingConfig.colorRenderTargets[0].renderTarget = colorAttachment;
        renderingConfig.depthStencilRenderTarget = depthAttachment;
        if (config.msaa != vireo::MSAA::NONE) {
            renderingConfig.colorRenderTargets[0].multisampledRenderTarget = frame.multisampledColorAttachment;
            commandList.barrier(
                frame.multisampledColorAttachment,
                vireo::ResourceState::UNDEFINED,
                vireo::ResourceState::RENDER_TARGET_COLOR);
        }

        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.beginRendering(renderingConfig);
        scene.drawOpaquesModels(
            commandList,
            pipelines);
        scene.drawShaderMaterialModels(
            commandList,
            pipelines);
        scene.drawTransparentModels(
            commandList,
            pipelines);
        commandList.endRendering();
        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::UNDEFINED);
        if (config.msaa != vireo::MSAA::NONE) {
            commandList.barrier(
                frame.multisampledColorAttachment,
                vireo::ResourceState::RENDER_TARGET_COLOR,
                vireo::ResourceState::UNDEFINED);
        }
    }

    void ForwardColorPass::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        if (config.msaa != vireo::MSAA::NONE) {
            for (auto& frame : framesData) {
                frame.multisampledColorAttachment = ctx.vireo->createRenderTarget(
                  config.colorRenderingFormat,
                  extent.width, extent.height,
                  vireo::RenderTargetType::COLOR,
                  {config.clearColor.r, config.clearColor.g, config.clearColor.b, 1.0f},
                  1,
                  config.msaa,
                  "MSAA color attachment");
            }
        }
    }
}