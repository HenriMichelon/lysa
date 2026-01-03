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
import lysa.resources.material;
import lysa.resources.mesh;
import lysa.resources.mesh_instance;
import lysa.resources.texture;

export namespace lysa {

    struct SceneConfiguration {
        //! Number of nodes updates per frame for asynchronous scene updates
        uint32 asyncObjectUpdatesPerFrame{50};
        //! Maximum number of lights per scene
        size_t maxLights{10};
        //! Maximum number of mesh instances per frame per scene
        size_t maxMeshInstances{10000};
        //! Maximum number of mesh surfaces instances per pipeline
        size_t maxMeshSurfacePerPipeline{100000};
    };

    class Scene : public UniqueResource {
    public:
        /**
         * Constructs a Scene for a given configuration
         */
        Scene(Context& ctx,
              const SceneConfiguration& config = {});

        ~Scene() override;

        void setEnvironment(const Environment& environmentId);

        const Environment& getEnvironment() const { return environment; }

        /** Adds a mesh instance to the scene. */
        void addInstance(const std::shared_ptr<MeshInstance>& meshInstance, bool async = false);

        /** Updates a mesh instance. */
        void updateInstance(const std::shared_ptr<MeshInstance>& meshInstance);

        /** Removes a node previously added to the scene. */
        void removeInstance(const std::shared_ptr<MeshInstance>& meshInstance, bool async = false);

        void processDeferredOperations(uint32 frameIndex);

        SceneFrameData& get(const uint32 frameIndex) const { return *framesData[frameIndex].scene; }

    protected:
        Context& ctx;
        ImageManager& imageManager;
        MaterialManager& materialManager;
        MeshManager& meshManager;

    private:
        /** Per‑frame state and deferred operations processed at frame boundaries. */
        struct FrameData {
            /** Nodes to add on the next frame (synchronous path). */
            std::list<std::shared_ptr<MeshInstance>> addedNodes;
            /** Nodes to add on the next frame (async path). */
            std::list<std::shared_ptr<MeshInstance>> addedNodesAsync;
            /** Nodes to remove on the next frame (synchronous path). */
            std::list<std::shared_ptr<MeshInstance>> removedNodes;
            /** Nodes to remove on the next frame (async path). */
            std::list<std::shared_ptr<MeshInstance>> removedNodesAsync;
            /** Scene instance associated with this frame. */
            std::unique_ptr<SceneFrameData> scene;
        };
        const uint32 maxAsyncNodesUpdatedPerFrame;
        std::vector<FrameData> framesData;
        std::mutex frameDataMutex;
        Environment environment;
        std::list<std::shared_ptr<MeshInstance>> meshInstances;
        std::list<std::shared_ptr<MeshInstance>> updatedNodes;
    };

}

