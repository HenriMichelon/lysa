/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpass.depth_prepass;

import lysa.renderers.renderer;

namespace lysa {
    DepthPrepass::DepthPrepass(
        Context& ctx,
        const RendererConfiguration& config,
        const bool withStencil):
        Renderpass{ctx, config, "Depth pre-pass"} {
        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.stencilTestEnable = withStencil;
        pipelineConfig.backStencilOpState = pipelineConfig.frontStencilOpState;
        /*pipelineConfig.resources = ctx.vireo->createPipelineResources({
            Resources::descriptorLayout,
            Application::getResources().getSamplers().getDescriptorLayout(),
            Scene::sceneDescriptorLayout,
            Scene::pipelineDescriptorLayout,
            Scene::sceneDescriptorLayoutOptional1},
            Scene::instanceIndexConstantDesc, name);*/
        renderingConfig.stencilTestEnable = pipelineConfig.stencilTestEnable;
    }

    void DepthPrepass::render(
        vireo::CommandList& commandList,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment) {
        renderingConfig.depthStencilRenderTarget = depthAttachment;
        commandList.beginRendering(renderingConfig);
        if (pipelineConfig.stencilTestEnable) {
            commandList.setStencilReference(1);
        }
        // scene.drawOpaquesModels(
        //   commandList,
        //   pipelines);
        commandList.endRendering();
    }
}