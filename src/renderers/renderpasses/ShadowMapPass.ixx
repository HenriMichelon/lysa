/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <cstdlib>
export module lysa.renderers.renderpasses.shadow_map_pass;

import vireo;
import lysa.context;
import lysa.math;
import lysa.memory;
import lysa.resources.camera;
import lysa.resources.light;
import lysa.renderers.configuration;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.scene_frame_data;
import lysa.renderers.pipelines.frustum_culling;
import lysa.renderers.renderpasses.renderpass;

export namespace lysa {

    class ShadowMapPass : public Renderpass {
    public:
        ShadowMapPass(
            const Context& ctx,
            const Light* light,
            const DeviceMemoryArray& meshInstancesDataArray,
            size_t maxMeshSurfacePerPipeline);

        void compute(
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        void setCurrentCamera(const Camera& camera) {
            currentCamera = const_cast<Camera*>(&camera);
        }

        void update(uint32 frameIndex) override;

        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene);

        auto getShadowMapCount() const { return subpassesCount; }

        auto getShadowMap(const uint32 index) const {
            return subpassData[index].shadowMap;
        }

        auto getTransparencyColorMap(const uint32 index) const {
            return subpassData[index].transparencyColorMap;
        }

        const auto& getLightSpace(const uint32 index) const {
            return subpassData[index].globalUniform.lightSpace;
        }

        auto getCascadeSplitDepth(const uint32 index) const {
            return subpassData[index].globalUniform.splitDepth;
        }

    private:
        const std::string VERTEX_SHADER{"shadowmap.vert"};
        const std::string FRAGMENT_SHADER{"shadowmap.frag"};
        const std::string FRAGMENT_SHADER_CUBEMAP{"shadowmap_cubemap.frag"};

        static constexpr uint32 SET_RESOURCES{0};
        static constexpr uint32 SET_SCENE{1};
        static constexpr uint32 SET_PIPELINE{2};
        static constexpr uint32 SET_PASS{3};
        static constexpr uint32 SET_SAMPLERS{4};
        static constexpr vireo::DescriptorIndex BINDING_GLOBAL{0};

        struct GlobalUniform {
            float4x4 lightSpace;
            float4   lightPosition; // XYZ: Position, W: far plane
            float    transparencyScissor;
            float    transparencyColorScissor;
            float    splitDepth;
        };

        struct SubpassData {
            float4x4 inverseViewMatrix;
            float4x4 projection;
            GlobalUniform globalUniform;
            std::shared_ptr<vireo::RenderTarget> shadowMap;
            std::shared_ptr<vireo::RenderTarget> transparencyColorMap;
            std::shared_ptr<vireo::Buffer> globalUniformBuffer;
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
            std::map<pipeline_id, std::shared_ptr<FrustumCulling>> frustumCullingPipelines;
            std::map<pipeline_id, std::shared_ptr<vireo::Buffer>> culledDrawCommandsBuffers;
            std::map<pipeline_id, std::shared_ptr<vireo::Buffer>> culledDrawCommandsCountBuffers;
        };

        const bool isCubeMap;
        const bool isCascaded;
        uint32 subpassesCount;
        Camera* currentCamera{nullptr};
        float3 lastLightPosition{-10000.0f};
        std::vector<SubpassData> subpassData;

        const std::vector<vireo::VertexAttributeDesc> vertexAttributes {
            {"POSITION", vireo::AttributeFormat::R32G32B32A32_FLOAT, offsetof(VertexData, position)},
            {"NORMAL", vireo::AttributeFormat::R32G32B32A32_FLOAT, offsetof(VertexData, normal)},
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorRenderFormats = { vireo::ImageFormat::R8G8B8A8_SNORM }, // Packed RGB + alpha
            .colorBlendDesc = {{}},
            .depthStencilImageFormat = vireo::ImageFormat::D32_SFLOAT,
            .depthTestEnable = true,
            .depthWriteEnable = true,
            .depthBiasEnable = true,
            .depthBiasConstantFactor = 1.25f,
            .depthBiasSlopeFactor = 1.75f,
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {
                {
                    .clear = true,
                    .clearValue{ .color = {0.0f, 0.0f, 0.0f, 1.0f} }
                }},
            .depthTestEnable = pipelineConfig.depthTestEnable,
            .clearDepthStencil = true,
            .discardDepthStencilAfterRender = false,
        };

        bool firstPass{true};

        const size_t maxMeshSurfacePerPipeline;
        const DeviceMemoryArray& meshInstancesDataArray;

        vireo::Rect scissors;
        vireo::Viewport viewport;
        const Light* light;
        std::shared_ptr<vireo::GraphicPipeline> pipeline;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
    };
}