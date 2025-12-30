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
        Resource{}, ctx(ctx), type{type} {
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
        if (diffuseTexture.texture) {
            data.diffuseTexture = {
                .index = static_cast<int32>(diffuseTexture.texture->getImage().getIndex()),
                .samplerIndex = diffuseTexture.texture->getSamplerIndex(),
                .transform = float4x4{diffuseTexture.transform},
            };
        }
        if (normalTexture.texture) {
            data.normalTexture = {
                .index = static_cast<int32>(normalTexture.texture->getImage().getIndex()),
                .samplerIndex = normalTexture.texture->getSamplerIndex(),
                .transform = float4x4{normalTexture.transform},
            };
        }
        if (metallicTexture.texture) {
            data.metallicTexture = {
                .index = static_cast<int32>(metallicTexture.texture->getImage().getIndex()),
                .samplerIndex = metallicTexture.texture->getSamplerIndex(),
                .transform = float4x4{metallicTexture.transform},
            };
        }
        if (roughnessTexture.texture) {
            data.roughnessTexture = {
                .index = static_cast<int32>(roughnessTexture.texture->getImage().getIndex()),
                .samplerIndex = roughnessTexture.texture->getSamplerIndex(),
                .transform = float4x4{roughnessTexture.transform},
            };
        }
        if (emissiveTexture.texture) {
            data.emissiveTexture = {
                .index = static_cast<int32>(emissiveTexture.texture->getImage().getIndex()),
                .samplerIndex = emissiveTexture.texture->getSamplerIndex(),
                .transform = float4x4{emissiveTexture.transform},
            };
        }
        return data;
    }

    StandardMaterial::StandardMaterial(Context& ctx):
        Material(ctx, STANDARD),
        imageTextureManager(ctx.res.get<ImageTextureManager>()){
    }

    StandardMaterial::~StandardMaterial() {
        if (diffuseTexture.texture) {
            imageTextureManager.destroy(diffuseTexture.texture->id);
        }
        if (metallicTexture.texture) {
            imageTextureManager.destroy(metallicTexture.texture->id);
        }
        if (roughnessTexture.texture) {
            imageTextureManager.destroy(roughnessTexture.texture->id);
        }
        if (emissiveTexture.texture) {
            imageTextureManager.destroy(emissiveTexture.texture->id);
        }
        if (normalTexture.texture) {
            imageTextureManager.destroy(normalTexture.texture->id);
        }
    }

    void StandardMaterial::setAlbedoColor(const float4 &color) {
        albedoColor = color;
        upload();
    }

    void StandardMaterial::setDiffuseTexture(const TextureInfo &texture) {
        diffuseTexture = texture;
        if (texture.texture) {
            imageTextureManager.use(texture.texture->id);
        }
        upload();
    }

    void StandardMaterial::setNormalTexture(const TextureInfo &texture) {
        normalTexture = texture;
        if (texture.texture) {
            imageTextureManager.use(texture.texture->id);
        }
        upload();
    }

    void StandardMaterial::setMetallicTexture(const TextureInfo &texture) {
        metallicTexture = texture;
        if (texture.texture) {
            imageTextureManager.use(texture.texture->id);
        }
        if (metallicFactor == -1.0f) { metallicFactor = 0.0f; }
        upload();
    }

    void StandardMaterial::setRoughnessTexture(const TextureInfo &texture) {
        roughnessTexture = texture;
        if (texture.texture) {
            imageTextureManager.use(texture.texture->id);
        }
        if (metallicFactor == -1.0f) { metallicFactor = 0.0f; }
        upload();
    }

    void StandardMaterial::setEmissiveTexture(const TextureInfo &texture) {
        emissiveTexture = texture;
        if (texture.texture) {
            imageTextureManager.use(texture.texture->id);
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
        return ResourcesManager::destroy(id);
    }

}
