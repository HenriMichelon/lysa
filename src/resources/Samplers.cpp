/* Copyright (c) 2025-present Henri Michelon
*
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */
module lysa.resources.samplers;

import lysa.exception;

namespace lysa {

    Samplers::Samplers(const std::shared_ptr<vireo::Vireo>& vireo, const size_t capacity):
        vireo{vireo},
        samplers(capacity),
        samplersInfo(capacity) {
        descriptorLayout = vireo->createSamplerDescriptorLayout("Static Samplers");
        descriptorLayout->add(0, vireo::DescriptorType::SAMPLER, capacity);
        descriptorLayout->build();
        descriptorSet = vireo->createDescriptorSet(descriptorLayout, "Static Samplers");

        addSampler(
            vireo::Filter::NEAREST,
            vireo::Filter::NEAREST,
            vireo::AddressMode::CLAMP_TO_BORDER,
            vireo::AddressMode::CLAMP_TO_BORDER);
        addSampler(
            vireo::Filter::LINEAR,
            vireo::Filter::LINEAR,
            vireo::AddressMode::CLAMP_TO_EDGE,
            vireo::AddressMode::CLAMP_TO_EDGE);
        addSampler(
            vireo::Filter::LINEAR,
            vireo::Filter::LINEAR,
            vireo::AddressMode::CLAMP_TO_EDGE,
            vireo::AddressMode::CLAMP_TO_EDGE,
            0.0f, vireo::Sampler::LOD_CLAMP_NONE,
            false);
        addSampler(
            vireo::Filter::LINEAR,
            vireo::Filter::LINEAR,
            vireo::AddressMode::REPEAT,
            vireo::AddressMode::REPEAT);
        addSampler(
            vireo::Filter::NEAREST,
            vireo::Filter::NEAREST,
            vireo::AddressMode::REPEAT,
            vireo::AddressMode::REPEAT);
        for (int i = samplerCount; i < samplers.size(); i++) {
            samplers[i] = samplers[0];
        }
    }

    void Samplers::update() {
        auto lock = std::lock_guard{mutex};
        descriptorSet->update(0, samplers);
        samplersUpdated = false;
    }

    uint32 Samplers::addSampler(
        const vireo::Filter minFilter,
        const vireo::Filter maxFilter,
        const vireo::AddressMode samplerAddressModeU,
        const vireo::AddressMode samplerAddressModeV,
        const float minLod,
        const float maxLod,
        const bool anisotropyEnable,
        const vireo::MipMapMode mipMapMode,
        const vireo::CompareOp compareOp) {
        auto lock = std::lock_guard{mutex};
        if (samplerCount >= samplers.size()) {
            throw Exception("Too many samplers");
        }
        const auto samplerInfo = SamplerInfo{
            minFilter, maxFilter,
            samplerAddressModeU, samplerAddressModeV,
            minLod, maxLod,
            anisotropyEnable,
            mipMapMode,compareOp};
        for (int i = 0; i < samplerCount; i++) {
            if (samplersInfo[i] == samplerInfo) {
                return i;
            }
        }
        const auto index = samplerCount;
        samplers[index] = vireo->createSampler(
            minFilter,
            maxFilter,
            samplerAddressModeU,
            samplerAddressModeV,
            samplerAddressModeV,
            minLod,
            maxLod,
            anisotropyEnable,
            mipMapMode,
            compareOp);
        samplersInfo[index] = samplerInfo;
        samplersUpdated = true;
        samplerCount++;
        return index;
    }

    Samplers::~Samplers() {
        samplers.clear();
        descriptorSet.reset();
        descriptorLayout.reset();
    }

}