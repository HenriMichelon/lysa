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
import lysa.resources.mesh_instance;
import lysa.renderers.configuration;
import lysa.renderers.pipelines.frustum_culling;

export namespace lysa {

    /**
     * event.Per-frame scene uniform payload consumed by shaders.
     *
     */
    struct SceneData {
        /** event.World-space camera position in XYZ; W is unused. */
        float3      cameraPosition;
        /** event.Projection matrix used for rendering (clip-from-view). */
        alignas(16) float4x4 projection;
        /** event.View matrix (view-from-world). */
        float4x4    view;
        /** event.Inverse of the view matrix (world-from-view). */
        float4x4    viewInverse;
        /** event.Ambient light RGB color in xyz and strength in w. */
        float4      ambientLight{1.0f, 1.0f, 1.0f, 0.0f}; // RGB + strength
        /** event.Number of active lights currently bound. */
        uint32      lightsCount{0};
        /** event.Toggle for bloom post-process (1 enabled, 0 disabled). */
        uint32      bloomEnabled{0};
        /** event.Toggle for SSAO post-process (1 enabled, 0 disabled). */
        uint32      ssaoEnabled{0};
    };

    /**
     * event.Layout of vertex data in the vertex buffer.
     */
    struct VertexData {
        /** event.Position (xyz) and texture coordinate u (w). */
        float4 position;
        /** event.Normal (xyz) and texture coordinate v (w). */
        float4 normal;
        /** event.Tangent (xyz) and bitangent sign (w). */
        float4 tangent;

        /** event.Descriptor for vertex attributes. */
        static const std::vector<vireo::VertexAttributeDesc> vertexAttributes;
    };

    /**
     * event.A single draw instance.
     *
     * Indices reference engine-wide arrays for mesh instances, mesh surfaces
     * and materials.
     */
    struct alignas(8) InstanceData {
        /** event.Index of the MeshInstance in the global instances array. */
        uint32 meshInstanceIndex;
        /** event.Index of the mesh surface within the global surfaces array. */
        uint32 meshSurfaceIndex;
        /** event.Index of the material used by this instance. */
        uint32 materialIndex;
        /** event.Index of the mesh-surface-local material slot. */
        uint32 meshSurfaceMaterialIndex;
    };

    /**
     * event.A single indirect draw coupled with the instance index it belongs to.
     */
    struct DrawCommand {
        /** event.InstanceData index associated to this draw. */
        uint32 instanceIndex;
        /** event.Standard indexed indirect draw command parameters. */
        vireo::DrawIndexedIndirectCommand command;
    };

    /**
     * event.Per-pipeline data for instances, draw command arrays and culling.
     *
     * Stores instance data, staging/copy buffers and the compute pipeline
     * used to perform frustum culling and produce culled indirect draws.
     */
    struct GraphicPipelineData {
        /** event.Descriptor binding for per-instance buffer used by pipelines. */
        static constexpr vireo::DescriptorIndex BINDING_INSTANCES{0};
        /** event.Shared descriptor layout for pipeline-local resources. */
        inline static std::shared_ptr<vireo::DescriptorLayout> pipelineDescriptorLayout{nullptr};
        /**
         * event.Create the shared descriptor layout.
         * @param vireo Shared pointer to the Vireo instance.
         */
        static void createDescriptorLayouts(const std::shared_ptr<vireo::Vireo>& vireo);
        /** event.Destroy the shared descriptor layout. */
        static void destroyDescriptorLayouts();

        /** event.Identifier of the material/pipeline family. */
        pipeline_id pipelineId;
        /** event.Descriptor set bound when drawing with this pipeline. */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** event.Compute pipeline used to cull draw commands against the frustum. */
        FrustumCulling frustumCullingPipeline;
        /** event.Reference to the material manager. */
        MaterialManager& materialManager;
        /** event.Reference to Vireo. */
        std::shared_ptr<vireo::Vireo> vireo;

        /** event.Flag tracking if the instances set has been updated. */
        bool instancesUpdated{false};
        /** event.Set of mesh instances scheduled for removal. */
        std::unordered_set<const MeshInstance*> instancesToRemove;
        /** event.Device memory array that stores InstanceData blocks. */
        DeviceMemoryArray instancesArray;
        /** event.Mapping of mesh instance to its memory block within instancesArray. */
        std::unordered_map<const MeshInstance*, MemoryBlock> instancesMemoryBlocks;

        /** event.Number of indirect draw commands before culling. */
        uint32 drawCommandsCount{0};
        /** event.CPU-side list of draw commands to upload. */
        std::vector<DrawCommand> drawCommands;
        /** event.GPU buffer storing indirect draw commands. */
        std::shared_ptr<vireo::Buffer> drawCommandsBuffer;
        /** event.GPU buffer storing the count of culled draw commands. */
        std::shared_ptr<vireo::Buffer> culledDrawCommandsCountBuffer;
        /** event.GPU buffer storing culled indirect draw commands. */
        std::shared_ptr<vireo::Buffer> culledDrawCommandsBuffer;

        /** event.Staging buffer used to copy draw commands to the GPU. */
        std::shared_ptr<vireo::Buffer> drawCommandsStagingBuffer;
        /** event.Number of draw commands stored in the current staging buffer. */
        uint32 drawCommandsStagingBufferCount{0};

        /**
         * event.Create a pipeline data object for a specific material/pipeline ID.
         * 
         * @param ctx Reference to the rendering context.
         * @param pipelineId Identifier of the pipeline.
         * @param meshInstancesDataArray Array storing per-mesh-instance data.
         * @param maxMeshSurfacePerPipeline Maximum number of mesh surfaces supported by this pipeline.
         */
        GraphicPipelineData(
            const Context& ctx,
            uint32 pipelineId,
            const DeviceMemoryArray& meshInstancesDataArray,
            uint32 maxMeshSurfacePerPipeline);

        /**
         * event.Registers a mesh instance into this pipeline data object.
         * @param meshInstance Pointer to the mesh instance to add.
         * @param meshInstancesDataMemoryBlocks Map of memory blocks for mesh instance data.
         */
        void addInstance(
            const MeshInstance* meshInstance,
            const std::unordered_map<const MeshInstance*, MemoryBlock>& meshInstancesDataMemoryBlocks);

        /**
         * event.Removes a previously registered mesh instance.
         * @param meshInstance Pointer to the mesh instance to remove.
         */
        void removeInstance(
            const MeshInstance* meshInstance);

        /**
         * event.Adds a single draw instance and wires memory blocks.
         * @param meshInstance Pointer to the mesh instance.
         * @param instanceMemoryBlock Memory block for the instance data.
         * @param meshInstanceMemoryBlock Memory block for the mesh instance data.
         */
        void addInstance(
            const MeshInstance* meshInstance,
            const MemoryBlock& instanceMemoryBlock,
            const MemoryBlock& meshInstanceMemoryBlock);

        /**
         * event.Uploads/refreshes GPU buffers and prepares culled draw arrays.
         * 
         * @param commandList Command buffer for GPU operations.
         * @param drawCommandsStagingBufferRecycleBin Set for recycling staging buffers.
         * @param meshInstancesDataMemoryBlocks Map of memory blocks for mesh instance data.
         */
        void updateData(
            const vireo::CommandList& commandList,
            std::unordered_set<std::shared_ptr<vireo::Buffer>>& drawCommandsStagingBufferRecycleBin,
            const std::unordered_map<const MeshInstance*, MemoryBlock>& meshInstancesDataMemoryBlocks);
    };

}
