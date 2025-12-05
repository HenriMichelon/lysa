/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.forward_renderer;

namespace lysa {

    ForwardRenderer::ForwardRenderer(
        const Context& ctx,
        const RendererConfiguration& config) :
        Renderer(ctx, config, false),
        forwardColorPass(ctx, config) {
    }

    void ForwardRenderer::update(const uint32 frameIndex) {
        Renderer::update(frameIndex);
        forwardColorPass.update(frameIndex);
    }

    void ForwardRenderer::colorPass(
       vireo::CommandList& commandList,
       const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
       const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
       const bool clearAttachment,
       const uint32 frameIndex) {
        forwardColorPass.render(commandList, colorAttachment, depthAttachment, clearAttachment, frameIndex);
    }


    void ForwardRenderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        Renderer::resize(extent, commandList);
        forwardColorPass.resize(extent, commandList);
    }

}