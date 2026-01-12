/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.forward_color_pass;

import vireo;
import lysa.context;
import lysa.types;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.renderpass;
import lysa.resources.material;

export namespace lysa {

    /**
     * @brief Render pass for forward color rendering
     */
    class ForwardColorPass : public Renderpass {
    public:
        /**
         * @brief Constructs a ForwardColorPass
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        ForwardColorPass(
            const Context& ctx,
            const RendererConfiguration& config);

        /**
         * @brief Updates the graphics pipelines based on active pipeline IDs
         * @param pipelineIds Map of pipeline IDs to unique object IDs
         */
        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /**
         * @brief Renders the forward color pass
         * @param commandList The command list to record rendering commands into
         * @param scene The scene frame data
         * @param colorAttachment The target color attachment
         * @param depthAttachment The target depth attachment
         * @param multisampledDepthAttachment The multisampled depth attachment
         * @param clearAttachment Whether to clear the color attachment
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            const std::shared_ptr<vireo::RenderTarget>& multisampledDepthAttachment,
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
        const std::string DEFAULT_VERTEX_SHADER{"default.vert"};
        const std::string DEFAULT_FRAGMENT_SHADER{"forward.frag"};
        const std::string DEFAULT_FRAGMENT_BLOOM_SHADER{"forward_bloom.frag"};

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = { { .blendEnable = true }},
            .depthTestEnable = true,
            .depthWriteEnable = true,
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{} },
            .depthTestEnable = pipelineConfig.depthTestEnable,
        };

        struct FrameData {
            std::shared_ptr<vireo::RenderTarget> multisampledColorAttachment;
            std::shared_ptr<vireo::RenderTarget> brightnessBuffer;
        };

        const MaterialManager& materialManager;
        std::vector<FrameData> framesData;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;

    };
}