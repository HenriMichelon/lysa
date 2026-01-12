/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.lighting_pass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.renderpass;
import lysa.renderers.renderpasses.gbuffer_pass;

export namespace lysa {

    class LightingPass : public Renderpass {
    public:
        LightingPass(
            const Context& ctx,
            const RendererConfiguration& config, const
            GBufferPass& gBufferPass,
            bool withStencil);

        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            const std::shared_ptr<vireo::RenderTarget>& aoMap,
            bool clearAttachment,
            uint32 frameIndex);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        auto getBrightnessBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].brightnessBuffer;
        }

    private:
        const std::string VERTEX_SHADER{"quad.vert"};
        const std::string FRAGMENT_SHADER{"glighting.frag"};
        const std::string FRAGMENT_SHADER_BLOOM{"glighting_bloom.frag"};

        static constexpr vireo::DescriptorIndex BINDING_POSITION_BUFFER{0};
        static constexpr vireo::DescriptorIndex BINDING_NORMAL_BUFFER{1};
        static constexpr vireo::DescriptorIndex BINDING_ALBEDO_BUFFER{2};
        static constexpr vireo::DescriptorIndex BINDING_EMISSIVE_BUFFER{3};
        static constexpr vireo::DescriptorIndex BINDING_AO_MAP{4};

        struct FrameData {
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
            std::shared_ptr<vireo::RenderTarget> brightnessBuffer;
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = {{}},
            .frontStencilOpState = {
                .failOp = vireo::StencilOp::KEEP,
                .passOp = vireo::StencilOp::KEEP,
                .depthFailOp = vireo::StencilOp::KEEP,
                .compareOp = vireo::CompareOp::EQUAL,
                .compareMask = 0xff,
                .writeMask = 0x00
            }
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{ }},
            .depthTestEnable    = pipelineConfig.depthTestEnable,
        };

        const GBufferPass& gBufferPass;
        std::vector<FrameData> framesData;
        std::shared_ptr<vireo::GraphicPipeline> pipeline;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
    };
}