/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderer;

import vireo;

import lysa.context;
import lysa.math;
import lysa.types;
import lysa.renderers.configuration;
import lysa.renderers.renderpass.depth_prepass;

export namespace lysa {

    /**
     * @breif High-level scene renderer
     *  - Own and manage the set of render passes required by a rendering path
     *    (depth pre-pass, opaque/transparent color, shader-material passes, SMAA,
     *    bloom and other post-processing).
     *  - Allocate per-frame color/depth attachments and expose them to callers.
     *  - Update and (re)build graphics pipelines when the set of materials changes.
     */
    class Renderer {
    public:
        static std::unique_ptr<Renderer> create(
            Context& ctx,
            const RendererConfiguration& config);

        /** Returns the color attachment of the current renderer for the frame. */
        std::shared_ptr<vireo::RenderTarget> getCurrentColorAttachment(uint32 frameIndex) const;

        /** Accessor for the color render target of the frame. */
        auto getFrameColorAttachment(const uint32 frameIndex) const {
            return framesData[frameIndex].colorAttachment;
        }

        /** Accessor for the depth render target of the frame. */
        auto getFrameDepthAttachment(const uint32 frameIndex) const {
            return framesData[frameIndex].depthAttachment;
        }

        /**
         * Recreates attachments/pipelines after a resize.
         * @param extent       New swap chain extent.
         * @param commandList  Command list used for any required transitions/copies.
         */
        virtual void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList);

        /** Performs per-frame housekeeping (e.g., pass-local data updates). */
        virtual void update(uint32 frameIndex);

        /** Pre-render stage: uploads, layout transitions, depth pre pass and shadow maps. */
        void preRender(
            vireo::CommandList& commandList,
            uint32 frameIndex);

        /** Main render stage: records opaque/transparent draw calls. */
        void render(
            vireo::CommandList& commandList,
            bool clearAttachment,
            uint32 frameIndex);

        virtual ~Renderer() = default;
        Renderer(Renderer&) = delete;
        Renderer& operator=(Renderer&) = delete;

    protected:
        const Context& ctx;
        const bool withStencil;
        const RendererConfiguration config;
        // Depth-only pre-pass used by both forward and deferred renderers
        DepthPrepass depthPrePass;

        Renderer(
            const Context& ctx,
            const RendererConfiguration& config,
            bool withStencil);

        /**
        * Records the pipeline-specific color pass for the concrete renderer.
        * Implementations dispatch scene draws into colorAttachment/depthAttachment.
        */
        virtual void colorPass(
            vireo::CommandList& commandList,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex) = 0;

    private:
        /** Per-frame attachments owned by the renderer. */
        struct FrameData {
            std::shared_ptr<vireo::RenderTarget> colorAttachment;
            std::shared_ptr<vireo::RenderTarget> depthAttachment;
        };

        vireo::Extent currentExtent{};
        std::vector<FrameData> framesData;

    };
}
