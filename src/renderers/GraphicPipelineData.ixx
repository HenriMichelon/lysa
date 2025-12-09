/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.graphic_pipeline_data;

import vireo;

import lysa.aabb;
import lysa.context;
import lysa.math;
import lysa.memory;
import lysa.resources.material;
import lysa.resources.mesh;
import lysa.pipelines.frustum_culling;
import lysa.renderers.configuration;

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
    * Struct to pass camera data from the ECS/POO systems to the rendering system
    */
    struct CameraDesc {
        /** World space position */
        float3 position;
        /** World space transform */
        float4x4 transform;
        /** View projection */
        float4x4 projection;
    };

    struct VertexData {
        float4 position; // position + uv.x
        float4 normal;   // normal + uv.y
        float4 tangent;  // tangent + sign

        static const std::vector<vireo::VertexAttributeDesc> vertexAttributes;
    };
    /**
     * Light data in GPU memory
     */
    struct LightData {
        // light params
        int32 type{0}; // Light::LightType
        float range{0.0f};
        float cutOff{0.0f};
        float outerCutOff{0.0f};
        float4 position{0.0f};
        float4 direction{0.0f};
        float4 color{1.0f, 1.0f, 1.0f, 1.0f}; // RGB + Intensity;
        // shadow map params
        int32 mapIndex{-1};
        uint32 cascadesCount{0};
        float4 cascadeSplitDepth{0.0f};
        float4x4 lightSpace[6];
    };

    /**
    * Struct to pass light data from the ECS/POO systems to the rendering system
    */
    struct LightDesc {
        int32 type; // Light::LightType
        bool visible;
        float3 position;
        float4 colorAndIntensity;
        LightData getData() const;
    };

    /**
    * Struct to pass environment data from the ECS/POO systems to the rendering system
    */
    struct EnvironmentDesc {
        float4 ambientColorIntensity;
    };

    /**
     * Mesh instance data in GPU memory
     */
    struct MeshInstanceData {
        float4x4 transform;
        float3   aabbMin;
        float3   aabbMax;
        uint     visible;
        uint     castShadows;
    };

    /**
     * Structure to pass data between the ECS/OOP systems to the rendering system
     */
    struct MeshInstanceDesc {
        Mesh& mesh;
        bool visible;
        bool castShadows;
        AABB worldAABB;
        float4x4 worldTransform;
        std::unordered_map<uint32, unique_id> materialsOverride;
        /** Current number of pending updates to process. */
        uint32 pendingUpdates{0};
        /** Upper bound on pendingUpdates; */
        uint32 maxUpdates{0};

        unique_id getSurfaceMaterial(uint32 surfaceIndex) const;

        MeshInstanceData getData() const;
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
     * A single indirect draw coupled with the instance index it belongs to.
     */
    struct DrawCommand {
        /** InstanceData index associated to this draw. */
        uint32 instanceIndex;
        /** Standard indexed indirect draw command parameters. */
        vireo::DrawIndexedIndirectCommand command;
    };

    /**
     * Per-pipeline data for instances, draw command arrays and culling.
     *
     * Stores instance data, staging/copy buffers and the compute pipeline
     * used to perform frustum culling and produce culled indirect draws.
     */
    struct GraphicPipelineData {
        /** Descriptor binding for per-instance buffer used by pipelines. */
        static constexpr vireo::DescriptorIndex BINDING_INSTANCES{0};
        /** Shared descriptor layout for pipeline-local resources. */
        inline static std::shared_ptr<vireo::DescriptorLayout> pipelineDescriptorLayout{nullptr};
        /** Create the shared descriptor layout */
        static void createDescriptorLayouts(const std::shared_ptr<vireo::Vireo>& vireo);
        /** Destroy the shared descriptor layout */
        static void destroyDescriptorLayouts();

        /** Identifier of the material/pipeline family. */
        pipeline_id pipelineId;
        /** Descriptor set bound when drawing with this pipeline. */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** Compute pipeline used to cull draw commands against the frustum. */
        FrustumCulling frustumCullingPipeline;
        /** Reference to the material manager */
        MaterialManager& materialManager;
        /** Reference to Vireo */
        std::shared_ptr<vireo::Vireo> vireo;

        /** Flags tracking mutations in the instances set. */
        bool instancesUpdated{false};
        bool instancesRemoved{false};
        /** Device memory array that stores InstanceData blocks. */
        DeviceMemoryArray instancesArray;
        /** Mapping of mesh instance to its memory block within instancesArray. */
        std::unordered_map<std::shared_ptr<MeshInstanceDesc>, MemoryBlock> instancesMemoryBlocks;

        /** Number of indirect draw commands before culling. */
        uint32 drawCommandsCount{0};
        /** CPU-side list of draw commands to upload. */
        std::vector<DrawCommand> drawCommands;
        /** GPU buffer storing indirect draw commands. */
        std::shared_ptr<vireo::Buffer> drawCommandsBuffer;
        /** GPU buffer storing the count of culled draw commands. */
        std::shared_ptr<vireo::Buffer> culledDrawCommandsCountBuffer;
        /** GPU buffer storing culled indirect draw commands. */
        std::shared_ptr<vireo::Buffer> culledDrawCommandsBuffer;

        /** Staging buffer used to copy draw commands to the GPU. */
        std::shared_ptr<vireo::Buffer> drawCommandsStagingBuffer;
        /** Number of draw commands stored in the current staging buffer. */
        uint32 drawCommandsStagingBufferCount{0};

        /**
         * Create a pipeline data object for a specific material/pipeline ID
         */
        GraphicPipelineData::GraphicPipelineData(
            const Context& ctx,
            uint32 pipelineId,
            const DeviceMemoryArray& meshInstancesDataArray,
            uint32 maxMeshSurfacePerPipeline);

        /** Registers a mesh instance into this pipeline data object. */
        void addInstance(
            const std::shared_ptr<MeshInstanceDesc>& meshInstance,
            const std::unordered_map<std::shared_ptr<MeshInstanceDesc>, MemoryBlock>& meshInstancesDataMemoryBlocks);

        /** Removes a previously registered mesh instance. */
        void removeInstance(
            const std::shared_ptr<MeshInstanceDesc>& meshInstance);

        /** Adds a single draw instance and wires memory blocks. */
        void addInstance(
            const std::shared_ptr<MeshInstanceDesc>& meshInstance,
            const MemoryBlock& instanceMemoryBlock,
            const MemoryBlock& meshInstanceMemoryBlock);

        /** Uploads/refreshes GPU buffers and prepares culled draw arrays. */
        void updateData(
            const vireo::CommandList& commandList,
            std::unordered_set<std::shared_ptr<vireo::Buffer>>& drawCommandsStagingBufferRecycleBin,
            const std::unordered_map<std::shared_ptr<MeshInstanceDesc>, MemoryBlock>& meshInstancesDataMemoryBlocks);
    };

}
