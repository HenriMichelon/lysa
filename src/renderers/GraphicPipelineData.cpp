/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.graphic_pipeline_data;

import vireo;

namespace lysa {

    unique_id MeshInstanceDesc::getSurfaceMaterial(const uint32 surfaceIndex) const {
        if (materialsOverride.contains(surfaceIndex)) {
            return materialsOverride.at(surfaceIndex);
        }
        return mesh.getSurfaces()[surfaceIndex].material;
    }

    MeshInstanceData MeshInstanceDesc::getData() const {
        return {
            .transform = worldTransform,
            .aabbMin = worldAABB.min,
            .aabbMax = worldAABB.max,
            .visible = visible ? 1u : 0u,
            .castShadows = castShadows ? 1u : 0u,
        };
    }

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
        const SceneRenderContextConfiguration& config,
        const uint32 pipelineId,
        const DeviceMemoryArray& meshInstancesDataArray) :
        pipelineId{pipelineId},
        config{config},
        frustumCullingPipeline{ctx, true, meshInstancesDataArray},
        materialManager(ctx.res.get<MaterialManager>()),
        vireo(vireo),
        instancesArray{
            ctx.vireo,
            sizeof(InstanceData),
            config.maxMeshSurfacePerPipeline,
            config.maxMeshSurfacePerPipeline,
            vireo::BufferType::DEVICE_STORAGE,
            "Pipeline instances array"},
        drawCommands(config.maxMeshSurfacePerPipeline),
        drawCommandsBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::DEVICE_STORAGE,
            sizeof(DrawCommand) * config.maxMeshSurfacePerPipeline,
            1,
            "Pipeline draw commands")},
        culledDrawCommandsCountBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::READWRITE_STORAGE,
            sizeof(uint32),
            1,
            "Pipeline draw commands counter")},
        culledDrawCommandsBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::READWRITE_STORAGE,
            sizeof(DrawCommand) * config.maxMeshSurfacePerPipeline,
            1,
            "Pipeline culled draw commands")}
    {
        descriptorSet = ctx.vireo->createDescriptorSet(pipelineDescriptorLayout, "Pipeline");
        descriptorSet->update(BINDING_INSTANCES, instancesArray.getBuffer());
    }

    void GraphicPipelineData::addInstance(
        const std::shared_ptr<MeshInstanceDesc>& meshInstance,
        const std::unordered_map<std::shared_ptr<MeshInstanceDesc>, MemoryBlock>& meshInstancesDataMemoryBlocks) {
        const auto& mesh = meshInstance->mesh;
        const auto instanceMemoryBlock = instancesArray.alloc(mesh.getSurfaces().size());
        instancesMemoryBlocks[meshInstance] = instanceMemoryBlock;
        addInstance(meshInstance, instanceMemoryBlock, meshInstancesDataMemoryBlocks.at(meshInstance));
    }

    void GraphicPipelineData::addInstance(
        const std::shared_ptr<MeshInstanceDesc>& meshInstance,
        const MemoryBlock& instanceMemoryBlock,
        const MemoryBlock& meshInstanceMemoryBlock) {
        const auto& mesh = meshInstance->mesh;
        auto instancesData = std::vector<InstanceData>{};
        for (uint32 i = 0; i < mesh.getSurfaces().size(); i++) {
            const auto& surface = mesh.getSurfaces()[i];
            const auto& material = materialManager[meshInstance->getSurfaceMaterial(i)];
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
        const std::shared_ptr<MeshInstanceDesc>& meshInstance) {
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
        const std::unordered_map<std::shared_ptr<MeshInstanceDesc>, MemoryBlock>& meshInstancesDataMemoryBlocks) {
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
