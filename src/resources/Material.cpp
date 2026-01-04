/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.resources.material;
#include <xxhash.h>

import lysa.log;

namespace lysa {

    Material::Material(Context& ctx, const Type type):
        ManagedResource{}, ctx(ctx), type{type} {
    }

    void Material::upload() const {
        ctx.res.get<MaterialManager>().upload(*this);
    }

    MaterialData StandardMaterial::getMaterialData() const {
        auto data = MaterialData {
            .albedoColor = albedoColor,
            .pipelineId = getPipelineId(),
            .transparency = static_cast<int>(getTransparency()),
            .alphaScissor = getAlphaScissor(),
            .normalScale = normalScale,
            .metallicFactor = metallicFactor,
            .roughnessFactor = roughnessFactor,
            .emissiveFactor = float4{emissiveFactor, emissiveStrength}
        };
        if (diffuseTexture.texture.image != INVALID_ID) {
            data.diffuseTexture = {
                .index = static_cast<int32>(imageManager[diffuseTexture.texture.image].getIndex()),
                .samplerIndex = diffuseTexture.texture.samplerIndex,
                .transform = float4x4{diffuseTexture.transform},
            };
        }
        if (normalTexture.texture.image != INVALID_ID) {
            data.normalTexture = {
                .index = static_cast<int32>(imageManager[normalTexture.texture.image].getIndex()),
                .samplerIndex = normalTexture.texture.samplerIndex,
                .transform = float4x4{normalTexture.transform},
            };
        }
        if (metallicTexture.texture.image != INVALID_ID) {
            data.metallicTexture = {
                .index = static_cast<int32>(imageManager[metallicTexture.texture.image].getIndex()),
                .samplerIndex = metallicTexture.texture.samplerIndex,
                .transform = float4x4{metallicTexture.transform},
            };
        }
        if (roughnessTexture.texture.image != INVALID_ID) {
            data.roughnessTexture = {
                .index = static_cast<int32>(imageManager[roughnessTexture.texture.image].getIndex()),
                .samplerIndex = roughnessTexture.texture.samplerIndex,
                .transform = float4x4{roughnessTexture.transform},
            };
        }
        if (emissiveTexture.texture.image != INVALID_ID) {
            data.emissiveTexture = {
                .index = static_cast<int32>(imageManager[emissiveTexture.texture.image].getIndex()),
                .samplerIndex = emissiveTexture.texture.samplerIndex,
                .transform = float4x4{emissiveTexture.transform},
            };
        }
        return data;
    }

    StandardMaterial::StandardMaterial(Context& ctx):
        Material(ctx, STANDARD),
        imageManager(ctx.res.get<ImageManager>()) {
    }

    void StandardMaterial::setAlbedoColor(const float4 &color) {
        albedoColor = color;
        upload();
    }

    StandardMaterial::~StandardMaterial() {
        if (diffuseTexture.texture.image != INVALID_ID) {
            imageManager.destroy(diffuseTexture.texture.image);
        }
        if (normalTexture.texture.image != INVALID_ID) {
            imageManager.destroy(normalTexture.texture.image);
        }
        if (metallicTexture.texture.image != INVALID_ID) {
            imageManager.destroy(metallicTexture.texture.image);
        }
        if (roughnessTexture.texture.image != INVALID_ID) {
            imageManager.destroy(roughnessTexture.texture.image);
        }
        if (emissiveTexture.texture.image != INVALID_ID) {
            imageManager.destroy(emissiveTexture.texture.image);
        }
    }

    void StandardMaterial::setDiffuseTexture(const TextureInfo &texture) {
        diffuseTexture = texture;
        if (texture.texture.image != INVALID_ID) {
            imageManager.use(texture.texture.image);
        }
        upload();
    }

    void StandardMaterial::setNormalTexture(const TextureInfo &texture) {
        normalTexture = texture;
        if (texture.texture.image != INVALID_ID) {
            imageManager.use(texture.texture.image);
        }
        upload();
    }

    void StandardMaterial::setMetallicTexture(const TextureInfo &texture) {
        metallicTexture = texture;
        if (texture.texture.image != INVALID_ID) {
            imageManager.use(texture.texture.image);
        }
        if (metallicFactor == -1.0f) { metallicFactor = 0.0f; }
        upload();
    }

    void StandardMaterial::setRoughnessTexture(const TextureInfo &texture) {
        roughnessTexture = texture;
        if (texture.texture.image != INVALID_ID) {
            imageManager.use(texture.texture.image);
        }
        if (roughnessFactor == -1.0f) { roughnessFactor = 0.0f; }
        upload();
    }

    void StandardMaterial::setEmissiveTexture(const TextureInfo &texture) {
        emissiveTexture = texture;
        if (texture.texture.image != INVALID_ID) {
            imageManager.use(texture.texture.image);
        }
        upload();
    }

    void StandardMaterial::setMetallicFactor(const float metallic) {
        this->metallicFactor = metallic;
        upload();
    }

    void StandardMaterial::setEmissiveStrength(const float strength) {
        this->emissiveStrength = strength;
        upload();
    }

    void StandardMaterial::setRoughnessFactor(const float roughness) {
        this->roughnessFactor = roughness;
        upload();
    }

    void StandardMaterial::setEmissiveFactor(const float3& factor) {
        emissiveFactor = factor;
        upload();
    }

    void StandardMaterial::setNormalScale(const float scale) {
        normalScale = scale;
        upload();
    }

    pipeline_id StandardMaterial::getPipelineId() const {
        const auto name = std::format("{0}{1}{2}",
            DEFAULT_PIPELINE_ID,
            static_cast<uint32>(getTransparency()),
            static_cast<uint32>(getCullMode()));
        return XXH32(name.c_str(), name.size(), 0);
    }

    ShaderMaterial::ShaderMaterial(Context& ctx, const std::shared_ptr<ShaderMaterial> &orig):
        Material{ctx, SHADER},
        fragFileName{orig->fragFileName},
        vertFileName{orig->vertFileName} {
        for (int i = 0; i < SHADER_MATERIAL_MAX_PARAMETERS; i++) {
            parameters[i] = orig->parameters[i];
        }
        upload();
    }

    ShaderMaterial::ShaderMaterial(Context& ctx,
                                    const std::string &fragShaderFileName,
                                   const std::string &vertShaderFileName):
        Material{ctx, SHADER},
        fragFileName{fragShaderFileName},
        vertFileName{vertShaderFileName} {
    }

    void ShaderMaterial::setParameter(const int index, const float4& value) {
        parameters[index] = value;
        upload();
    }

    pipeline_id ShaderMaterial::getPipelineId() const {
        const auto name = vertFileName + fragFileName;
        return XXH32(name.c_str(), name.size(), 0);
    }

    MaterialData ShaderMaterial::getMaterialData() const {
        return {
            .pipelineId = getPipelineId(),
            .parameters = {
                parameters[0],
                parameters[1],
                parameters[2],
                parameters[3],
            }
        };
    }

    MaterialManager::MaterialManager(Context& ctx, const size_t capacity) :
        ResourcesManager(ctx, capacity, "MaterialManager"),
        memoryArray {
            ctx.vireo,
            sizeof(MaterialData),
            static_cast<size_t>(capacity),
            static_cast<size_t>(capacity),
            vireo::BufferType::DEVICE_STORAGE,
            "Global material array"} {
        ctx.res.enroll(*this);
    }

    StandardMaterial& MaterialManager::create() {
        auto& material = dynamic_cast<StandardMaterial&>(allocate(std::make_unique<StandardMaterial>(ctx)));
        material.upload();
        return material;
    }

    ShaderMaterial& MaterialManager::create(const std::shared_ptr<ShaderMaterial> &orig) {
        auto& material = dynamic_cast<ShaderMaterial&>(allocate(std::make_unique<ShaderMaterial>(ctx, orig)));
        material.upload();
        return material;
    }

    ShaderMaterial& MaterialManager::create(
        const std::string &fragShaderFileName,
        const std::string &vertShaderFileName) {
        auto& material = dynamic_cast<ShaderMaterial&>(allocate(std::make_unique<ShaderMaterial>(ctx, fragShaderFileName, vertShaderFileName)));
        material.upload();
        return material;
    }

    void MaterialManager::upload(const Material& material) {
        if (material.bypassUpload) { return; }
        needUpload.insert(material.id);
    }

    void MaterialManager::flush() {
        if (!needUpload.empty()) {
            auto lock = std::unique_lock(mutex);
            for (const auto id : needUpload) {
                auto& material = (*this)[id];
                if (!material.isUploaded()) {
                    material.memoryBloc = memoryArray.alloc(1);
                }
                const auto materialData = material.getMaterialData();
                memoryArray.write(material.memoryBloc, &materialData);
            }
            needUpload.clear();
            const auto command = ctx.asyncQueue.beginCommand(vireo::CommandType::TRANSFER);
            memoryArray.flush(*command.commandList);
            ctx.asyncQueue.endCommand(command);
        }
    }

    bool MaterialManager::destroy(const unique_id id) {
        const auto& material = (*this)[id];
        if (material.refCounter <= 1 && material.isUploaded()) {
            memoryArray.free(material.memoryBloc);
        }
        needUpload.erase(id);
        return ResourcesManager::destroy(id);
    }

}
