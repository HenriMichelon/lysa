/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.pipelines.frustum_culling;

import lysa.log;
import lysa.virtual_fs;

namespace lysa {

    std::shared_ptr<vireo::DescriptorLayout> FrustumCulling::descriptorLayout;
    std::unordered_map<std::string, std::shared_ptr<vireo::ShaderModule>> FrustumCulling::shaderModules{};
    std::unordered_map<std::string, std::shared_ptr<vireo::Pipeline>> FrustumCulling::pipelines{};

    FrustumCulling::FrustumCulling(
        const Context& ctx,
        const bool isForScene,
        const DeviceMemoryArray& meshInstancesArray,
        pipeline_id pipelineId) {
        const auto& vireo = *ctx.vireo;
        auto debugName = DEBUG_NAME + ":" + std::to_string(pipelineId);
        globalBuffer = vireo.createBuffer(vireo::BufferType::UNIFORM, sizeof(Utils), 1, debugName + "/global");
        globalBuffer->map();
        commandClearCounterBuffer = vireo.createBuffer(vireo::BufferType::BUFFER_UPLOAD, sizeof(uint32), 1, debugName + "/commandClearCounter");
        constexpr auto clearValue = 0;
        commandClearCounterBuffer->map();
        commandClearCounterBuffer->write(&clearValue);
        commandClearCounterBuffer->unmap();

        downloadCounterBuffer = vireo.createBuffer(vireo::BufferType::BUFFER_DOWNLOAD, sizeof(uint32), 1, debugName + "/downloadCounter");
        downloadCounterBuffer->map();

        if (descriptorLayout == nullptr) {
            descriptorLayout = vireo.createDescriptorLayout(DEBUG_NAME);
            descriptorLayout->add(BINDING_GLOBAL, vireo::DescriptorType::UNIFORM);
            descriptorLayout->add(BINDING_MESHINSTANCES, vireo::DescriptorType::DEVICE_STORAGE);
            descriptorLayout->add(BINDING_INSTANCES, vireo::DescriptorType::DEVICE_STORAGE);
            descriptorLayout->add(BINDING_INPUT, vireo::DescriptorType::DEVICE_STORAGE);
            descriptorLayout->add(BINDING_OUTPUT, vireo::DescriptorType::READWRITE_STORAGE);
            descriptorLayout->add(BINDING_COUNTER, vireo::DescriptorType::READWRITE_STORAGE);
            descriptorLayout->build();
        }

        descriptorSet = vireo.createDescriptorSet(descriptorLayout, debugName);
        descriptorSet->update(BINDING_GLOBAL, globalBuffer);
        descriptorSet->update(BINDING_MESHINSTANCES, meshInstancesArray.getBuffer());

        auto& shaderName = isForScene ? SHADER_SCENE : SHADER_SHADOWMAP;
        if (!shaderModules.contains(shaderName)) {
            const auto pipelineResources = vireo.createPipelineResources(
                { descriptorLayout },
                {},
                DEBUG_NAME);
            auto tempBuffer = std::vector<char>{};
            ctx.fs.loadShader(shaderName, tempBuffer);
            shaderModules[shaderName] = vireo.createShaderModule(tempBuffer, shaderName);
            pipelines[shaderName] = vireo.createComputePipeline(pipelineResources, shaderModules[shaderName], shaderName);
        }
        pipeline = pipelines[shaderName];
    }

    void FrustumCulling::cleanup() {
        pipelines.clear();
        shaderModules.clear();
        descriptorLayout.reset();
    }

    void FrustumCulling::dispatch(
        vireo::CommandList& commandList,
        const uint32 drawCommandsCount,
        const float4x4& view,
        const float4x4& projection,
        const vireo::Buffer& instances,
        const vireo::Buffer& input,
        const vireo::Buffer& output,
        const vireo::Buffer& counter) {
        commandList.barrier(
            counter,
            vireo::ResourceState::INDIRECT_DRAW,
            vireo::ResourceState::COPY_DST);
        commandList.copy(*commandClearCounterBuffer, counter);
        commandList.barrier(
            counter,
            vireo::ResourceState::COPY_DST,
            vireo::ResourceState::COMPUTE_WRITE);
        if (drawCommandsCount == 0) { return; }

        auto global = Utils{
            .drawCommandsCount = drawCommandsCount,
            .viewMatrix = inverse(view),
        };
        Frustum::extractPlanes(global.planes, mul(global.viewMatrix, projection));
        globalBuffer->write(&global);

        descriptorSet->update(BINDING_INSTANCES, instances);
        descriptorSet->update(BINDING_INPUT, input);
        descriptorSet->update(BINDING_OUTPUT, output, counter);
        descriptorSet->update(BINDING_COUNTER, counter);

        commandList.barrier(
            input,
            vireo::ResourceState::INDIRECT_DRAW,
            vireo::ResourceState::COMPUTE_READ);
        commandList.barrier(
            output,
            vireo::ResourceState::INDIRECT_DRAW,
            vireo::ResourceState::COMPUTE_WRITE);
        commandList.bindPipeline(pipeline);
        commandList.bindDescriptors({ descriptorSet });
        commandList.dispatch((drawCommandsCount + 63) / 64, 1, 1);
        commandList.barrier(
            output,
            vireo::ResourceState::COMPUTE_WRITE,
            vireo::ResourceState::INDIRECT_DRAW);
        commandList.barrier(
            input,
            vireo::ResourceState::COMPUTE_READ,
            vireo::ResourceState::INDIRECT_DRAW);

        commandList.barrier(
            counter,
            vireo::ResourceState::COMPUTE_WRITE,
            vireo::ResourceState::COPY_SRC);
        commandList.copy(counter, *downloadCounterBuffer);
        commandList.barrier(
            counter,
            vireo::ResourceState::COPY_SRC,
            vireo::ResourceState::INDIRECT_DRAW);
    }

    uint32 FrustumCulling::getDrawCommandsCount() const {
        return *static_cast<uint32*>(downloadCounterBuffer->getMappedAddress());
    }

}