/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpass.forward_color;

import std;
import vireo;
import lysa.configuration;
import lysa.scene;
import lysa.resources.material;
import lysa.renderers.renderpass;

export namespace lysa {
    class ForwardColor : public Renderpass {
    public:
        ForwardColor(const RenderingConfiguration& config);

        void updatePipelines(
            const std::unordered_map<pipeline_id, std::vector<std::shared_ptr<Material>>>& pipelineIds);

        void render(
            vireo::CommandList& commandList,
            const Scene& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        auto getBrightnessBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].brightnessBuffer;
        }

    private:
        const std::wstring DEFAULT_VERTEX_SHADER{L"default.vert"};
        const std::wstring DEFAULT_FRAGMENT_SHADER{L"forward.frag"};
        const std::wstring DEFAULT_FRAGMENT_BLOOM_SHADER{L"forward_bloom.frag"};

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
            std::shared_ptr<vireo::RenderTarget> brightnessBuffer;
        };

        std::vector<FrameData> framesData;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;

    };
}