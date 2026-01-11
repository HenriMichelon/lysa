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
import lysa.resources;
import lysa.resources.manager;
import lysa.resources.material;
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
    class MeshInstance : public UniqueResource {
    public:
        MeshInstance(const MeshInstance& orig);

        MeshInstance(const Context& ctx, const Mesh& mesh, const std::string& name = "");

        MeshInstance(const MeshInstance& mi, const std::string& name);

        const Mesh& getMesh() const { return mesh; }

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

        MeshInstanceData getData() const;

        ~MeshInstance() override;

    private:
        MaterialManager& materialManager;
        MeshManager& meshManager;
        const Mesh& mesh;
        const std::string name;
        bool visible{true};
        bool castShadows{false};
        AABB worldAABB{};
        float4x4 worldTransform{float4x4::identity()};
        std::unordered_map<uint32, unique_id> materialsOverride;
    };

}

