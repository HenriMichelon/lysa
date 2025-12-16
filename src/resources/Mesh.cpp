/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <cstddef>
module lysa.resources.mesh;

import lysa.renderers.graphic_pipeline_data;

namespace lysa {

    const std::vector<vireo::VertexAttributeDesc> VertexData::vertexAttributes {
        {"POSITION", vireo::AttributeFormat::R32G32B32A32_FLOAT, offsetof(VertexData, position)},
        {"NORMAL", vireo::AttributeFormat::R32G32B32A32_FLOAT, offsetof(VertexData, normal)},
        {"TANGENT", vireo::AttributeFormat::R32G32B32A32_FLOAT, offsetof(VertexData, tangent)},
    };

    MeshSurface::MeshSurface(const uint32 firstIndex, const uint32 count):
        firstIndex{firstIndex},
        indexCount{count} {
    }

    Mesh::Mesh(Context& ctx,
               const std::vector<Vertex>& vertices,
               const std::vector<uint32>& indices,
               const std::vector<MeshSurface> &surfaces):
        ctx(ctx),
        vertices{vertices},
        indices{indices},
        surfaces{surfaces} {
        buildAABB();
    }

    void Mesh::setSurfaceMaterial(const uint32 surfaceIndex, const unique_id material) {
        assert([&]{return surfaceIndex < surfaces.size();}, "Invalid surface index");
        surfaces[surfaceIndex].material = material;
        materials.insert(surfaces[surfaceIndex].material);
        ctx.res.get<MeshManager>().upload(*this);
    }

    bool Mesh::operator==(const Mesh &other) const {
        return vertices == other.vertices &&
               indices == other.indices &&
               surfaces == other.surfaces &&
               materials == other.materials;
    }

    void Mesh::buildAABB() {
        auto min = float3{std::numeric_limits<float>::max()};
        auto max = float3{std::numeric_limits<float>::lowest()};
        for (const auto index : indices) {
            const auto& position = vertices[index].position;
            //Get the smallest vertex
            min.x = std::min(min.x, position.x);    // Find smallest x value in model
            min.y = std::min(min.y, position.y);    // Find smallest y value in model
            min.z = std::min(min.z, position.z);    // Find smallest z value in model
            //Get the largest vertex
            max.x = std::max(max.x, position.x);    // Find largest x value in model
            max.y = std::max(max.y, position.y);    // Find largest y value in model
            max.z = std::max(max.z, position.z);    // Find largest z value in model
        }
        localAABB = {min, max};
    }

    MeshManager::MeshManager(
        Context& ctx,
        const size_t capacity,
        const size_t vertexCapacity,
        const size_t indexCapacity,
        const size_t surfaceCapacity) :
        ResourcesManager(ctx, capacity),
        materialManager(ctx.res.get<MaterialManager>()),
        vertexArray {
            ctx.vireo,
            sizeof(VertexData),
            vertexCapacity,
            vertexCapacity,
            vireo::BufferType::VERTEX,
            "Vertex Array"},
        indexArray {
            ctx.vireo,
            sizeof(uint32),
            indexCapacity,
            indexCapacity,
            vireo::BufferType::INDEX,
            "Index Array"},
        meshSurfaceArray {
            ctx.vireo,
            sizeof(MeshSurfaceData),
            surfaceCapacity,
            surfaceCapacity,
            vireo::BufferType::DEVICE_STORAGE,
            "MeshSurface Array"} {
        ctx.res.enroll(*this);
    }

      void MeshManager::upload(const Mesh& mesh) {
        // if (mesh.bypassUpload) { return; }
        needUpload.insert(mesh.id);
    }

    void MeshManager::flush() {
        if (needUpload.empty()) return;
        for (const auto id : needUpload) {
            auto& mesh = get(id);
            if (!mesh.isUploaded()) {
                mesh.verticesMemoryBlock = vertexArray.alloc(mesh.vertices.size());
                mesh.indicesMemoryBlock = indexArray.alloc(mesh.indices.size());
                mesh.surfacesMemoryBlock = meshSurfaceArray.alloc(mesh.surfaces.size());
            }

            auto lock = std::unique_lock(mutex);

            // Uploading all vertices
            auto vertexData = std::vector<VertexData>(mesh.vertices.size());
            for (int i = 0; i < mesh.vertices.size(); i++) {
                const auto& v = mesh.vertices[i];
                vertexData[i].position = float4(v.position.x, v.position.y, v.position.z, v.uv.x);
                vertexData[i].normal = float4(v.normal.x, v.normal.y, v.normal.z, v.uv.y);
                vertexData[i].tangent = v.tangent;
            }
            vertexArray.write(mesh.verticesMemoryBlock, vertexData.data());

            // Uploading all indices
            indexArray.write(mesh.indicesMemoryBlock, mesh.indices.data());

            // Uploading all surfaces & materials
            auto surfaceData = std::vector<MeshSurfaceData>(mesh.surfaces.size());
            for (int i = 0; i < mesh.surfaces.size(); i++) {
                const auto& surface = mesh.surfaces[i];
                const auto& material = materialManager[surface.material];
                if (!material.isUploaded()) {
                    material.upload();
                }
                surfaceData[i].indexCount = surface.indexCount;
                surfaceData[i].indicesIndex = mesh.indicesMemoryBlock.instanceIndex + surface.firstIndex;
                surfaceData[i].verticesIndex = mesh.verticesMemoryBlock.instanceIndex;
            }
            meshSurfaceArray.write(mesh.surfacesMemoryBlock, surfaceData.data());
        }

        auto lock = std::unique_lock(mutex, std::try_to_lock);
        const auto command = ctx.asyncQueue.beginCommand(vireo::CommandType::TRANSFER);
        vertexArray.flush(*command.commandList);
        indexArray.flush(*command.commandList);
        meshSurfaceArray.flush(*command.commandList);
        ctx.asyncQueue.endCommand(command);
    }

#ifdef LUA_BINDING
    Mesh& MeshManager::create( const luabridge::LuaRef& vertices,
          const luabridge::LuaRef& indices,
          const luabridge::LuaRef&surfaces) {
        if (!vertices.isTable() || !indices.isTable() || !surfaces.isTable()) {
            throw Exception("Expected tables for vertices, indices, surfaces");
        }
        std::vector<Vertex> vVec;
        const auto vLen = vertices.length();
        vVec.reserve(vLen);
        for (int i = 1; i <= vLen; ++i) {
            const auto vref = vertices[i];
            Vertex v{};
            if (vref.isTable()) {
                const auto pos = vref["position"]; if (pos.isInstance<float3>()) v.position = pos.cast<float3>().value();
                const auto nrm = vref["normal"];   if (nrm.isInstance<float3>()) v.normal   = nrm.cast<float3>().value();
                const auto uv  = vref["uv"];       if (uv.isInstance<float2>())  v.uv       = uv.cast<float2>().value();
                const auto tan = vref["tangent"];  if (tan.isInstance<float4>()) v.tangent  = tan.cast<float4>().value();
            } else if (vref.isInstance<Vertex>()) {
                v = vref.cast<Vertex>().value();
            } else {
                throw Exception("Invalid vertex entry at index ", i);
            }
            vVec.emplace_back(v);
        }

        std::vector<uint32> iVec;
        const auto iLen = indices.length();
        iVec.reserve(iLen);
        for (int i = 1; i <= iLen; ++i) {
            const auto iref = indices[i];
            if (!iref.isNumber()) {
                throw Exception("Invalid index entry at index ", i);
            }
            const auto val = static_cast<uint32>(iref.cast<unsigned int>().value());
            iVec.emplace_back(val);
        }

        std::vector<MeshSurface> sVec;
        const auto sLen = surfaces.length();
        sVec.reserve(sLen);
        for (int i = 1; i <= sLen; ++i) {
            const auto sref = surfaces[i];
            if (sref.isInstance<MeshSurface>()) {
                sVec.emplace_back(sref.cast<MeshSurface>().value());
                continue;
            }

            if (sref.isTable()) {
                uint32 firstIndex = 0;
                uint32 indexCount = 0;
                unique_id material = 0;

                const auto fi = sref["firstIndex"]; if (fi.isNumber()) firstIndex = static_cast<uint32>(fi.cast<unsigned int>().value());
                const auto ic = sref["indexCount"]; if (ic.isNumber()) indexCount = static_cast<uint32>(ic.cast<unsigned int>().value());
                const auto mat = sref["material"];   if (mat.isNumber()) material  = static_cast<unique_id>(mat.cast<unsigned long long>().value());

                MeshSurface ms{firstIndex, indexCount};
                ms.material = material;
                sVec.emplace_back(ms);
                continue;
            }

            throw Exception("Invalid surface entry at index ", i);
        }

        return create(vVec, iVec, sVec);
    }
#endif

}
