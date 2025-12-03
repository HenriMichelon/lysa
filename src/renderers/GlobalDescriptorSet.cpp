/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.global_descriptor_set;

import vireo;

namespace lysa {

    GlobalDescriptorSet::GlobalDescriptorSet(Context& ctx):
        ctx(ctx),
        imageManager(ctx.resources.get<ImageManager>()) {
        descriptorLayout = ctx.vireo->createDescriptorLayout("Global");
        descriptorLayout->add(BINDING_MATERIALS, vireo::DescriptorType::DEVICE_STORAGE);
        descriptorLayout->add(BINDING_SURFACES, vireo::DescriptorType::DEVICE_STORAGE);
        descriptorLayout->add(BINDING_TEXTURES, vireo::DescriptorType::SAMPLED_IMAGE,
            imageManager.getCapacity());
        descriptorLayout->build();

        descriptorSet = ctx.vireo->createDescriptorSet(descriptorLayout, "Global");
        // descriptorSet->update(BINDING_MATERIAL, materialArray.getBuffer());
        // descriptorSet->update(BINDING_SURFACES, meshSurfaceArray.getBuffer());
        descriptorSet->update(BINDING_TEXTURES, imageManager.getImages());
    }

    GlobalDescriptorSet::~GlobalDescriptorSet() {
        descriptorLayout.reset();
        descriptorSet.reset();
    }

    void GlobalDescriptorSet::flush() {
        auto lock = std::unique_lock(mutex, std::try_to_lock);
        const auto command = ctx.asyncQueue.beginCommand(vireo::CommandType::TRANSFER);
        // materialArray.flush(*command.commandList);
        // meshSurfaceArray.flush(*command.commandList);
        ctx.asyncQueue.endCommand(command);
        updated = false;
    }

    void GlobalDescriptorSet::update() {
        auto lock = std::lock_guard(mutex);
        if (imageManager.isUpdateNeeded()) {
            ctx.graphicQueue->waitIdle();
            descriptorSet->update(BINDING_TEXTURES, imageManager.getImages());
            imageManager.resetUpdateFlag();
        }
    }


}
