/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.forward_renderer;

import std;
import vireo;

import lysa.context;
import lysa.renderers.renderer;

export namespace lysa {

    /**
     * Forward rendering path.
     *
     * Renders opaque geometry directly to the color/depth attachments using a
     * single forward pass, then handles transparency and optional bloom.
     * Suitable for scenes with many materials requiring complex shading (PBR,
     * alpha test, etc.) without a G-Buffer.
     */
    class ForwardRenderer : public Renderer {
    public:
        ForwardRenderer(
            Context& ctx,
            const RendererConfiguration& config,
            const SwapChainConfiguration& swapChainConfig);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;
    };
}