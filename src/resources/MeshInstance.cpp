/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
module lysa.resources.mesh_instance;

namespace lysa {

    MeshInstance::MeshInstance(Context& ctx, const unique_id meshId) :
        meshManager(ctx.res.get<MeshManager>()),
        name(name),
        mesh(meshManager[meshId]){

    }

    MeshInstance::MeshInstance(
           Context& ctx,
           const unique_id meshId,
           const bool visible,
           const bool castShadows,
           const AABB& worldAABB,
           const float4x4& worldTransform,
           const std::string& name) :
           meshManager(ctx.res.get<MeshManager>()),
           name(name),
           mesh(meshManager[meshId]),
           visible(visible),
           castShadows(castShadows),
           worldAABB(worldAABB),
           worldTransform(worldTransform) {
        meshManager.use(mesh.id);
    }

    MeshInstance::MeshInstance(const Context&, const MeshInstance& mi, const std::string& name) :
        meshManager(meshManager),
        name(name),
        mesh(mi.mesh),
        visible(mi.visible),
        castShadows(mi.castShadows),
        worldAABB(mi.worldAABB),
        worldTransform(mi.worldTransform) {
        meshManager.use(mesh.id);
    }

    MeshInstance::~MeshInstance() {
        meshManager.destroy(mesh.id);
    }

    unique_id MeshInstance::getSurfaceMaterial(const uint32 surfaceIndex) const {
        if (materialsOverride.contains(surfaceIndex)) {
            return materialsOverride.at(surfaceIndex);
        }
        return mesh.getSurfaces()[surfaceIndex].material;
    }

    MeshInstanceData MeshInstance::getData() const {
        return {
            .transform = worldTransform,
            .aabbMin = worldAABB.min,
            .aabbMax = worldAABB.max,
            .visible = visible ? 1u : 0u,
            .castShadows = castShadows ? 1u : 0u,
        };
    }

}
