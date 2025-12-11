/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.forward_color;

import lysa.renderers.graphic_pipeline_data;

namespace lysa {
    ForwardColor::ForwardColor(
        const Context& ctx,
        const RendererConfiguration& config,
        const uint32 framesInFlight):
        Renderpass{ctx, config, "Forward Color"},
        materialManager(ctx.res.get<MaterialManager>()) {
        pipelineConfig.colorRenderFormats.push_back(config.colorRenderingFormat); // Color

        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.resources = ctx.vireo->createPipelineResources({
                   ctx.globalDescriptorLayout,
                   ctx.samplers.getDescriptorLayout(),
                   SceneRenderContext::sceneDescriptorLayout,
                   GraphicPipelineData::pipelineDescriptorLayout,
                   SceneRenderContext::sceneDescriptorLayoutOptional1},
                   SceneRenderContext::instanceIndexConstantDesc, name);
        pipelineConfig.vertexInputLayout = ctx.vireo->createVertexLayout(sizeof(VertexData), VertexData::vertexAttributes);
        renderingConfig.colorRenderTargets[0].clearValue = {
            config.clearColor.r,
            config.clearColor.g,
            config.clearColor.b,
            1.0f};
        renderingConfig.clearDepthStencil = false;

        framesData.resize(framesInFlight);
    }

    void ForwardColor::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
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
                pipelines[pipelineId] = ctx.vireo->createGraphicPipeline(pipelineConfig, vertShaderName + "+" + fragShaderName);
            }
        }
    }

    void ForwardColor::render(
        vireo::CommandList& commandList,
        const SceneRenderContext& scene,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const bool clearAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];

        renderingConfig.colorRenderTargets[0].clear = clearAttachment;
        renderingConfig.colorRenderTargets[0].renderTarget = colorAttachment;
        renderingConfig.depthStencilRenderTarget = depthAttachment;

        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.beginRendering(renderingConfig);
        scene.drawOpaquesModels(
            commandList,
            pipelines);
        scene.drawTransparentModels(
            commandList,
            pipelines);
        scene.drawShaderMaterialModels(
            commandList,
            pipelines);
        commandList.endRendering();
        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::UNDEFINED);
    }

    void ForwardColor::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        for (auto& frame : framesData) {
        }
    }
}