/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderer;

import std;
import vireo;
import lysa.global;
import lysa.configuration;
import lysa.samplers;
import lysa.scene;
import lysa.resources.material;
import lysa.renderers.renderpass.post_processing;
import lysa.renderers.renderpass.depth_prepass;
import lysa.renderers.renderpass.shader_material_pass;
import lysa.renderers.renderpass.shadow_map_pass;
import lysa.renderers.renderpass.transparency_pass;

export namespace lysa {
    class Renderer {
    public:
        struct FrameData {
            std::shared_ptr<vireo::RenderTarget> colorAttachment;
            std::shared_ptr<vireo::RenderTarget> depthAttachment;
        };

        Renderer(
            const RenderingConfiguration& config,
            bool withStencil,
            const std::wstring& name);

        virtual void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList);

        std::shared_ptr<vireo::RenderTarget> getColorAttachment(uint32 frameIndex) const;

        auto getColorRenderTarget(const uint32 frameIndex) const {
            return framesData[frameIndex].colorAttachment;
        }

        auto getDepthRenderTarget(const uint32 frameIndex) const {
            return framesData[frameIndex].depthAttachment;
        }

        virtual std::shared_ptr<vireo::RenderTarget> getBloomColorAttachment(uint32 frameIndex) const = 0;

        void updatePipelines(const Scene& scene);

        virtual void update(uint32 frameIndex);

        void compute(
            vireo::CommandList& commandList,
            Scene& scene,
            uint32 frameIndex) const;

        void preRender(
            vireo::CommandList& commandList,
            const Scene& scene,
            uint32 frameIndex);

        void render(
            vireo::CommandList& commandList,
            const Scene& scene,
            bool clearAttachment,
            uint32 frameIndex);

        void postprocess(
            vireo::CommandList& commandList,
            const vireo::Viewport&viewport,
            const vireo::Rect&scissor,
            uint32 frameIndex);

        void addPostprocessing(
            const std::wstring& fragShaderName,
            bool useRenderingColorAttachmentFormat,
            void* data = nullptr,
            uint32 dataSize = 0);

        void removePostprocessing(const std::wstring& fragShaderName);

        virtual ~Renderer() = default;
        Renderer(Renderer&) = delete;
        Renderer& operator=(Renderer&) = delete;

    protected:
        const RenderingConfiguration& config;
        const std::wstring name;
        const bool withStencil;

        virtual void updatePipelines(
            const std::unordered_map<pipeline_id, std::vector<std::shared_ptr<Material>>>& pipelineIds) = 0;

        virtual void colorPass(
            vireo::CommandList& commandList,
            const Scene& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex) = 0;

    private:
        struct {
            uint32 kernelSize;
            float  strength;
        } bloomBlurData;

        vireo::Extent          currentExtent{};
        std::vector<FrameData> framesData;
        DepthPrepass           depthPrePass;
        ShaderMaterialPass     shaderMaterialPass;
        TransparencyPass       transparencyPass;

        std::unique_ptr<PostProcessing> bloomBlurPass;
        std::vector<std::shared_ptr<PostProcessing>> postProcessingPasses;
    };
}