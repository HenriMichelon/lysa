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

    class ForwardColorPass : public Renderpass {
    public:
        ForwardColorPass(
            const Context& ctx,
            const RendererConfiguration& config);

        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            const std::shared_ptr<vireo::RenderTarget>& multisampledDepthAttachment,
            bool clearAttachment,
            uint32 frameIndex);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

    private:
        const std::string DEFAULT_VERTEX_SHADER{"default.vert"};
        const std::string DEFAULT_FRAGMENT_SHADER{"forward.frag"};

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
        };

        const MaterialManager& materialManager;
        std::vector<FrameData> framesData;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;

    };
}