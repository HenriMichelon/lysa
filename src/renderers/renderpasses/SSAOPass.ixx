/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.ssao_pass;

import vireo;
import lysa.context;
import lysa.math;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.gbuffer_pass;
import lysa.renderers.renderpasses.renderpass;

export namespace lysa {

    /**
     * @brief Render pass for Screen Space Ambient Occlusion (SSAO)
     */
    class SSAOPass : public Renderpass {
    public:
        /**
         * @brief Constructs an SSAOPass
         * @param ctx The engine context
         * @param config The renderer configuration
         * @param gBufferPass Reference to the G-buffer pass providing input textures
         * @param withStencil Whether to enable stencil testing
         */
        SSAOPass(
            const Context& ctx,
            const RendererConfiguration& config,
            const GBufferPass& gBufferPass,
            bool withStencil);

        /**
         * @brief Renders the SSAO pass
         * @param commandList The command list to record rendering commands into
         * @param scene The scene frame data
         * @param depthAttachment The target depth attachment
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            uint32 frameIndex);

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        /**
         * @brief Gets the SSAO color buffer for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the SSAO color render target
         */
        auto getSSAOColorBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].ssaoColorBuffer;
        }

        /**
         * @brief Gets the image format used for the SSAO buffer
         * @return The SSAO buffer image format
         */
        auto getSSAOBufferFormat() const {
            return pipelineConfig.colorRenderFormats[0];
        }

    private:
        const std::string VERTEX_SHADER{"quad.vert"};
        const std::string FRAGMENT_SHADER{"ssao.frag"};

        static constexpr vireo::DescriptorIndex BINDING_PARAMS{0};
        static constexpr vireo::DescriptorIndex BINDING_POSITION_BUFFER{1};
        static constexpr vireo::DescriptorIndex BINDING_NORMAL_BUFFER{2};
        static constexpr vireo::DescriptorIndex BINDING_NOISE_TEXTURE{3};

        struct Params {
            float2 screenSize;
            float2 noiseScale;
            float  radius;
            float  bias;
            float  power;
            uint   sampleCount{64};
            float4 samples[64];
        };

        struct FrameData {
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
            std::shared_ptr<vireo::RenderTarget> ssaoColorBuffer;
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorRenderFormats = { vireo::ImageFormat::R8_UNORM },
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

        Params params;
        const GBufferPass& gBufferPass;
        std::vector<FrameData> framesData;
        std::shared_ptr<vireo::Buffer> paramsBuffer;
        std::shared_ptr<vireo::Image> noiseTexture;
        std::shared_ptr<vireo::GraphicPipeline> pipeline;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
    };
}