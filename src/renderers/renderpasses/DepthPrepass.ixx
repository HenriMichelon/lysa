/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.depth_prepass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.renderpasses.renderpass;
import lysa.renderers.scene_frame_data;
import lysa.resources.material;

export namespace lysa {

    /**
     * Render pass for generating the depth pre-pass
     */
    class DepthPrepass : public Renderpass {
    public:
        /**
         * Constructs a DepthPrepass render pass
         * @param ctx The engine context
         * @param config The renderer configuration
         * @param withStencil Whether to enable stencil testing
         */
        DepthPrepass(
            const Context& ctx,
            const RendererConfiguration& config,
            bool withStencil);

        /**
         * Updates the graphics pipelines based on active pipeline IDs
         * @param pipelineIds Map of pipeline IDs to unique object IDs
         */
        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /**
         * Gets the multisampled depth attachment for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the multisampled depth render target
         */
        auto getMultisampledDepthAttachment(const uint32 frameIndex) {
            return framesData[frameIndex].multisampledDepthAttachment;
        }

        /**
         * Renders the depth pre-pass
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
         * Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

    private:
        const std::string VERTEX_SHADER{"depth_prepass.vert"};

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .cullMode = vireo::CullMode::BACK,
            .depthTestEnable = true,
            .depthWriteEnable = true,
            .frontStencilOpState = {
                .failOp      = vireo::StencilOp::KEEP,
                .passOp      = vireo::StencilOp::REPLACE,
                .depthFailOp = vireo::StencilOp::KEEP,
                .compareOp   = vireo::CompareOp::ALWAYS,
                .compareMask = 0xff,
                .writeMask   = 0xff
            }
        };

        vireo::RenderingConfiguration renderingConfig {
            .depthTestEnable = pipelineConfig.depthTestEnable,
            .clearDepthStencil = true,
        };

        struct FrameData {
            std::shared_ptr<vireo::RenderTarget> multisampledDepthAttachment;
        };

        const MaterialManager& materialManager;
        std::vector<FrameData> framesData;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;
    };
}