/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
module lysa.resources.mesh_instance;

namespace lysa {

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
