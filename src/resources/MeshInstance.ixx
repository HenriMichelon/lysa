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
    class MeshInstance : public ManagedResource {
    public:
        MeshInstance(Context& ctx, unique_id meshId);

        MeshInstance(
           Context& ctx,
           unique_id meshId,
           bool visible,
           bool castShadows,
           const AABB& worldAABB,
           const float4x4& worldTransform,
           const std::string& name = "");

        MeshInstance(const Context& ctx, const MeshInstance& mi, const std::string& name = "") ;

        Mesh& getMesh() const { return mesh; }

        bool isVisible() const { return visible; }

        void setVisible(const bool visible) { this->visible = visible; }

        bool isCastShadows() const { return castShadows; }

        void setCastShadow(const bool castShadows) { this->castShadows = castShadows; }

        const AABB& getAABB() const { return worldAABB; }

        void setAABB(const AABB& aabb) { worldAABB = aabb; }

        const float4x4& getTransform() const { return worldTransform; }

        void setTransform(const float4x4& transform) { worldTransform = transform; }

        unique_id getSurfaceMaterial(uint32 surfaceIndex) const;

        void setSurfaceMaterialOverride(const uint32 surfaceIndex, const unique_id materialId) {
            materialsOverride[surfaceIndex] = materialId;
        }

        void removeSurfaceMaterialOverride(const uint32 surfaceIndex) {
            materialsOverride.erase(surfaceIndex);
        }

        uint32 getPendingUpdates() const { return pendingUpdates; }

        void setPendingUpdates(const uint32 updates) { pendingUpdates = updates; }

        MeshInstanceData getData() const;

        ~MeshInstance() override;

    private:
        MeshManager& meshManager;
        const std::string name;
        Mesh& mesh;
        bool visible{true};
        bool castShadows{false};
        AABB worldAABB{};
        float4x4 worldTransform{float4x4::identity()};
        std::unordered_map<uint32, unique_id> materialsOverride;
        /** Current number of pending updates to process. */
        uint32 pendingUpdates{0};
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

