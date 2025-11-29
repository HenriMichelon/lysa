/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.graphic_pipeline_data;

import vireo;

import lysa.types;
import lysa.memory;
import lysa.pipelines.frustum_culling;
import lysa.renderers.configuration;

export namespace lysa {

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
     * Per-pipeline data for instances, draw command streams and culling.
     *
     * Stores instance data, staging/copy buffers and the compute pipeline
     * used to perform frustum culling and produce culled indirect draws.
     */
    struct GraphicPipelineData {
        /** Identifier of the material/pipeline family. */
        pipeline_id pipelineId;
        /** Reference to the scene configuration. */
        const SceneRenderContextConfiguration& config;
        /** Descriptor set bound when drawing with this pipeline. */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** Compute pipeline used to cull draw commands against the frustum. */
        FrustumCulling frustumCullingPipeline;

        /** Flags tracking mutations in the instances set. */
        bool instancesUpdated{false};
        bool instancesRemoved{false};
        /** Device memory array that stores InstanceData blocks. */
        DeviceMemoryArray instancesArray;
        /** Mapping of mesh instance to its memory block within instancesArray. */
        //std::unordered_map<std::shared_ptr<MeshInstance>, MemoryBlock> instancesMemoryBlocks;

        /** Number of indirect draw commands before culling. */
        uint32 drawCommandsCount{0};
        /** CPU-side list of draw commands to upload. */
        std::vector<DrawCommand> drawCommands;
        /** GPU buffer holding indirect draw commands. */
        std::shared_ptr<vireo::Buffer> drawCommandsBuffer;
        /** GPU buffer storing the count of culled draw commands. */
        std::shared_ptr<vireo::Buffer> culledDrawCommandsCountBuffer;
        /** GPU buffer holding culled indirect draw commands. */
        std::shared_ptr<vireo::Buffer> culledDrawCommandsBuffer;

        /** Staging buffer used to stream draw commands to the GPU. */
        std::shared_ptr<vireo::Buffer> drawCommandsStagingBuffer;
        /** Number of draw commands stored in the current staging buffer. */
        uint32 drawCommandsStagingBufferCount{0};

        GraphicPipelineData::GraphicPipelineData(
            const std::shared_ptr<vireo::Vireo>& vireo,
            const SceneRenderContextConfiguration& config,
            uint32 pipelineId,
            const DeviceMemoryArray& meshInstancesDataArray);

        /** Registers a mesh instance into this pipeline cache. */
        // void addNode(
        //     const std::shared_ptr<MeshInstance>& meshInstance,
        //     const std::unordered_map<std::shared_ptr<MeshInstance>, MemoryBlock>& meshInstancesDataMemoryBlocks);

        /** Removes a previously registered mesh instance. */
        // void removeNode(
            // const std::shared_ptr<MeshInstance>& meshInstance);

        /** Adds a single draw instance and wires memory blocks. */
        // void addInstance(
            // const std::shared_ptr<MeshInstance>& meshInstance,
            // const MemoryBlock& instanceMemoryBlock,
            // const MemoryBlock& meshInstanceMemoryBlock);

        /** Uploads/refreshes GPU buffers and prepares culled draw streams. */
        // void updateData(
            // const vireo::CommandList& commandList,
            // std::unordered_set<std::shared_ptr<vireo::Buffer>>& drawCommandsStagingBufferRecycleBin,
            // const std::unordered_map<std::shared_ptr<MeshInstance>, MemoryBlock>& meshInstancesDataMemoryBlocks);
    };

}
