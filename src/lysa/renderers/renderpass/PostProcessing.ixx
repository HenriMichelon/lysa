/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpass.post_processing;

import std;
import vireo;
import lysa.global;
import lysa.configuration;
import lysa.scene;
import lysa.renderers.renderpass;

export namespace lysa {
    class PostProcessing : public Renderpass {
    public:
        PostProcessing(
            const RenderingConfiguration& config,
            const std::wstring& fragShaderName,
            bool useRenderingColorAttachmentFormat,
            void* data,
            uint32 dataSize,
            const std::wstring& name);

        void update(uint32 frameIndex) override;

        void render(
           uint32 frameIndex,
           const vireo::Viewport&viewport,
           const vireo::Rect&scissor,
           const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
           const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
           const std::shared_ptr<vireo::RenderTarget>& bloomColorAttachment,
           vireo::CommandList& commandList);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        virtual std::shared_ptr<vireo::RenderTarget> getColorAttachment(const uint32 frameIndex) {
            return framesData[frameIndex].colorAttachment;
        }

        const auto& getFragShaderName() const { return fragShaderName; }

    protected:
        const std::wstring VERTEX_SHADER{L"quad.vert"};

        static constexpr vireo::DescriptorIndex BINDING_PARAMS{0};
        static constexpr vireo::DescriptorIndex BINDING_DATA{1};

        static constexpr vireo::DescriptorIndex BINDING_INPUT{0};
        static constexpr vireo::DescriptorIndex BINDING_DEPTH_BUFFER{1};
        static constexpr vireo::DescriptorIndex BINDING_BLOOM_BUFFER{2};

        struct PostProcessingParams {
            uint32 applyBloom;
            float  time;
            uint2  imageSize;
        };

        struct FrameData {
            PostProcessingParams                  params;
            std::shared_ptr<vireo::Buffer>        paramsUniform;
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
            std::shared_ptr<vireo::DescriptorSet> descriptorSetBuffers;
            std::shared_ptr<vireo::RenderTarget>  colorAttachment;
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = {{}}
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{}}
        };

        const std::wstring                       fragShaderName;
        void*                                    data{nullptr};
        std::shared_ptr<vireo::Buffer>           dataUniform{nullptr};
        std::vector<FrameData>                   framesData;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayoutBuffers;
        std::shared_ptr<vireo::GraphicPipeline>  pipeline;

    };
}