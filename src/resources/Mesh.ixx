/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.resources.mesh;

import vireo;
import lysa.aabb;
import lysa.context;
import lysa.exception;
import lysa.math;
import lysa.memory;
import lysa.resources.material;
import lysa.resources.resource_manager;

export namespace lysa {

    struct VertexData {
        float4 position; // position + uv.x
        float4 normal;   // normal + uv.y
        float4 tangent;  // tangent + sign

        static const std::vector<vireo::VertexAttributeDesc> vertexAttributes;
    };


    /**
     * %A Mesh vertex
     */
    struct Vertex {
        //! local position
        float3 position;
        //! surface normal
        float3 normal;
        //! UV coordinates in the surface
        float2 uv;
        //! UV-based tangents
        float4 tangent;

        inline bool operator==(const Vertex &other) const {
            return all(position == other.position) &&
                    all(normal == other.normal) &&
                    all(uv == other.uv) &&
                    all(tangent == other.tangent);
        }
    };

    struct MeshSurfaceData {
        uint32 indexCount;
        uint32 indicesIndex;
        uint32 verticesIndex;
    };

    /**
     * %A Mesh surface, with counterclockwise triangles
     */
    struct MeshSurface {
        //! Index of the first vertex of the surface
        uint32 firstIndex{0};
        //! Number of vertices
        uint32 indexCount{0};
        //! Material
        unique_id material{INVALID_ID};

        MeshSurface(uint32 firstIndex, uint32 count);

        inline bool operator==(const MeshSurface &other) const {
            return firstIndex == other.firstIndex && indexCount == other.indexCount && material == other.material;
        }

        friend inline bool operator==(const std::shared_ptr<MeshSurface>& a, const std::shared_ptr<MeshSurface>& b) {
            return *a == *b;
        }
    };

    /**
     * %A mesh composed by multiple Surface and an indexes collection of Vertex
     */
    class Mesh : public Resource {
    public:
        /**
         * Creates a Mesh from vertices
         * @param ctx
         * @param vertices Vertices
         * @param indices Indexes of vertices
         * @param surfaces Surfaces
         */
        Mesh(Context& ctx,
             const std::vector<Vertex>& vertices,
             const std::vector<uint32>& indices,
             const std::vector<MeshSurface>&surfaces);

        Mesh(Context& ctx) : ctx(ctx) {}

         /**
         * Returns the material for a given surface
         * @param surfaceIndex Zero-based index of the surface
         */
        unique_id getSurfaceMaterial(const uint32 surfaceIndex) const {
            assert([&]{return surfaceIndex < surfaces.size(); }, "Invalid surface index");
            return surfaces[surfaceIndex].material;
        }

        /**
         * Changes the material of a given surface.
         * Warning: this will update the data in memory only but not the node in the scene.
         * If the material needs a different pipeline the change will not appear.
         * Use MeshInstance::setSurfaceMaterial instead.
         * @param surfaceIndex Zero-based index of the Surface
         * @param material New material for the Surface
         */
        void setSurfaceMaterial(uint32 surfaceIndex, unique_id material);

        /**
         * Returns all the Surfaces
         */
        std::vector<MeshSurface>& getSurfaces() { return surfaces; }

        /**
         * Returns all the Surfaces
         */
        const std::vector<MeshSurface>& getSurfaces() const { return surfaces; }

        /**
         * Returns all the vertices
         */
        std::vector<Vertex>& getVertices() { return vertices; }

        /**
         * Return all the vertices indexes
         */
        std::vector<uint32>& getIndices() { return indices; }

        /**
         * Returns all the vertices
         */
        const std::vector<Vertex>& getVertices() const { return vertices; }

        /**
         * Return all the vertices indexes
         */
        const std::vector<uint32>& getIndices() const { return indices; }

        /**
         * Returns the local space axis aligned bounding box
         */
        const AABB& getAABB() const { return localAABB; }

        bool operator==(const Mesh &other) const;

        auto getVerticesIndex() const { return verticesMemoryBlock.instanceIndex; }

        auto getIndicesIndex() const { return indicesMemoryBlock.instanceIndex; }

        auto getSurfacesIndex() const { return surfacesMemoryBlock.instanceIndex; }

        auto& getMaterials() { return materials; }

        auto isUploaded() const { return verticesMemoryBlock.size > 0; }

        void buildAABB();

    protected:
        Context& ctx;
        AABB localAABB;
        std::vector<Vertex> vertices;
        std::vector<uint32> indices;

        std::vector<MeshSurface> surfaces{};
        std::unordered_set<unique_id> materials{};

    private:
        friend class MeshManager;
        MemoryBlock verticesMemoryBlock;
        MemoryBlock indicesMemoryBlock;
        MemoryBlock surfacesMemoryBlock;
    };


    class MeshManager : public ResourcesManager<Mesh> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        MeshManager(
            Context& ctx,
            size_t capacity,
            size_t vertexCapacity,
            size_t indexCapacity,
            size_t surfaceCapacity);

        ~MeshManager() override { cleanup(); }

        Mesh& create(const std::vector<Vertex>& vertices,
             const std::vector<uint32>& indices,
             const std::vector<MeshSurface>&surfaces) {
            return allocate(std::make_unique<Mesh>(ctx, vertices, indices, surfaces));
        }

        Mesh& create() {
            return allocate(std::make_unique<Mesh>(ctx));
        }

        void upload(unique_id mesh_id);

        void flush();

        //auto getBuffer() const { return memoryArray.getBuffer(); }

    private:
        MaterialManager& materialManager;
        /** Device memory array that stores all vertex buffers. */
        DeviceMemoryArray vertexArray;
        /** Device memory array that stores all index buffers. */
        DeviceMemoryArray indexArray;
        /** Device memory array that stores mesh surface descriptors. */
        DeviceMemoryArray meshSurfaceArray;
        /** Mutex to guard mutations to memory array. */
        std::mutex mutex;
        bool updated{false};
    };
}

