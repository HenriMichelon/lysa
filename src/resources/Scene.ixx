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
     * @brief Configuration settings for a Scene.
     */
    struct SceneConfiguration {
        /** @brief Number of nodes updates per frame for asynchronous scene updates. */
        uint32 asyncObjectUpdatesPerFrame{50};
        /** @brief Maximum number of lights per scene. */
        size_t maxLights{10};
        /** @brief Maximum number of mesh instances per frame per scene. */
        size_t maxMeshInstances{10000};
        /** @brief Maximum number of mesh surfaces instances per pipeline. */
        size_t maxMeshSurfacePerPipeline{100000};
    };

    /**
     * @brief Represents a 3D scene containing lights and mesh instances.
     *
     * The Scene class manages the high-level representation of a scene, including
     * its environment, lights, and mesh instances. It handles deferred operations
     * for adding/removing instances across multiple frames.
     */
    class Scene : public UniqueResource {
    public:
        /**
         * @brief Constructs a Scene with a given configuration.
         * @param ctx Reference to the rendering context.
         * @param config Scene configuration settings.
         */
        Scene(Context& ctx,
              const SceneConfiguration& config = {});

        /** @brief Virtual destructor for Scene. */
        ~Scene() override;

        /**
         * @brief Sets the environment for the scene.
         * @param environment The environment settings to apply.
         */
        void setEnvironment(const Environment& environment);

        /**
         * @brief Checks if a mesh instance is present in the scene.
         * @param meshInstance The mesh instance to check.
         * @return True if the instance is in the scene, false otherwise.
         */
        bool haveInstance(const MeshInstance& meshInstance) const;

        /**
         * @brief Adds a mesh instance to the scene.
         * @param meshInstance The mesh instance to add.
         * @param async Whether to add the instance asynchronously.
         */
        void addInstance(const MeshInstance& meshInstance, bool async = false);

        /**
         * @brief Updates an existing mesh instance.
         * @param meshInstance The mesh instance to update.
         */
        void updateInstance(const MeshInstance& meshInstance);

        /**
         * @brief Removes a mesh instance from the scene.
         * @param meshInstance The mesh instance to remove.
         * @param async Whether to remove the instance asynchronously.
         */
        void removeInstance(const MeshInstance& meshInstance, bool async = false);

        /**
         * @brief Adds a light to the scene.
         * @param light The light to add.
         */
        void addLight(const Light& light);

        /**
         * @brief Removes a light from the scene.
         * @param light The light to remove.
         */
        void removeLight(const Light& light);

        /**
         * @brief Processes deferred scene operations for a specific frame.
         * @param frameIndex The index of the frame to process.
         */
        void processDeferredOperations(uint32 frameIndex);

        /**
         * @brief Gets the frame-specific data for a given frame index.
         * @param frameIndex The index of the frame.
         * @return A reference to the SceneFrameData for the frame.
         */
        SceneFrameData& get(const uint32 frameIndex) const { return *framesData[frameIndex].scene; }

    protected:
        /** @brief Reference to the engine context. */
        Context& ctx;
        /** @brief Reference to the image manager. */
        ImageManager& imageManager;
        /** @brief Reference to the material manager. */
        MaterialManager& materialManager;
        /** @brief Reference to the mesh manager. */
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

