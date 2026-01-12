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

    /**
     * @brief Render pass for deferred lighting
     */
    class LightingPass : public Renderpass {
    public:
        /**
         * @brief Constructs a LightingPass
         * @param ctx The engine context
         * @param config The renderer configuration
         * @param gBufferPass Reference to the G-buffer pass providing input textures
         * @param withStencil Whether to enable stencil testing
         */
        LightingPass(
            const Context& ctx,
            const RendererConfiguration& config, const
            GBufferPass& gBufferPass,
            bool withStencil);

        /**
         * @brief Renders the deferred lighting pass
         * @param commandList The command list to record rendering commands into
         * @param scene The scene frame data
         * @param colorAttachment The target color attachment
         * @param depthAttachment The target depth attachment
         * @param aoMap The ambient occlusion map
         * @param clearAttachment Whether to clear the color attachment
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            const std::shared_ptr<vireo::RenderTarget>& aoMap,
            bool clearAttachment,
            uint32 frameIndex);

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        /**
         * @brief Gets the brightness buffer for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the brightness render target
         */
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