/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.depth_prepass;

import lysa.renderers.graphic_pipeline_data;

namespace lysa {
    DepthPrepass::DepthPrepass(
        const Context& ctx,
        const RendererConfiguration& config,
        const bool withStencil,
        const uint32 framesInFlight):
        Renderpass{ctx, config, "Depth pre-pass"},
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
        renderingConfig.stencilTestEnable = pipelineConfig.stencilTestEnable;
        framesData.resize(framesInFlight);
    }

    void DepthPrepass::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        for (const auto& [pipelineId, materials] : pipelineIds) {
            if (!pipelines.contains(pipelineId)) {
                const auto& material = materials.at(0);
                pipelineConfig.cullMode = materialManager[material].getCullMode();
                pipelineConfig.vertexShader = loadShader(VERTEX_SHADER);
                pipelineConfig.vertexInputLayout = ctx.vireo->createVertexLayout(sizeof(VertexData), VertexData::vertexAttributes);
                pipelineConfig.msaa = config.msaa;
                pipelines[pipelineId] = ctx.vireo->createGraphicPipeline(pipelineConfig, name + ":" + std::to_string(pipelineId));
            }
        }
    }

    void DepthPrepass::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];
        renderingConfig.depthStencilRenderTarget = depthAttachment;
        renderingConfig.multisampledDepthStencilRenderTarget = frame.multisampledDepthAttachment;
        commandList.beginRendering(renderingConfig);
        if (pipelineConfig.stencilTestEnable) {
            commandList.setStencilReference(1);
        }
        scene.drawOpaquesModels(commandList, pipelines);
        commandList.endRendering();
    }

    void DepthPrepass::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        if (config.msaa != vireo::MSAA::NONE) {
            for (auto& frame : framesData) {
                frame.multisampledDepthAttachment = ctx.vireo->createRenderTarget(
                    config.depthStencilFormat,
                    extent.width, extent.height,
                    vireo::RenderTargetType::DEPTH,
                    { .depthStencil = { .depth = 1.0f, .stencil = 0 } },
                    1,
                    config.msaa,
                    "MSAA depth stencil attachment");
                const auto depthStage =
                    config.depthStencilFormat == vireo::ImageFormat::D32_SFLOAT_S8_UINT ||
                    config.depthStencilFormat == vireo::ImageFormat::D24_UNORM_S8_UINT   ?
                    vireo::ResourceState::RENDER_TARGET_DEPTH_STENCIL :
                    vireo::ResourceState::RENDER_TARGET_DEPTH;
                commandList->barrier(
                    frame.multisampledDepthAttachment,
                    vireo::ResourceState::UNDEFINED,
                    depthStage);
            }
        }
    }
}