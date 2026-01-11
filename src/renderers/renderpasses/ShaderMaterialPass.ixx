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

    class ShaderMaterialPass : public Renderpass {
    public:
        ShaderMaterialPass(
            const Context& ctx,
            const RendererConfiguration& config);

        void updatePipelines(
            const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

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