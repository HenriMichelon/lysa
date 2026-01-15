/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.shader_material_pass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.renderpass;
import lysa.resources.material;

export namespace lysa {

    /**
     * Render pass for shader-based material rendering
     */
    class ShaderMaterialPass : public Renderpass {
    public:
        /**
         * Constructs a ShaderMaterialPass
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        ShaderMaterialPass(
            const Context& ctx,
            const RendererConfiguration& config);

        /**
         * Updates the graphics pipelines based on active pipeline IDs
         * @param pipelineIds Map of pipeline IDs to unique object IDs
         */
        void updatePipelines(
            const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /**
         * Renders the shader material pass
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

    private:
        const std::string DEFAULT_VERTEX_SHADER{"default.vert"};
        const std::string DEFAULT_FRAGMENT_SHADER{"forward.frag"};

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = { { } },
            .depthTestEnable = true,
            .depthWriteEnable = true,
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{ }},
            .depthTestEnable = pipelineConfig.depthTestEnable,
        };

        const MaterialManager& materialManager;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;

    };
}