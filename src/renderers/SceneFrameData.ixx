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
import lysa.resources.light;
import lysa.resources.material;
import lysa.resources.manager;
import lysa.resources.mesh_instance;
import lysa.renderers.configuration;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.pipelines.frustum_culling;
import lysa.renderers.renderpasses.renderpass;

export namespace lysa {

    /**
     * Manages per-frame scene data for rendering.
     *
     * SceneFrameData handles the storage and update of scene-wide information,
     * including cameras, lights, environment settings, and mesh instance data.
     * It also manages descriptor sets and buffers required for rendering.
     */
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
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
        /** Optional descriptor layout (set used when transparency color is needed). */
        inline static std::shared_ptr<vireo::DescriptorLayout> sceneDescriptorLayoutOptional1{nullptr};
#endif

        /**
         * Creates all static descriptor layouts used by scenes and pipelines.
         */
        static void createDescriptorLayouts(const Context& ctx);
        /** Destroys static descriptor layouts created by createDescriptorLayouts(). */
        static void destroyDescriptorLayouts();

        /**
         * Defines the structure for the instance index push constant.
         */
        struct InstanceIndexConstant {
            /** Index of the instance to be fetched by the vertex shader via push constants. */
            uint32 instanceIndex;
        };

        /** Push constants description for the instance index. */
        static constexpr auto instanceIndexConstantDesc = vireo::PushConstantsDesc {
            .stage = vireo::ShaderStage::VERTEX,
            .size = sizeof(InstanceIndexConstant),
        };

        /**
         * Constructs a new SceneFrameData object.
         * 
         * @param ctx Reference to the global context.
         * @param maxLights Maximum number of lights supported.
         * @param maxMeshInstancesPerScene Maximum number of mesh instances per scene.
         * @param maxMeshSurfacePerPipeline Maximum number of mesh surfaces per pipeline.
         */
        SceneFrameData(
            const Context& ctx,
            uint32 maxLights,
            uint32 maxMeshInstancesPerScene,
            uint32 maxMeshSurfacePerPipeline);

        /**
         * Sets the scene's environment settings.
         * @param environment The environment to set (e.g., skybox, ambient lighting).
         */
        void setEnvironment(const Environment& environment) {
            this->environment = environment;
        }

        /**
         * Updates CPU/GPU scene state.
         * 
         * Synchronizes uniforms, lights, instances, and descriptors for the current frame.
         * 
         * @param commandList Command buffer for GPU operations.
         * @param camera The current camera.
         * @param config Renderer configuration.
         * @param frameIndex Index of the current frame.
         */
        void update(const vireo::CommandList& commandList, const Camera& camera, const RendererConfiguration& config, uint32 frameIndex);

        /**
         * Executes compute workloads.
         * 
         * Performs operations such as frustum culling.
         * 
         * @param commandList Command buffer for GPU operations.
         * @param camera The current camera.
         */
        void compute(vireo::CommandList& commandList, const Camera& camera) const;

        /**
         * Adds a mesh instance to the scene.
         * @param meshInstance Pointer to the mesh instance to add.
         */
        void addInstance(const MeshInstance* meshInstance);

        /**
         * Updates an existing mesh instance in the scene.
         * @param meshInstance Pointer to the mesh instance to update.
         */
        void updateInstance(const MeshInstance* meshInstance);

        /**
         * Removes a mesh instance from the scene.
         * @param meshInstance Pointer to the mesh instance to remove.
         */
        void removeInstance(const MeshInstance* meshInstance);

        /**
         * Adds a light to the scene.
         * @param light Pointer to the light to add.
         */
        void addLight(const Light* light);

        /**
         * Removes a light from the scene.
         * @param light Pointer to the light to remove.
         */
        void removeLight(const Light* light);

        /**
         * Issues draw calls for opaque models.
         * @param commandList Command buffer to record into.
         * @param pipelines   Map of material/pipeline identifiers to pipelines.
         */
        void drawOpaquesModels(
           vireo::CommandList& commandList,
           const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const;

        /**
         * Issues draw calls for transparent models.
         * @param commandList Command buffer to record into.
         * @param pipelines   Map of material/pipeline identifiers to pipelines.
         */
        void drawTransparentModels(
           vireo::CommandList& commandList,
           const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const;

        /**
         * Issues draw calls for models driven by shader materials/special passes.
         * @param commandList Command buffer to record into.
         * @param pipelines   Map of material/pipeline identifiers to pipelines.
         */
        void drawShaderMaterialModels(
           vireo::CommandList& commandList,
           const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const;

        /**
         * Issues multi-draw indirect calls for models.
         * 
         * @param commandList Command buffer to record into.
         * @param set Descriptor set index.
         * @param culledDrawCommandsBuffers Map of buffers containing indirect draw commands.
         * @param culledDrawCommandsCountBuffers Map of buffers containing indirect draw counts.
         * @param frustumCullingPipelines Map of frustum culling pipelines.
         */
        void drawModels(
           vireo::CommandList& commandList,
           uint32 set,
           const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsBuffers,
           const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsCountBuffers,
           const std::map<pipeline_id, std::shared_ptr<FrustumCulling>>& frustumCullingPipelines) const;

        /**
         * Returns the mapping of pipeline identifiers to their materials.
         * @return A reference to the pipeline to materials map.
         */
        const auto& getPipelineIds() const { return pipelineIds; }

        /**
         * Checks if materials have been updated.
         * @return True if materials were updated and pipelines/descriptors must be refreshed.
         */
        auto isMaterialsUpdated() const { return materialsUpdated; }

        /**
         * Resets the materials updated flag.
         */
        void resetMaterialsUpdated() { materialsUpdated = false; }

        /**
         * Returns the main descriptor set.
         * @return The main descriptor set containing scene resources.
         */
        auto getDescriptorSet() const { return descriptorSet; }

#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
        /**
         * Returns the optional descriptor set.
         * @return The optional descriptor set used for shadow map transparency color.
         */
        auto getDescriptorSetOptional1() const { return descriptorSetOpt1; }
#endif

        /**
         * Returns the shadow map renderers.
         * @return A view over the shadow map renderer values.
         */
        auto getShadowMapRenderers() const { return std::views::values(shadowMapRenderers); }

        SceneFrameData(SceneFrameData&) = delete;

        SceneFrameData& operator=(SceneFrameData&) = delete;

    private:
        /*Reference to the engine context. */
        const Context& ctx;
        /* Reference to the material manager. */
        MaterialManager& materialManager;
        /* Maximum number of supported lights. */
        const uint32 maxLights;
        /* Maximum number of mesh surfaces per pipeline. */
        const uint32 maxMeshSurfacePerPipeline;
        /* Main descriptor set for scene bindings. */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
        /* Optional descriptor set for special passes. */
        std::shared_ptr<vireo::DescriptorSet> descriptorSetOpt1;
#endif
        /* Uniform buffer containing SceneData. */
        std::shared_ptr<vireo::Buffer> sceneUniformBuffer;
        /* Current environment settings. */
        Environment environment;
        /* Map of lights to their shadow-map render passes. */
        std::map<const Light*, std::shared_ptr<Renderpass>> shadowMapRenderers;
        /* Array of shadow map images. */
        std::vector<std::shared_ptr<vireo::Image>> shadowMaps;
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
        /* Array of transparency-color shadow maps. */
        std::vector<std::shared_ptr<vireo::Image>> shadowTransparencyColorMaps;
#endif
        /* Associates each light with a shadow map index. */
        std::map<const Light*, uint32> shadowMapIndex;
        /* Lights scheduled for removal. */
        std::unordered_set<const Light*> removedLights;
        /* Flag set if shadow maps have changed. */
        bool shadowMapsUpdated{false};

        /* Device array for per-mesh-instance data. */
        DeviceMemoryArray meshInstancesDataArray;
        /* Memory blocks in meshInstancesDataArray per mesh instance. */
        std::unordered_map<const MeshInstance*, MemoryBlock> meshInstancesDataMemoryBlocks{};
        /* Flag set if mesh instance data changed. */
        bool meshInstancesDataUpdated{false};

        /* Mapping of pipeline id to its materials. */
        std::unordered_map<pipeline_id, std::vector<unique_id>> pipelineIds;
        /* Flag set when the materials list changes. */
        bool materialsUpdated{false};

        /* List of active lights. */
        std::unordered_set<const Light*> lights;
        /* GPU buffer for packed light parameters. */
        std::shared_ptr<vireo::Buffer> lightsBuffer;
        /* Number of allocated light slots in lightsBuffer. */
        uint32 lightsBufferCount{1};

        /* Recycle bin for indirect draw commands staging buffers. */
        std::unordered_set<std::shared_ptr<vireo::Buffer>> drawCommandsStagingBufferRecycleBin;

        /* Opaque pipelines data. */
        std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>> opaquePipelinesData;
        /* Shader material pipelines data. */
        std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>> shaderMaterialPipelinesData;
        /* Transparent pipelines data. */
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
            const MeshInstance*& meshInstance,
            std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData);

        void drawModels(
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        void enableLightShadowCasting(const Light* light);

        void disableLightShadowCasting(const Light* light);
    };

}

