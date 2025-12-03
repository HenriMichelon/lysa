/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <cstddef>
module lysa.resources.mesh;

import lysa.log;

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
        ctx.resources.get<MeshManager>().upload(id);
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
        materialManager(ctx.resources.get<MaterialManager>()),
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
        ctx.resources.enroll(*this);
    }

    void MeshManager::flush() {
        if (updated) {
            auto lock = std::unique_lock(mutex, std::try_to_lock);
            const auto command = ctx.asyncQueue.beginCommand(vireo::CommandType::TRANSFER);
            vertexArray.flush(*command.commandList);
            indexArray.flush(*command.commandList);
            meshSurfaceArray.flush(*command.commandList);
            ctx.asyncQueue.endCommand(command);
        }
    }

    void MeshManager::upload(const unique_id mesh_id) {
        auto& mesh = get(mesh_id);
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
        updated = true;
    }

}
