/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.smaa_pass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.post_processing;
import lysa.renderers.renderpasses.renderpass;

export namespace lysa {

    /**
     * @brief Render pass for Subpixel Morphological Antialiasing (SMAA)
     */
    class SMAAPass : public Renderpass {
    public:
        /**
         * @brief Constructs an SMAAPass
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        SMAAPass(const Context& ctx,
            const RendererConfiguration& config);

        /**
         * @brief Renders the SMAA pass
         * @param commandList The command list to record rendering commands into
         * @param colorAttachment The input color attachment to antialias
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            uint32 frameIndex);

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        /**
         * @brief Gets the color attachment for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the resulting antialiased color render target
         */
        virtual std::shared_ptr<vireo::RenderTarget> getColorAttachment(const uint32 frameIndex) {
            return framesData[frameIndex].colorBuffer;
        }

        auto getEdgeDetectBuffer(const uint32 frameIndex) {
            return framesData[frameIndex].edgeDetectBuffer;
        }

        auto getBlendWeightBuffer(const uint32 frameIndex) {
            return framesData[frameIndex].blendWeightBuffer;
        }


    private:
        const std::string EDGE_DETECT_FRAGMENT_SHADER{"smaa_edge_detect.frag"};
        const std::string BLEND_WEIGHT_FRAGMENT_SHADER{"smaa_blend_weight.frag"};
        const std::string BLEND_FRAGMENT_SHADER{"smaa_neighborhood_blend.frag"};

        struct Data {
            float edgeThreshold;
            int   blendMaxSteps;
        };

        struct FrameData {
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
            std::shared_ptr<vireo::RenderTarget>  edgeDetectBuffer;
            std::shared_ptr<vireo::RenderTarget>  blendWeightBuffer;
            std::shared_ptr<vireo::RenderTarget>  colorBuffer;
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorRenderFormats = { vireo::ImageFormat::R16G16_SFLOAT },
            .colorBlendDesc = {{}},
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{ }},
        };

        Data data;
        std::vector<FrameData> framesData;
        PostProcessing::PostProcessingParams params;
        std::shared_ptr<vireo::Buffer> dataBuffer;
        std::shared_ptr<vireo::Buffer> paramsBuffer;
        std::vector<std::shared_ptr<vireo::Image>> textures;
        std::shared_ptr<vireo::GraphicPipeline> edgeDetectPipeline;
        std::shared_ptr<vireo::GraphicPipeline> blendWeightPipeline;
        std::shared_ptr<vireo::GraphicPipeline> blendPipeline;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
    };
}