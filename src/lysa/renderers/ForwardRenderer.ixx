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
import lysa.types;
import lysa.renderers.configuration;
import lysa.renderers.renderer;
import lysa.renderers.renderpasses.forward_color;

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
            const Context& ctx,
            const RendererConfiguration& config);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

    protected:
        /** Per-frame housekeeping (post-process data, etc.). */
        void update(uint32 frameIndex) override;

        /** Records the forward color pass followed by transparency. */
        void colorPass(
            vireo::CommandList& commandList,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex) override;


    private:
        /** Opaque/alpha-tested color pass used by forward rendering. */
        ForwardColor forwardColorPass;
    };
}