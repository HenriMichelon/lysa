/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.scene_frame_data;

import vireo;
import lysa.context;
import lysa.math;
import lysa.memory;
import lysa.resources.camera;
import lysa.resources.environment;
import lysa.resources.material;
import lysa.resources.manager;
import lysa.resources.mesh_instance;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.pipelines.frustum_culling;

export namespace lysa {

    class SceneFrameData {
    public:
         /** Descriptor binding for SceneData uniform buffer. */
        static constexpr vireo::DescriptorIndex BINDING_SCENE{0};
        /** Descriptor binding for per-model/instance data buffer. */
        static constexpr vireo::DescriptorIndex BINDING_MODELS{1};
        /** Descriptor binding for lights buffer. */
        static constexpr vireo::DescriptorIndex BINDING_LIGHTS{2};
        /** Descriptor binding for shadow maps array. */
        static constexpr vireo::DescriptorIndex BINDING_SHADOW_MAPS{3};
        /** Shared descriptor layout for the main scene set. */
        inline static std::shared_ptr<vireo::DescriptorLayout> sceneDescriptorLayout{nullptr};

        /** Optional descriptor binding: transparency color for shadow maps. */
        static constexpr vireo::DescriptorIndex BINDING_SHADOW_MAP_TRANSPARENCY_COLOR{0};
        /** Optional descriptor layout (set used when transparency color is needed). */
        inline static std::shared_ptr<vireo::DescriptorLayout> sceneDescriptorLayoutOptional1{nullptr};

        /** Creates all static descriptor layouts used by scenes and pipelines. */
        static void createDescriptorLayouts(
            const std::shared_ptr<vireo::Vireo>& vireo,
            uint32 maxShadowMaps);
        /** Destroys static descriptor layouts created by createDescriptorLayouts(). */
        static void destroyDescriptorLayouts();

        struct InstanceIndexConstant {
            /** Index of the instance to be fetched by the vertex shader via push constants. */
            uint32 instanceIndex;
        };

        static constexpr auto instanceIndexConstantDesc = vireo::PushConstantsDesc {
            .stage = vireo::ShaderStage::VERTEX,
            .size = sizeof(InstanceIndexConstant),
        };

        SceneFrameData(
            const Context& ctx,
            uint32 maxLights,
            uint32 maxMeshInstancesPerScene,
            uint32 maxMeshSurfacePerPipeline,
            uint32 framesInFlight,
            uint32 maxShadowMaps);

        void setEnvironment(const Environment& environment) {
            this->environment = environment;
        }

        /** Set initial dynamic states */
        void prepare(
            const vireo::CommandList& commandList,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissors) const;

        /** Updates CPU/GPU scene state (uniforms, lights, instances, descriptors). */
        void update(const vireo::CommandList& commandList, const Camera& camera);

        /** Executes compute workloads such as frustum culling. */
        void compute(vireo::CommandList& commandList, const Camera& camera) const;

        /** Adds a mesh instance to the scene. */
        void addInstance(unique_id meshInstance);

        /** Adds a mesh instance to the scene. */
        void updateInstance(unique_id meshInstance);

        /** Removes a node previously added to the scene. */
        void removeInstance(unique_id node);

        /**
         * Issues draw calls for opaque models using the supplied pipelines map.
         * @param commandList Command buffer to record into.
         * @param pipelines   Map of material/pipeline identifiers to pipelines.
         */
        void drawOpaquesModels(
           vireo::CommandList& commandList,
           const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const;

        /** Issues draw calls for transparent models. */
        void drawTransparentModels(
           vireo::CommandList& commandList,
           const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const;

        /** Issues draw calls for models driven by shader materials/special passes. */
        void drawShaderMaterialModels(
           vireo::CommandList& commandList,
           const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const;

        void drawModels(
           vireo::CommandList& commandList,
           uint32 set,
           const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsBuffers,
           const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsCountBuffers,
           const std::map<pipeline_id, std::shared_ptr<FrustumCulling>>& frustumCullingPipelines) const;

        /** Returns the mapping of pipeline identifiers to their materials. */
        const auto& getPipelineIds() const { return pipelineIds; }

        /** True when materials set changed and pipelines/descriptors must be refreshed. */
        auto isMaterialsUpdated() const { return materialsUpdated; }

        /** Resets the materials updated flag after processing. */
        void resetMaterialsUpdated() { materialsUpdated = false; }

        /** Returns the main descriptor set containing scene resources. */
        auto getDescriptorSet() const { return descriptorSet; }

        /** Returns the optional descriptor set (transparency color for shadows). */
        auto getDescriptorSetOptional1() const { return descriptorSetOpt1; }

        /** Returns a view over the shadow map renderers values. */
        // auto getShadowMapRenderers() const { return std::views::values(shadowMapRenderers); }

        SceneFrameData(SceneFrameData&) = delete;
        SceneFrameData& operator=(SceneFrameData&) = delete;

    private:
        const Context& ctx;
        MaterialManager& materialManager;
        MeshInstanceManager& meshInstanceManager;
        const uint32 maxLights;
        const uint32 maxMeshSurfacePerPipeline;
        /** Number of frames processed in-flight. */
        const uint32 framesInFlight;
        /** Main descriptor set for scene bindings (scene, models, lights, textures). */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** Optional descriptor set for special passes (e.g., transparency). */
        std::shared_ptr<vireo::DescriptorSet> descriptorSetOpt1;
        /** Uniform buffer containing SceneData. */
        std::shared_ptr<vireo::Buffer> sceneUniformBuffer;
        /** Current environment settings (skybox, etc.). */
        Environment environment;
        /** Map of lights to their shadow-map renderpasses. */
        // std::map<std::shared_ptr<Light>, std::shared_ptr<Renderpass>> shadowMapRenderers;
        /** Array of shadow map images. */
        std::vector<std::shared_ptr<vireo::Image>> shadowMaps;
        /** Array of transparency-color shadow maps (optional). */
        std::vector<std::shared_ptr<vireo::Image>> shadowTransparencyColorMaps;
        /** Associates each light with a shadow map index. */
        // std::map<std::shared_ptr<LightData>, uint32> shadowMapIndex;
        /** Lights scheduled for removal (deferred to safe points). */
        std::list<std::shared_ptr<LightDesc>> removedLights;
        /** True if the set of shadow maps has changed and descriptors must be updated. */
        bool shadowMapsUpdated{false};

        /** Device array storing per-mesh-instance GPU data. */
        DeviceMemoryArray meshInstancesDataArray;
        /** Memory blocks allocated in meshInstancesDataArray per MeshInstance. */
        std::unordered_map<unique_id, MemoryBlock> meshInstancesDataMemoryBlocks{};
        /** True if meshInstancesDataArray content changed. */
        bool meshInstancesDataUpdated{false};

        /** Mapping of pipeline id to materials used by that pipeline. */
        std::unordered_map<pipeline_id, std::vector<unique_id>> pipelineIds;
        /** Flag set when materials list changes. */
        bool materialsUpdated{false};

        /** Active lights list. */
        std::list<std::shared_ptr<LightDesc>> lights;
        /** GPU buffer with packed light parameters. */
        std::shared_ptr<vireo::Buffer> lightsBuffer;
        /** Number of allocated light slots in lightsBuffer. */
        uint32 lightsBufferCount{1};

        /** Recycle bin for staging buffers used to stream indirect draw commands. */
        std::unordered_set<std::shared_ptr<vireo::Buffer>> drawCommandsStagingBufferRecycleBin;

        std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>> opaquePipelinesData;
        std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>> shaderMaterialPipelinesData;
        std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>> transparentPipelinesData;

        void updatePipelinesData(
            const vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData);

        void compute(
            const Camera& camera,
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        void addInstance(
            pipeline_id pipelineId,
            unique_id meshInstance,
            std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData);

        void drawModels(
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        // void enableLightShadowCasting(const std::shared_ptr<Node>&node);

        // void disableLightShadowCasting(const std::shared_ptr<Light>&light);
    };

}

