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
import lysa.renderers.configuration;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.depth_prepass;
import lysa.renderers.renderpasses.shader_material_pass;
import lysa.renderers.renderpasses.transparency_pass;
import lysa.resources.camera;
import lysa.resources.mesh;

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

        /**
         * Updates graphics pipelines according to the Scene material mapping.
         * Convenience overload that pulls mapping from the Scene.
         */
        void updatePipelines(const SceneFrameData& scene);

        /**
         * Updates graphics pipelines according to the provided materials mapping.
         * @param pipelineIds Map of pipeline family id to materials.
         */
        virtual void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /** Performs per-frame housekeeping (e.g., pass-local data updates). */
        virtual void update(uint32 frameIndex);

        /** Pre-render stage: uploads, layout transitions, depth pre pass and shadow maps. */
        void prepare(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissors,
            uint32 frameIndex);

        /** Main render stage: records opaque/transparent draw calls. */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissors,
            bool clearAttachment,
            uint32 frameIndex);

        /** Applies post-processing chain (SMAA, bloom, custom passes). */
        // void postprocess(
        //     vireo::CommandList& commandList,
        //     const vireo::Viewport&viewport,
        //     const vireo::Rect&scissor,
        //     uint32 frameIndex);

        /** Adds a full-screen post-processing pass by fragment shader name. */
        // void addPostprocessing(
        //     const std::string& fragShaderName,
        //     vireo::ImageFormat outputFormat,
        //     void* data = nullptr,
        //     uint32 dataSize = 0);

        /** Removes a previously added post-processing pass by fragment name. */
        // void removePostprocessing(const std::string& fragShaderName);

        virtual ~Renderer() = default;
        Renderer(Renderer&) = delete;
        Renderer& operator=(Renderer&) = delete;

    protected:
        const Context& ctx;
        const bool withStencil;
        const RendererConfiguration config;
        // Depth-only pre-pass used by both forward and deferred renderers
        DepthPrepass depthPrePass;
        // Renders objects using custom shader materials.
        ShaderMaterialPass shaderMaterialPass;
        // Transparent objects pass (sorted/blended).
        TransparencyPass transparencyPass;
        // Per-frame attachments owned by the renderer.
        struct FrameData {
            std::shared_ptr<vireo::RenderTarget> colorAttachment;
            std::shared_ptr<vireo::RenderTarget> depthAttachment;
        };
        std::vector<FrameData> framesData;

        Renderer(
            const Context& ctx,
            const RendererConfiguration& config,
            bool withStencil);

        /*
        * Records the pipeline-specific color pass for the concrete renderer.
        * Implementations dispatch scene draws into colorAttachment/depthAttachment.
        */
        virtual void colorPass(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            bool clearAttachment,
            uint32 frameIndex) = 0;

    private:
        const MeshManager& meshManager;
        vireo::Extent currentExtent{};

    };
}
