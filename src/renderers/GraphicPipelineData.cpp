/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.graphic_pipeline_data;

import vireo;
import lysa.log;

namespace lysa {

    LightData LightDesc::getData() const {
        return {
            .type = type,
            .position = float4{position, 0.0f},
            .color = colorAndIntensity
        };
    }

    void GraphicPipelineData::createDescriptorLayouts(const std::shared_ptr<vireo::Vireo>& vireo) {
        pipelineDescriptorLayout = vireo->createDescriptorLayout("Pipeline data");
        pipelineDescriptorLayout->add(BINDING_INSTANCES, vireo::DescriptorType::DEVICE_STORAGE);
        pipelineDescriptorLayout->build();
    }

    void GraphicPipelineData::destroyDescriptorLayouts() {
        pipelineDescriptorLayout.reset();
    }

    GraphicPipelineData::GraphicPipelineData(
        const Context& ctx,
        const uint32 pipelineId,
        const DeviceMemoryArray& meshInstancesDataArray,
        const uint32 maxMeshSurfacePerPipeline) :
        pipelineId{pipelineId},
        frustumCullingPipeline{ctx, true, meshInstancesDataArray, pipelineId},
        meshInstanceManager(ctx.res.get<MeshInstanceManager>()),
        materialManager(ctx.res.get<MaterialManager>()),
        vireo(ctx.vireo),
        instancesArray{
            ctx.vireo,
            sizeof(InstanceData),
            maxMeshSurfacePerPipeline,
            maxMeshSurfacePerPipeline,
            vireo::BufferType::DEVICE_STORAGE,
            "instance:" + std::to_string(pipelineId)},
        drawCommands(maxMeshSurfacePerPipeline),
        drawCommandsBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::DEVICE_STORAGE,
            sizeof(DrawCommand) * maxMeshSurfacePerPipeline,
            1,
            "drawCommand:" + std::to_string(pipelineId))},
        culledDrawCommandsCountBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::READWRITE_STORAGE,
            sizeof(uint32),
            1,
            "culledDrawCommandsCount:" + std::to_string(pipelineId))},
        culledDrawCommandsBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::READWRITE_STORAGE,
            sizeof(DrawCommand) * maxMeshSurfacePerPipeline,
            1,
            "culledDrawCommands:" + std::to_string(pipelineId))}
    {
        descriptorSet = ctx.vireo->createDescriptorSet(pipelineDescriptorLayout, "Graphic : " + std::to_string(pipelineId));
        descriptorSet->update(BINDING_INSTANCES, instancesArray.getBuffer());
    }

    void GraphicPipelineData::addInstance(
        const unique_id meshInstance,
        const std::unordered_map<unique_id, MemoryBlock>& meshInstancesDataMemoryBlocks) {
        const auto& mesh = meshInstanceManager[meshInstance].getMesh();
        const auto instanceMemoryBlock = instancesArray.alloc(mesh.getSurfaces().size());
        instancesMemoryBlocks[meshInstance] = instanceMemoryBlock;
        addInstance(meshInstance, instanceMemoryBlock, meshInstancesDataMemoryBlocks.at(meshInstance));
    }

    void GraphicPipelineData::addInstance(
        const unique_id meshInstance,
        const MemoryBlock& instanceMemoryBlock,
        const MemoryBlock& meshInstanceMemoryBlock) {
        const auto& mesh = meshInstanceManager[meshInstance].getMesh();
        auto instancesData = std::vector<InstanceData>{};
        for (uint32 i = 0; i < mesh.getSurfaces().size(); i++) {
            const auto& surface = mesh.getSurfaces()[i];
            const auto& material = materialManager[meshInstanceManager[meshInstance].getSurfaceMaterial(i)];
            if (material.getPipelineId() == pipelineId) {
                const uint32 id = instanceMemoryBlock.instanceIndex + instancesData.size();
                drawCommands[drawCommandsCount] = {
                    .instanceIndex = id,
                    .command = {
                        .indexCount = surface.indexCount,
                        .instanceCount = 1,
                        .firstIndex = mesh.getIndicesIndex() + surface.firstIndex,
                        .vertexOffset = static_cast<int32>(mesh.getVerticesIndex()),
                        .firstInstance = id,
                    }
                };
                instancesData.push_back(InstanceData {
                    .meshInstanceIndex = meshInstanceMemoryBlock.instanceIndex,
                    .meshSurfaceIndex = mesh.getSurfacesIndex() + i,
                    .materialIndex = material.getIndex(),
                    .meshSurfaceMaterialIndex =  materialManager[mesh.getSurfaceMaterial(i)].getIndex(),
                });
                drawCommandsCount++;
            }
        }
        if (!instancesData.empty()) {
            instancesArray.write(instanceMemoryBlock, instancesData.data());
            instancesUpdated = true;
        }
    }

    void GraphicPipelineData::removeInstance(
        unique_id meshInstance) {
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
        const std::unordered_map<unique_id, MemoryBlock>& meshInstancesDataMemoryBlocks) {
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
                drawCommandsStagingBuffer = vireo->createBuffer(
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
