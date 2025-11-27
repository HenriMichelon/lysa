/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.forward_renderer;

namespace lysa {

    ForwardRenderer::ForwardRenderer(
        Context& ctx,
        const RendererConfiguration& config,
        const SwapChainConfiguration& swapChainConfig) :
        Renderer(ctx, config, swapChainConfig, false) {
    }

    void ForwardRenderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        Renderer::resize(extent, commandList);
    }

}