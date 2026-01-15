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

    /**
     * Render pass for generating shadow maps
     */
    class ShadowMapPass : public Renderpass {
    public:
        /**
         * Constructs a ShadowMapPass
         * @param ctx The engine context
         * @param light Pointer to the light source for which shadows are generated
         * @param meshInstancesDataArray Array of mesh instance data in device memory
         * @param maxMeshSurfacePerPipeline Maximum number of mesh surfaces per pipeline
         */
        ShadowMapPass(
            const Context& ctx,
            const Light* light,
            const DeviceMemoryArray& meshInstancesDataArray,
            size_t maxMeshSurfacePerPipeline);

        /**
         * Computes frustum culling for shadow map generation
         * @param commandList The command list to record compute commands into
         * @param pipelinesData Map of pipeline data for mesh rendering
         */
        void compute(
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        /**
         * Updates the graphics pipelines based on active pipeline IDs
         * @param pipelineIds Map of pipeline IDs to unique object IDs
         */
        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /**
         * Sets the current camera for cascaded shadow maps calculation
         * @param camera Reference to the current camera
         */
        void setCurrentCamera(const Camera& camera) {
            currentCamera = const_cast<Camera*>(&camera);
        }

        /**
         * Updates the shadow map pass state for the current frame
         * @param frameIndex Index of the current frame
         */
        void update(uint32 frameIndex) override;

        /**
         * Renders the shadow maps
         * @param commandList The command list to record rendering commands into
         * @param scene The scene frame data
         */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene);

        /**
         * Gets the number of shadow maps (subpasses)
         * @return The number of shadow maps
         */
        auto getShadowMapCount() const { return subpassesCount; }

        /**
         * Gets a shadow map render target
         * @param index Index of the shadow map
         * @return A shared pointer to the shadow map render target
         */
        auto getShadowMap(const uint32 index) const {
            return subpassData[index].shadowMap;
        }

        /**
         * Gets a transparency color map render target
         * @param index Index of the shadow map
         * @return A shared pointer to the transparency color map render target
         */
        auto getTransparencyColorMap(const uint32 index) const {
            return subpassData[index].transparencyColorMap;
        }

        /**
         * Gets the light space matrix for a shadow map
         * @param index Index of the shadow map
         * @return The light space matrix
         */
        const auto& getLightSpace(const uint32 index) const {
            return subpassData[index].globalUniform.lightSpace;
        }

        /**
         * Gets the cascade split depth for a shadow map
         * @param index Index of the shadow map
         * @return The split depth
         */
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
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
            .colorRenderFormats = { vireo::ImageFormat::R8G8B8A8_SNORM }, // Packed RGB + alpha
            .colorBlendDesc = {{}},
#endif
            .depthStencilImageFormat = vireo::ImageFormat::D32_SFLOAT,
            .depthTestEnable = true,
            .depthWriteEnable = true,
            .depthBiasEnable = true,
            .depthBiasConstantFactor = 1.25f,
            .depthBiasSlopeFactor = 1.75f,
        };

        vireo::RenderingConfiguration renderingConfig {
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
            .colorRenderTargets = {
                {
                    .clear = true,
                    .clearValue{ .color = {0.0f, 0.0f, 0.0f, 1.0f} }
                }},
#endif
            .depthTestEnable = pipelineConfig.depthTestEnable,
            .clearDepthStencil = true,
            .discardDepthStencilAfterRender = false,
        };

        bool firstPass{true};

        const size_t maxMeshSurfacePerPipeline;
        const DeviceMemoryArray& meshInstancesDataArray;

        const Light* light;
        std::shared_ptr<vireo::GraphicPipeline> pipeline;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
    };
}