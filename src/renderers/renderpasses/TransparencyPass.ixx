/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.transparency_pass;

import vireo;
import lysa.context;
import lysa.resources.material;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.renderpass;

export namespace lysa {

    /**
     * Render pass for transparent objects using Order-Independent Transparency (OIT)
     */
    class TransparencyPass : public Renderpass {
    public:
        /**
         * Constructs a TransparencyPass
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        TransparencyPass(
            const Context& ctx,
            const RendererConfiguration& config);

        /**
         * Updates the OIT graphics pipelines based on active pipeline IDs
         * @param pipelineIds Map of pipeline IDs to unique object IDs
         */
        void updatePipelines(
           const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /**
         * Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(
            const vireo::Extent& extent,
            const std::shared_ptr<vireo::CommandList>& commandList) override;

        /**
         * Renders the transparency pass
         * @param commandList The command list to record rendering commands into
         * @param scene The scene frame data
         * @param colorAttachment The target color attachment
         * @param depthAttachment The target depth attachment
         * @param clearAttachment Whether to clear the color attachment
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex);

        auto getAccumBuffer(const uint32 frameIndex) { return framesData[frameIndex].accumBuffer; }

        auto getRevealageBuffer(const uint32 frameIndex) { return framesData[frameIndex].revealageBuffer; }

    private:
        const std::string VERTEX_SHADER_OIT{"default.vert"};
        const std::string FRAGMENT_SHADER_OIT{"transparency_oit.frag"};
        const std::string VERTEX_SHADER_COMPOSITE{"quad.vert"};
        const std::string FRAGMENT_SHADER_COMPOSITE{"transparency_oit_composite.frag"};

        static constexpr vireo::DescriptorIndex BINDING_ACCUM_BUFFER{0};
        static constexpr vireo::DescriptorIndex BINDING_REVEALAGE_BUFFER{1};

        struct FrameData {
            std::shared_ptr<vireo::DescriptorSet> compositeDescriptorSet;
            std::shared_ptr<vireo::RenderTarget>  accumBuffer;
            std::shared_ptr<vireo::RenderTarget>  revealageBuffer;
        };

        vireo::GraphicPipelineConfiguration oitPipelineConfig {
            .colorRenderFormats  = {
                vireo::ImageFormat::R16G16B16A16_SFLOAT, // Color accumulation
                vireo::ImageFormat::R16_SFLOAT,          // Alpha accumulation
            },
            .colorBlendDesc = {
                {
                    .blendEnable = true,
                    .srcColorBlendFactor = vireo::BlendFactor::ONE,
                    .dstColorBlendFactor = vireo::BlendFactor::ONE,
                    .colorBlendOp = vireo::BlendOp::ADD,
                    .srcAlphaBlendFactor = vireo::BlendFactor::ONE,
                    .dstAlphaBlendFactor = vireo::BlendFactor::ONE,
                    .alphaBlendOp = vireo::BlendOp::ADD,
                    .colorWriteMask = vireo::ColorWriteMask::ALL,
                },
                {
                    .blendEnable = true,
                    .srcColorBlendFactor = vireo::BlendFactor::ZERO,
                    .dstColorBlendFactor = vireo::BlendFactor::ONE_MINUS_SRC_COLOR,
                    .colorBlendOp = vireo::BlendOp::ADD,
                    .srcAlphaBlendFactor = vireo::BlendFactor::ONE,
                    .dstAlphaBlendFactor = vireo::BlendFactor::ONE,
                    .alphaBlendOp = vireo::BlendOp::ADD,
                    .colorWriteMask = vireo::ColorWriteMask::RED,
                }},
            .depthTestEnable = true,
            .depthWriteEnable = false
        };

        vireo::RenderingConfiguration oitRenderingConfig {
            .colorRenderTargets = {
                {
                    .clear = true,
                    .clearValue = {0.0f, 0.0f, 0.0f, 0.0f},
                },
                {
                    .clear = true,
                    .clearValue = {1.0f, 0.0f, 0.0f, 0.0f},
                }
            },
            .depthTestEnable = oitPipelineConfig.depthTestEnable,
        };

        vireo::GraphicPipelineConfiguration compositePipelineConfig {
            .colorBlendDesc = {
            {
                    .blendEnable = true,
                    .srcColorBlendFactor = vireo::BlendFactor::ONE_MINUS_SRC_ALPHA,
                    .dstColorBlendFactor = vireo::BlendFactor::SRC_ALPHA,
                    .colorBlendOp = vireo::BlendOp::ADD,
                    .srcAlphaBlendFactor = vireo::BlendFactor::ONE,
                    .dstAlphaBlendFactor = vireo::BlendFactor::ZERO,
                    .alphaBlendOp = vireo::BlendOp::ADD,
                    .colorWriteMask = vireo::ColorWriteMask::ALL,
                }
            },
            .depthTestEnable = false,
            .depthWriteEnable = false
        };

        vireo::RenderingConfiguration compositeRenderingConfig {
            .colorRenderTargets = {{}},
            .depthTestEnable = compositePipelineConfig.depthTestEnable,
        };

        const MaterialManager& materialManager;
        std::vector<FrameData> framesData;
        std::shared_ptr<vireo::Pipeline> compositePipeline;
        std::shared_ptr<vireo::DescriptorLayout> compositeDescriptorLayout;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> oitPipelines;
    };
}