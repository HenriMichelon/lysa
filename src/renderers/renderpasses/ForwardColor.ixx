/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.forward_color;

import vireo;
import lysa.context;
import lysa.types;
import lysa.renderers.configuration;
import lysa.renderers.scene_render_context;
import lysa.renderers.renderpasses.renderpass;
import lysa.resources.material;

export namespace lysa {

    class ForwardColor : public Renderpass {
    public:
        ForwardColor(
            const Context& ctx,
            const RendererConfiguration& config,
            uint32 framesInFlight);

        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        void render(
            vireo::CommandList& commandList,
            const SceneRenderContext& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

    private:
        const std::string DEFAULT_VERTEX_SHADER{"default.vert"};
        const std::string DEFAULT_FRAGMENT_SHADER{"forward.frag"};

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = { {}},
            .depthTestEnable = true,
            .depthWriteEnable = true,
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{} },
            .depthTestEnable = pipelineConfig.depthTestEnable,
        };

        struct FrameData {
        };

        const MaterialManager& materialManager;
        std::vector<FrameData> framesData;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;

    };
}