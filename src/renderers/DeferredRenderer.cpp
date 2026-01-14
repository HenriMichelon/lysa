/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.deferred_renderer;

namespace lysa {

    DeferredRenderer::DeferredRenderer(
        const Context& ctx,
        const RendererConfiguration& config) :
        Renderer{ctx, config},
        ssaoBlurData{.kernelSize = config.ssaoBlurKernelSize},
        gBufferPass{ctx, config, withStencil},
        lightingPass{ctx, config, gBufferPass, withStencil} {
        if (config.ssaoEnabled) {
            ssaoPass = std::make_unique<SSAOPass>(ctx, config, gBufferPass, withStencil);
            ssaoBlurPass = std::make_unique<PostProcessing>(
                ctx,
                config,
                "ssao_blur",
                &ssaoBlurData,
                sizeof(ssaoBlurData),
                ssaoPass->getSSAOBufferFormat(),
                "SSAO Blur");
        }
    }

    void DeferredRenderer::update(const uint32 frameIndex) {
        Renderer::update(frameIndex);
        if (config.ssaoEnabled) { ssaoBlurPass->update(frameIndex); }
    }

    void DeferredRenderer::updatePipelines(
        const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        Renderer::updatePipelines(pipelineIds);
        gBufferPass.updatePipelines(pipelineIds);
    }

    void DeferredRenderer::colorPass(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors,
        const bool clearAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];
        gBufferPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            clearAttachment,
            frameIndex);
        if (config.ssaoEnabled) {
            ssaoPass->render(
                commandList,
                scene,
                frame.depthAttachment,
                frameIndex);
            ssaoBlurPass->render(
                commandList,
                viewport,
                scissors,
                ssaoPass->getSSAOColorBuffer(frameIndex),
                nullptr,
                frameIndex);
        }
        lightingPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            config.ssaoEnabled ? ssaoBlurPass->getColorAttachment(frameIndex) : nullptr,
            true,
            frameIndex);
        if (config.ssaoEnabled) {
            commandList.barrier(
                ssaoBlurPass->getColorAttachment(frameIndex),
                vireo::ResourceState::SHADER_READ,
                vireo::ResourceState::UNDEFINED);
        }
    }

    void DeferredRenderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        Renderer::resize(extent, commandList);
        gBufferPass.resize(extent, commandList);
        if (config.ssaoEnabled) {
            ssaoBlurData.update(extent, 1.2);
            ssaoPass->resize(extent, commandList);
            ssaoBlurPass->resize(extent);
        }
        lightingPass.resize(extent, commandList);
    }

}