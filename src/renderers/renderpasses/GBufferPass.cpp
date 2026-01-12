/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.gbuffer_pass;

import lysa.renderers.graphic_pipeline_data;

namespace lysa {

    GBufferPass::GBufferPass(
    const Context& ctx,
        const RendererConfiguration& config,
        const bool withStencil):
        Renderpass{ctx, config, "GBuffer"},
        materialManager(ctx.res.get<MaterialManager>()) {

        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.stencilTestEnable = withStencil;
        pipelineConfig.backStencilOpState = pipelineConfig.frontStencilOpState;
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
        renderingConfig.stencilTestEnable = pipelineConfig.stencilTestEnable;

        framesData.resize(ctx.config.framesInFlight);
    }

    void GBufferPass::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        for (const auto& [pipelineId, materials] : pipelineIds) {
            if (!pipelines.contains(pipelineId)) {
                const auto& material = materialManager[materials.at(0)];
                //INFO("GBufferPass updatePipelines ", std::to_string(material->getName()));
                pipelineConfig.cullMode = material.getCullMode();
                pipelineConfig.vertexShader = loadShader(VERTEX_SHADER);
                pipelineConfig.fragmentShader = loadShader(FRAGMENT_SHADER);
                pipelines[pipelineId] = ctx.vireo->createGraphicPipeline(pipelineConfig, name);
            }
        }
    }

    void GBufferPass::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const std::shared_ptr<vireo::RenderTarget>&,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const bool,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];

        renderingConfig.colorRenderTargets[BUFFER_POSITION].renderTarget = frame.positionBuffer;
        renderingConfig.colorRenderTargets[BUFFER_NORMAL].renderTarget = frame.normalBuffer;
        renderingConfig.colorRenderTargets[BUFFER_ALBEDO].renderTarget = frame.albedoBuffer;
        renderingConfig.colorRenderTargets[BUFFER_EMISSIVE].renderTarget = frame.emissiveBuffer;
        renderingConfig.depthStencilRenderTarget = depthAttachment;

        auto renderTargets = std::views::transform(renderingConfig.colorRenderTargets, [](const auto& colorRenderTarget) {
            return colorRenderTarget.renderTarget;
        });
        commandList.barrier(
           {renderTargets.begin(), renderTargets.end()},
           vireo::ResourceState::SHADER_READ,
           vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.beginRendering(renderingConfig);
        commandList.setStencilReference(1);
        scene.drawOpaquesModels(
          commandList,
          pipelines);
        scene.drawTransparentModels(
          commandList,
          pipelines);
        commandList.endRendering();
        commandList.barrier(
            {renderTargets.begin(), renderTargets.end()},
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::SHADER_READ);
    }

    void GBufferPass::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        for (auto& frame : framesData) {
            frame.positionBuffer = ctx.vireo->createRenderTarget(
                pipelineConfig.colorRenderFormats[BUFFER_POSITION],
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[BUFFER_POSITION].clearValue,
                1,
                vireo::MSAA::NONE,
                "Position");
            frame.normalBuffer = ctx.vireo->createRenderTarget(
                pipelineConfig.colorRenderFormats[BUFFER_NORMAL],
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[BUFFER_NORMAL].clearValue,
                1,
                vireo::MSAA::NONE,
                "Normal");
            frame.albedoBuffer = ctx.vireo->createRenderTarget(
                pipelineConfig.colorRenderFormats[BUFFER_ALBEDO],
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[BUFFER_ALBEDO].clearValue,
                1,
                vireo::MSAA::NONE,
                "Albedo");
            frame.emissiveBuffer = ctx.vireo->createRenderTarget(
                pipelineConfig.colorRenderFormats[BUFFER_EMISSIVE],
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[BUFFER_EMISSIVE].clearValue,
                1,
                vireo::MSAA::NONE,
                "Emissive");
            commandList->barrier(
                {
                    frame.positionBuffer,
                    frame.normalBuffer,
                    frame.albedoBuffer,
                    frame.emissiveBuffer
                },
                vireo::ResourceState::UNDEFINED,
                vireo::ResourceState::SHADER_READ);
        }
    }
}