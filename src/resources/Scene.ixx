/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.scene;

import lysa.context;
import lysa.math;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.scene_frame_data;
import lysa.resources;
import lysa.resources.manager;
import lysa.resources.environment;
import lysa.resources.image;
import lysa.resources.light;
import lysa.resources.material;
import lysa.resources.mesh;
import lysa.resources.mesh_instance;
import lysa.resources.texture;

export namespace lysa {

    /**
     * Configuration settings for a Scene.
     */
    struct SceneConfiguration {
        /** Number of nodes updates per frame for asynchronous scene updates. */
        uint32 asyncObjectUpdatesPerFrame{50};
        /** Maximum number of lights per scene. */
        size_t maxLights{10};
        /** Maximum number of mesh instances per frame per scene. */
        size_t maxMeshInstances{10000};
        /** Maximum number of mesh surfaces instances per pipeline. */
        size_t maxMeshSurfacePerPipeline{100000};
    };

    /**
     * Represents a 3D scene containing lights and mesh instances.
     *
     * The Scene class manages the high-level representation of a scene, including
     * its environment, lights, and mesh instances. It handles deferred operations
     * for adding/removing instances across multiple frames.
     */
    class Scene : public UniqueResource {
    public:
        /**
         * Constructs a Scene with a given configuration.
         * @param ctx Reference to the rendering context.
         * @param config Scene configuration settings.
         */
        Scene(Context& ctx,
              const SceneConfiguration& config = {});

        /** Virtual destructor for Scene. */
        ~Scene() override;

        /**
         * Sets the environment for the scene.
         * @param environment The environment settings to apply.
         */
        void setEnvironment(const Environment& environment);

        /**
         * Checks if a mesh instance is present in the scene.
         * @param meshInstance The mesh instance to check.
         * @return True if the instance is in the scene, false otherwise.
         */
        bool haveInstance(const MeshInstance& meshInstance) const;

        /**
         * Adds a mesh instance to the scene.
         * @param meshInstance The mesh instance to add.
         * @param async Whether to add the instance asynchronously.
         */
        void addInstance(const MeshInstance& meshInstance, bool async = false);

        /**
         * Updates an existing mesh instance.
         * @param meshInstance The mesh instance to update.
         */
        void updateInstance(const MeshInstance& meshInstance);

        /**
         * Removes a mesh instance from the scene.
         * @param meshInstance The mesh instance to remove.
         * @param async Whether to remove the instance asynchronously.
         */
        void removeInstance(const MeshInstance& meshInstance, bool async = false);

        /**
         * Adds a light to the scene.
         * @param light The light to add.
         */
        void addLight(const Light& light);

        /**
         * Removes a light from the scene.
         * @param light The light to remove.
         */
        void removeLight(const Light& light);

        /**
         * Processes deferred scene operations for a specific frame.
         * @param frameIndex The index of the frame to process.
         */
        void processDeferredOperations(uint32 frameIndex);

        /**
         * Gets the frame-specific data for a given frame index.
         * @param frameIndex The index of the frame.
         * @return A reference to the SceneFrameData for the frame.
         */
        SceneFrameData& get(const uint32 frameIndex) const { return *framesData[frameIndex].scene; }

    protected:
        /** Reference to the engine context. */
        Context& ctx;
        /** Reference to the image manager. */
        ImageManager& imageManager;
        /** Reference to the material manager. */
        MaterialManager& materialManager;
        /** Reference to the mesh manager. */
        MeshManager& meshManager;

    private:
        /*Per-frame state and deferred operations processed at frame boundaries.*/
        struct FrameData {
            /* Nodes to add on the next frame (synchronous path). */
            std::unordered_set<const MeshInstance*> addedNodes;
            /* Nodes to add on the next frame (async path). */
            std::unordered_set<const MeshInstance*> addedNodesAsync;
            /* Nodes to remove on the next frame (synchronous path). */
            std::unordered_set<const MeshInstance*> removedNodes;
            /* Nodes to remove on the next frame (async path). */
            std::unordered_set<const MeshInstance*> removedNodesAsync;
            /* Scene instance associated with this frame. */
            std::unique_ptr<SceneFrameData> scene;
        };
        /* Maximum number of nodes to update asynchronously per frame. */
        const uint32 maxAsyncNodesUpdatedPerFrame;
        /* Vector of per-frame data. */
        std::vector<FrameData> framesData;
        /* Mutex to guard access to frame data. */
        std::mutex frameDataMutex;
        /* Set of all mesh instances currently in the scene. */
        std::unordered_set<const MeshInstance*> meshInstances;
        /* Set of nodes that have been updated and need synchronization. */
        std::unordered_set<const MeshInstance*> updatedNodes;
    };

}

