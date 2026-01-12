/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderer;

import lysa.exception;
import lysa.renderers.renderpasses.renderpass;
import lysa.renderers.renderpasses.shadow_map_pass;
#ifdef FORWARD_RENDERER
import lysa.renderers.forward_renderer;
#endif
#ifdef DEFERRED_RENDERER
import lysa.renderers.deferred_renderer;
#endif

namespace lysa {

    std::unique_ptr<Renderer> Renderer::create(
        Context& ctx,
        const RendererConfiguration& config) {
#ifdef FORWARD_RENDERER
        if (config.rendererType == RendererType::FORWARD) {
            return std::make_unique<ForwardRenderer>(ctx, config);
        }
#endif
#ifdef DEFERRED_RENDERER
        if (config.rendererType == RendererType::DEFERRED) {
            return std::make_unique<DeferredRenderer>(ctx, config);
        }
#endif
        throw Exception("Unknown renderer type");
    }

    Renderer::Renderer(
        const Context& ctx,
        const RendererConfiguration& config,
        const bool withStencil):
        ctx(ctx),
        withStencil(withStencil),
        config(config),
        depthPrePass(ctx, config, withStencil),
        transparencyPass(ctx, config),
        shaderMaterialPass(ctx, config),
        meshManager(ctx.res.get<MeshManager>()) {
        framesData.resize(ctx.config.framesInFlight);
    }

    void Renderer::update(const uint32 frameIndex) {
        depthPrePass.update(frameIndex);
    }

    void Renderer::updatePipelines(const SceneFrameData& scene) {
        const auto& pipelineIds = scene.getPipelineIds();
        updatePipelines(pipelineIds);
    }

    void Renderer::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        depthPrePass.updatePipelines(pipelineIds);
        shaderMaterialPass.updatePipelines(pipelineIds);
        transparencyPass.updatePipelines(pipelineIds);
    }

    void Renderer::prepare(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors,
        const uint32 frameIndex) {
        commandList.bindVertexBuffer(meshManager.getVertexBuffer());
        commandList.bindIndexBuffer(meshManager.getIndexBuffer());
        for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            static_pointer_cast<ShadowMapPass>(shadowMapRenderer)->render(commandList, scene);
        }
        commandList.setViewport(viewport);
        commandList.setScissors(scissors);
        depthPrePass.render(commandList, scene, framesData[frameIndex].depthAttachment, frameIndex);
    }

    void Renderer::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors,
        const bool clearAttachment,
        const uint32 frameIndex) {
        commandList.bindVertexBuffer(meshManager.getVertexBuffer());
        commandList.bindIndexBuffer(meshManager.getIndexBuffer());
        commandList.setViewport(viewport);
        commandList.setScissors(scissors);
        colorPass(
            commandList,
            scene,
            viewport,
            scissors,
            clearAttachment,
            frameIndex);
        const auto& frame = framesData[frameIndex];
        shaderMaterialPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            false,
            frameIndex);
        transparencyPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            false,
            frameIndex);
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
               vireo::MSAA::NONE,
               "Main color attachment");
            frame.depthAttachment = ctx.vireo->createRenderTarget(
                config.depthStencilFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::DEPTH,
                { .depthStencil = { .depth = 1.0f, .stencil = 0 } },
                1,
                vireo::MSAA::NONE,
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
        depthPrePass.resize(extent, commandList);
        shaderMaterialPass.resize(extent, commandList);
        transparencyPass.resize(extent, commandList);
    }

    std::shared_ptr<vireo::RenderTarget> Renderer::getCurrentColorAttachment(const uint32 frameIndex) const {
        return getFrameColorAttachment(frameIndex);
    }

    void Renderer::updateBlurData(BlurData& blurData, const vireo::Extent& extent, const float strength) const {
        // Pre-compute Gaussian weights
        if (blurData.kernelSize > 9) { blurData.kernelSize = 9; }
        blurData.texelSize = (1.0 / float2(extent.width, extent.height)) * strength;
        const int halfKernel = blurData.kernelSize / 2;
        float sum = 0.0;
        for (int i = 0; i < blurData.kernelSize; i++) {
            for (int j = 0; j < blurData.kernelSize; j++) {
                const int index = i * blurData.kernelSize + j;
                const float x = static_cast<float>(i - halfKernel) * blurData.texelSize.x;
                const float y = static_cast<float>(j - halfKernel) * blurData.texelSize.y;
                blurData.weights[index].x = std::exp(-(x * x + y * y) / 2.0);
                sum += blurData.weights[index].x;
            }
        }
        // Normalize weights
        for (int i = 0; i < blurData.kernelSize * blurData.kernelSize; i++) {
            blurData.weights[i].x /= sum;
        }
    }
}