/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.global_descriptor_set;

import vireo;
import lysa.resources.material;
import lysa.resources.mesh;

namespace lysa {

    GlobalDescriptorSet::GlobalDescriptorSet(Context& ctx):
        ctx(ctx),
        imageManager(ctx.res.get<ImageManager>()) {
        descriptorLayout = ctx.vireo->createDescriptorLayout("Global");
        descriptorLayout->add(BINDING_MATERIALS, vireo::DescriptorType::DEVICE_STORAGE);
        descriptorLayout->add(BINDING_SURFACES, vireo::DescriptorType::DEVICE_STORAGE);
        descriptorLayout->add(BINDING_TEXTURES, vireo::DescriptorType::SAMPLED_IMAGE, imageManager.getCapacity());
        descriptorLayout->build();

        descriptorSet = ctx.vireo->createDescriptorSet(descriptorLayout, "Global");
        descriptorSet->update(BINDING_MATERIALS, ctx.res.get<MaterialManager>().getBuffer());
        descriptorSet->update(BINDING_SURFACES,  ctx.res.get<MeshManager>().getMeshSurfaceBuffer());
        descriptorSet->update(BINDING_TEXTURES, imageManager.getImages());
    }

    GlobalDescriptorSet::~GlobalDescriptorSet() {
        descriptorLayout.reset();
        descriptorSet.reset();
    }

    void GlobalDescriptorSet::update() {
        if (imageManager.isUpdateNeeded()) {
            auto lock = std::lock_guard(mutex);
            ctx.graphicQueue->waitIdle();
            descriptorSet->update(BINDING_TEXTURES, imageManager.getImages());
            imageManager.resetUpdateFlag();
        }
    }

}
