/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderer;

import lysa.exception;
import lysa.renderers.renderpasses.renderpass;
#ifdef FORWARD_RENDERER
import lysa.renderers.forward_renderer;
#endif

namespace lysa {

    std::unique_ptr<Renderer> Renderer::create(
        Context& ctx,
        const RendererConfiguration& config,
        const uint32 framesInFlight) {
#ifdef FORWARD_RENDERER
        if (config.rendererType == RendererType::FORWARD) {
            return std::make_unique<ForwardRenderer>(ctx, config, framesInFlight);
        }
#endif
        throw Exception("Unknown renderer type");
    }

    Renderer::Renderer(
        const Context& ctx,
        const RendererConfiguration& config,
        const uint32 framesInFlight,
        const bool withStencil):
        ctx(ctx),
        withStencil(withStencil),
        config(config),
        depthPrePass(ctx, config, withStencil),
        meshManager(ctx.res.get<MeshManager>()) {
        framesData.resize(framesInFlight);
    }

    void Renderer::update(const uint32 frameIndex) {
        depthPrePass.update(frameIndex);
    }

    void Renderer::updatePipelines(const SceneFrameData& scene) {
        const auto& pipelineIds = scene.getPipelineIds();
        // for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            // static_pointer_cast<ShadowMapPass>(shadowMapRenderer)->updatePipelines(pipelineIds);
        // }
        updatePipelines(pipelineIds);
    }

    void Renderer::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        depthPrePass.updatePipelines(pipelineIds);
        // shaderMaterialPass.updatePipelines(pipelineIds);
        // transparencyPass.updatePipelines(pipelineIds);
    }

    void Renderer::compute(
       vireo::CommandList& commandList,
       SceneFrameData& scene,
       const Camera& camera,
       const uint32 frameIndex) const {
        // auto resourcesLock = std::lock_guard{Application::getResources().getMutex()};
        // for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            // shadowMapRenderer->update(frameIndex);
        // }
        scene.update(commandList, camera);
        scene.compute(commandList, camera);
    }

    void Renderer::prepare(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const uint32 frameIndex) {
        commandList.bindVertexBuffer(meshManager.getVertexBuffer());
        commandList.bindIndexBuffer(meshManager.getIndexBuffer());
        // for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            // static_pointer_cast<ShadowMapPass>(shadowMapRenderer)->render(commandList, scene);
        // }
        depthPrePass.render(commandList, scene, framesData[frameIndex].depthAttachment);
    }

    void Renderer::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const bool clearAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];
        commandList.bindVertexBuffer(meshManager.getVertexBuffer());
        commandList.bindIndexBuffer(meshManager.getIndexBuffer());
        colorPass(commandList, scene, frame.colorAttachment, frame.depthAttachment, clearAttachment, frameIndex);
    }

    void Renderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        currentExtent = extent;
        for (auto& frame : framesData) {
            frame.colorAttachment = ctx.vireo->createRenderTarget(
               config.colorRenderingFormat,
               extent.width, extent.height,
               vireo::RenderTargetType::COLOR,
               {config.clearColor.r, config.clearColor.g, config.clearColor.b, 1.0f},
               1,
               config.msaa,
               "Main color attachment");
            frame.depthAttachment = ctx.vireo->createRenderTarget(
                config.depthStencilFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::DEPTH,
                { .depthStencil = { .depth = 1.0f, .stencil = 0 } },
                1,
                config.msaa,
                "Main depth stencil attachment");
            const auto depthStage =
               config.depthStencilFormat == vireo::ImageFormat::D32_SFLOAT_S8_UINT ||
               config.depthStencilFormat == vireo::ImageFormat::D24_UNORM_S8_UINT   ?
               vireo::ResourceState::RENDER_TARGET_DEPTH_STENCIL :
               vireo::ResourceState::RENDER_TARGET_DEPTH;
            commandList->barrier(
                frame.depthAttachment,
                vireo::ResourceState::UNDEFINED,
                depthStage);
        }
    }

    std::shared_ptr<vireo::RenderTarget> Renderer::getCurrentColorAttachment(const uint32 frameIndex) const {
        return getFrameColorAttachment(frameIndex);
    }

}