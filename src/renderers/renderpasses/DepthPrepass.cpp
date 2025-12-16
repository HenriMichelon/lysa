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
        const bool withStencil):
        Renderpass{ctx, config, "Depth pre-pass"},
        materialManager(ctx.res.get<MaterialManager>()) {
        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.stencilTestEnable = withStencil;
        pipelineConfig.backStencilOpState = pipelineConfig.frontStencilOpState;
        pipelineConfig.resources = ctx.vireo->createPipelineResources({
            ctx.globalDescriptorLayout,
            ctx.samplers.getDescriptorLayout(),
            SceneRenderContext::sceneDescriptorLayout,
            GraphicPipelineData::pipelineDescriptorLayout,
            SceneRenderContext::sceneDescriptorLayoutOptional1},
            SceneRenderContext::instanceIndexConstantDesc, name);
        renderingConfig.stencilTestEnable = pipelineConfig.stencilTestEnable;
    }

    void DepthPrepass::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        for (const auto& [pipelineId, materials] : pipelineIds) {
            if (!pipelines.contains(pipelineId)) {
                const auto& material = materials.at(0);
                pipelineConfig.cullMode = materialManager[material].getCullMode();
                pipelineConfig.vertexShader = loadShader(VERTEX_SHADER);
                pipelineConfig.vertexInputLayout = ctx.vireo->createVertexLayout(sizeof(VertexData), VertexData::vertexAttributes);
                pipelines[pipelineId] = ctx.vireo->createGraphicPipeline(pipelineConfig, name + ":" + std::to_string(pipelineId));
            }
        }
    }

    void DepthPrepass::render(
        vireo::CommandList& commandList,
        const SceneRenderContext& scene,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment) {
        renderingConfig.depthStencilRenderTarget = depthAttachment;
        commandList.beginRendering(renderingConfig);
        if (pipelineConfig.stencilTestEnable) {
            commandList.setStencilReference(1);
        }
        scene.drawOpaquesModels(commandList, pipelines);
        commandList.endRendering();
    }
}