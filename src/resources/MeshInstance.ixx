/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.resources.mesh_instance;

import vireo;
import lysa.aabb;
import lysa.context;
import lysa.math;
import lysa.resources.manager;
import lysa.resources.mesh;

export namespace lysa {

    /**
     * Mesh instance data in GPU memory
     */
    struct MeshInstanceData {
        float4x4 transform;
        float3   aabbMin;
        float3   aabbMax;
        uint     visible;
        uint     castShadows;
    };

    /**
    * Scene node that holds a Mesh.
    */
    struct MeshInstance  : Resource {
        const std::string name;
        Mesh& mesh;
        bool visible{false};
        bool castShadows{false};
        AABB worldAABB;
        float4x4 worldTransform;
        std::unordered_map<uint32, unique_id> materialsOverride;
        /** Current number of pending updates to process. */
        uint32 pendingUpdates{0};

        unique_id getSurfaceMaterial(uint32 surfaceIndex) const;

        MeshInstanceData getData() const;

        MeshInstance(
            const Context& ctx,
            const unique_id meshId,
            const bool visible,
            const bool castShadows,
            const AABB& worldAABB,
            const float4x4& worldTransform,
            const std::string& name = "") :
            name(name),
            mesh(ctx.res.get<MeshManager>()[meshId]),
            visible(visible),
            castShadows(castShadows),
            worldAABB(worldAABB),
            worldTransform(worldTransform) {
            ctx.res.get<MeshManager>().use(mesh.id);
        }

        MeshInstance(const Context& ctx, const MeshInstance& mi, const std::string& name = "") :
            name(name),
            mesh(mi.mesh),
            visible(mi.visible),
            castShadows(mi.castShadows),
            worldAABB(mi.worldAABB),
            worldTransform(mi.worldTransform) {
            ctx.res.get<MeshManager>().use(mesh.id);
        }
    };

    class MeshInstanceManager : public ResourcesManager<Context, MeshInstance> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        MeshInstanceManager(Context& ctx, const unique_id capacity) :
            ResourcesManager(ctx, capacity, "MeshInstanceManager") {
            ctx.res.enroll(*this);
        }
    };
}

