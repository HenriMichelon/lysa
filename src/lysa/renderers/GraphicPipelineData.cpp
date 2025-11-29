/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.graphic_pipeline_data;

import vireo;

namespace lysa {

    GraphicPipelineData::GraphicPipelineData(
        const std::shared_ptr<vireo::Vireo>& vireo,
        const SceneRenderContextConfiguration& config,
        const uint32 pipelineId,
        const DeviceMemoryArray& meshInstancesDataArray) :
        pipelineId{pipelineId},
        config{config},
        frustumCullingPipeline{true, meshInstancesDataArray},
        instancesArray{
            vireo,
            sizeof(InstanceData),
            config.maxMeshSurfacePerPipeline,
            config.maxMeshSurfadcePerPipeline,
            vireo::BufferType::DEVICE_STORAGE,
            "Pipeline instances array"},
        drawCommands(config.maxMeshSurfacePerPipeline),
        drawCommandsBuffer{vireo->createBuffer(
            vireo::BufferType::DEVICE_STORAGE,
            sizeof(DrawCommand) * config.maxMeshSurfacePerPipeline,
            1,
            "Pipeline draw commands")},
        culledDrawCommandsCountBuffer{vireo->createBuffer(
            vireo::BufferType::READWRITE_STORAGE,
            sizeof(uint32),
            1,
            "Pipeline draw commands counter")},
        culledDrawCommandsBuffer{vireo->createBuffer(
            vireo::BufferType::READWRITE_STORAGE,
            sizeof(DrawCommand) * config.maxMeshSurfacePerPipeline,
            1,
            "Pipeline culled draw commands")}
    {
        descriptorSet = vireo->createDescriptorSet(pipelineDescriptorLayout, "Pipeline");
        descriptorSet->update(BINDING_INSTANCES, instancesArray.getBuffer());
    }

    void GraphicPipelineData::addNode(
        const std::shared_ptr<MeshInstance>& meshInstance,
        const std::unordered_map<std::shared_ptr<MeshInstance>, MemoryBlock>& meshInstancesDataMemoryBlocks) {
        const auto& mesh = meshInstance->getMesh();
        const auto instanceMemoryBlock = instancesArray.alloc(mesh->getSurfaces().size());
        instancesMemoryBlocks[meshInstance] = instanceMemoryBlock;
        addInstance(meshInstance, instanceMemoryBlock, meshInstancesDataMemoryBlocks.at(meshInstance));
    }

    void GraphicPipelineData::addInstance(
        const std::shared_ptr<MeshInstance>& meshInstance,
        const MemoryBlock& instanceMemoryBlock,
        const MemoryBlock& meshInstanceMemoryBlock) {
        const auto& mesh = meshInstance->getMesh();
        auto instancesData = std::vector<InstanceData>{};
        for (uint32 i = 0; i < mesh->getSurfaces().size(); i++) {
            const auto& surface = mesh->getSurfaces()[i];
            const auto& material = meshInstance->getSurfaceMaterial(i);
            if (material->getPipelineId() == pipelineId) {
                const uint32 id = instanceMemoryBlock.instanceIndex + instancesData.size();
                drawCommands[drawCommandsCount] = {
                    .instanceIndex = id,
                    .command = {
                        .indexCount = surface->indexCount,
                        .instanceCount = 1,
                        .firstIndex = mesh->getIndicesIndex() + surface->firstIndex,
                        .vertexOffset = static_cast<int32>(mesh->getVerticesIndex()),
                        .firstInstance = id,
                    }
                };
                instancesData.push_back(InstanceData {
                    .meshInstanceIndex = meshInstanceMemoryBlock.instanceIndex,
                    .meshSurfaceIndex = mesh->getSurfacesIndex() + i,
                    .materialIndex = material->getMaterialIndex(),
                    .meshSurfaceMaterialIndex = mesh->getSurfaceMaterial(i)->getMaterialIndex(),
                });
                drawCommandsCount++;
            }
        }
        if (!instancesData.empty()) {
            instancesArray.write(instanceMemoryBlock, instancesData.data());
            instancesUpdated = true;
        }
    }

    void GraphicPipelineData::removeNode(
        const std::shared_ptr<MeshInstance>& meshInstance) {
        if (instancesMemoryBlocks.contains(meshInstance)) {
            instancesArray.free(instancesMemoryBlocks.at(meshInstance));
            instancesMemoryBlocks.erase(meshInstance);
            drawCommandsCount = 0;
            instancesRemoved = true;
        }
    }

    void GraphicPipelineData::updateData(
        const vireo::CommandList& commandList,
        std::unordered_set<std::shared_ptr<vireo::Buffer>>& drawCommandsStagingBufferRecycleBin,
        const std::unordered_map<std::shared_ptr<MeshInstance>, MemoryBlock>& meshInstancesDataMemoryBlocks) {
        if (instancesRemoved) {
            for (const auto& instance : std::views::keys(instancesMemoryBlocks)) {
                addInstance(
                    instance,
                    instancesMemoryBlocks.at(instance),
                    meshInstancesDataMemoryBlocks.at(instance));
            }
            instancesRemoved = false;
        }
        if (instancesUpdated) {
            instancesArray.flush(commandList);
            instancesArray.postBarrier(commandList);
            if (drawCommandsStagingBufferCount < drawCommandsCount) {
                if (drawCommandsStagingBuffer) {
                    drawCommandsStagingBufferRecycleBin.insert(drawCommandsStagingBuffer);
                }
                drawCommandsStagingBuffer = ctx.vireo->createBuffer(
                    vireo::BufferType::BUFFER_UPLOAD,
                    sizeof(DrawCommand) * drawCommandsCount);
                drawCommandsStagingBufferCount = drawCommandsCount;
                drawCommandsStagingBuffer->map();
            }

            drawCommandsStagingBuffer->write(drawCommands.data(),
                sizeof(DrawCommand) * drawCommandsCount);
            commandList.copy(drawCommandsStagingBuffer, drawCommandsBuffer, sizeof(DrawCommand) * drawCommandsCount);
            instancesUpdated = false;
            commandList.barrier(
                *drawCommandsBuffer,
                vireo::ResourceState::COPY_DST,
                vireo::ResourceState::INDIRECT_DRAW);
            commandList.barrier(
                *culledDrawCommandsBuffer,
                vireo::ResourceState::COPY_DST,
                vireo::ResourceState::INDIRECT_DRAW);
        }
    }

}
