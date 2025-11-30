/*
 * Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */
export module lysa.renderers.scene_render_context;

import vireo;
import lysa.context;
import lysa.math;
import lysa.memory;
import lysa.types;
import lysa.pipelines.frustum_culling;
import lysa.renderers.configuration;
import lysa.renderers.graphic_pipeline_data;

export namespace lysa {

    /**
     * Per-frame scene uniform payload consumed by shaders.
     *
     * Contains camera transforms, ambient lighting, and feature toggles. The
     * layout/alignas qualifiers are chosen to satisfy typical std140/std430
     * alignment constraints on most backends.
     */
    struct SceneData {
        /** World-space camera position in XYZ; W is unused. */
        float3      cameraPosition;
        /** Projection matrix used for rendering (clip-from-view). */
        alignas(16) float4x4 projection;
        /** View matrix (view-from-world). */
        float4x4    view;
        /** Inverse of the view matrix (world-from-view). */
        float4x4    viewInverse;
        /** Ambient light RGB color in xyz and strength in w. */
        float4      ambientLight{1.0f, 1.0f, 1.0f, 1.0f}; // RGB + strength
        /** Number of active lights currently bound. */
        uint32      lightsCount{0};
        /** Toggle for bloom post-process (1 enabled, 0 disabled). */
        uint32      bloomEnabled{0};
        /** Toggle for SSAO post-process (1 enabled, 0 disabled). */
        uint32      ssaoEnabled{0};
    };

    /**
     * %A single draw instance.
     *
     * Indices reference engine-wide arrays for mesh instances, mesh surfaces
     * and materials, allowing shaders to fetch additional data.
     */
    struct alignas(8) InstanceData {
        /** Index of the MeshInstance in the global instances array. */
        uint32 meshInstanceIndex;
        /** Index of the mesh surface within the global surfaces array. */
        uint32 meshSurfaceIndex;
        /** Index of the material used by this instance. */
        uint32 materialIndex;
        /** Index of the mesh-surface-local material slot. */
        uint32 meshSurfaceMaterialIndex;
    };


    /**
     * %Scene orchestrator.
     *  - Track nodes added to the scene (cameras, lights, mesh instances, environment).
     *  - Own GPU-side resources necessary for scene rendering (uniform buffers, descriptor sets).
     *  - Build and maintain per-pipeline instance data and indirect draw commands.
     *  - Perform frustum culling via compute pipelines and submit culled draws.
     *  - Manage shadow-map renderers and their images.
     *
     * Thread-safety: unless stated otherwise, methods are intended to be called
     * only from the render thread.
     */
    class SceneRenderContext {
    public:
        /** Maximum number of shadow maps supported by the scene. */
        static constexpr uint32 MAX_SHADOW_MAPS{20};

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

        /** Descriptor binding for per-instance buffer used by pipelines. */
        static constexpr vireo::DescriptorIndex BINDING_INSTANCES{0};
        /** Shared descriptor layout for pipeline-local resources. */
        inline static std::shared_ptr<vireo::DescriptorLayout> pipelineDescriptorLayout{nullptr};

        /** Creates all static descriptor layouts used by scenes and pipelines. */
        static void createDescriptorLayouts(const std::shared_ptr<vireo::Vireo>& vireo);
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

        /**
         * Constructs a Scene for a given configuration and viewport/scissors.
         *
         * @param config             Scene high-level configuration (buffers sizes, features).
         * @param renderingConfig    Global rendering configuration.
         * @param framesInFlight     Number of buffered frames.
         * @param viewport           Default viewport used for render passes.
         * @param scissors           Default scissors rectangle.
         */
        SceneRenderContext(
            const Context& ctx,
            const SceneRenderContextConfiguration& config,
            const RendererConfiguration& renderingConfig,
            uint32 framesInFlight,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissors);

        /** Returns the default viewport for this scene. */
        auto getViewport() const { return viewport; }

        /** Returns the default scissors rectangle for this scene. */
        auto getScissors() const { return scissors; }

        /** Adds a node (camera, light, mesh instance, environment, etc.) to the scene. */
        //virtual void addNode(const std::shared_ptr<Node> &node);

        /** Removes a node previously added to the scene. */
        //virtual void removeNode(const std::shared_ptr<Node> &node);

        /** Updates CPU/GPU scene state (uniforms, lights, instances, descriptors). */
        void update(const vireo::CommandList& commandList);

        /** Executes compute workloads such as frustum culling. */
        void compute(vireo::CommandList& commandList) const;

        /** Writes initial GPU state required before issuing draw calls. */
        void setInitialState(const vireo::CommandList& commandList) const;

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

        /**
         * Common draw helper that dispatches indirect draws per pipeline using culled lists.
         * @param commandList Command buffer to record into.
         * @param set         Descriptor set index to bind for the pipelines family.
         * @param culledDrawCommandsBuffers Per-pipeline indirect commands buffer.
         * @param culledDrawCommandsCountBuffers Per-pipeline commands count buffer.
         * @param frustumCullingPipelines Per-pipeline frustum culling compute pipelines.
         */
        void drawModels(
           vireo::CommandList& commandList,
           uint32 set,
           const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsBuffers,
           const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsCountBuffers,
           const std::map<pipeline_id, std::shared_ptr<FrustumCulling>>& frustumCullingPipelines) const;

        /** Returns the mapping of pipeline identifiers to their materials. */
        // const auto& getPipelineIds() const { return pipelineIds; }

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

        virtual ~SceneRenderContext() = default;
        SceneRenderContext(SceneRenderContext&) = delete;
        SceneRenderContext& operator=(SceneRenderContext&) = delete;

    private:
        const Context& ctx;
        /** Read-only reference to scene configuration (sizes, toggles). */
        const SceneRenderContextConfiguration& config;
        /** Read-only reference to global rendering configuration. */
        const RendererConfiguration& renderingConfig;
        /** Number of frames processed in-flight. */
        const uint32 framesInFlight;
        /** Default viewport for render passes. */
        const vireo::Viewport& viewport;
        /** Default scissors rectangle for render passes. */
        const vireo::Rect& scissors;
        /** Main descriptor set for scene bindings (scene, models, lights, textures). */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** Optional descriptor set for special passes (e.g., transparency). */
        std::shared_ptr<vireo::DescriptorSet> descriptorSetOpt1;
        /** Uniform buffer containing SceneData. */
        std::shared_ptr<vireo::Buffer> sceneUniformBuffer;
        /** Current environment settings (skybox, etc.). */
        //std::shared_ptr<Environment> currentEnvironment{};
        /** Map of lights to their shadow-map renderpasses. */
        // std::map<std::shared_ptr<Light>, std::shared_ptr<Renderpass>> shadowMapRenderers;
        /** Array of shadow map images. */
        std::vector<std::shared_ptr<vireo::Image>> shadowMaps;
        /** Array of transparency-color shadow maps (optional). */
        std::vector<std::shared_ptr<vireo::Image>> shadowTransparencyColorMaps;
        /** Associates each light with a shadow map index. */
        // std::map<std::shared_ptr<Light>, uint32> shadowMapIndex;
        /** Lights scheduled for removal (deferred to safe points). */
        // std::list<std::shared_ptr<Light>> removedLights;
        /** True if the set of shadow maps has changed and descriptors must be updated. */
        bool shadowMapsUpdated{false};

        /** Device array storing per-mesh-instance GPU data. */
        DeviceMemoryArray meshInstancesDataArray;
        /** Memory blocks allocated in meshInstancesDataArray per MeshInstance. */
        // std::unordered_map<std::shared_ptr<MeshInstance>, MemoryBlock> meshInstancesDataMemoryBlocks{};
        /** Mesh instances scheduled for removal. */
        // std::list<std::shared_ptr<MeshInstance>> removedMeshInstances{};
        /** True if meshInstancesDataArray content changed. */
        bool meshInstancesDataUpdated{false};

        /** Mapping of pipeline id to materials used by that pipeline. */
        // std::unordered_map<pipeline_id, std::vector<std::shared_ptr<Material>>> pipelineIds;
        /** Flag set when materials list changes. */
        bool materialsUpdated{false};

        /** Active lights list. */
        // std::list<std::shared_ptr<Light>> lights;
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
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        // void addNode(
            // pipeline_id pipelineId,
            // const std::shared_ptr<MeshInstance>& meshInstance,
            // std::unordered_map<uint32, std::unique_ptr<PipelineData>>& pipelinesData);

        void drawModels(
            vireo::CommandList& commandList,
            const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines,
            const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const;

        // void enableLightShadowCasting(const std::shared_ptr<Node>&node);

        // void disableLightShadowCasting(const std::shared_ptr<Light>&light);

    };

}
