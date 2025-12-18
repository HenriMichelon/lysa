/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.scene_context;

import lysa.context;
import lysa.math;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.scene_frame_data;
import lysa.resources.resource_manager;

export namespace lysa {

    class SceneContext : public Resource {
    public:
        /**
         * Constructs a Scene for a given configuration
         *
         * @param ctx
         * @param maxAsyncNodesUpdatedPerFrame
         * @param maxLights
         * @param maxMeshInstancesPerScene
         * @param maxMeshSurfacePerPipeline
         * @param framesInFlight     Number of buffered frames.
         * @param maxShadowMaps
         */
        SceneContext(
            const Context& ctx,
            uint32 maxAsyncNodesUpdatedPerFrame,
            uint32 maxLights,
            uint32 maxMeshInstancesPerScene,
            uint32 maxMeshSurfacePerPipeline,
            uint32 framesInFlight,
            uint32 maxShadowMaps);

        ~SceneContext() override;

        void setAmbientLight(const float4& ambientLight) { this->ambientLight = ambientLight; }

        const float4& getAmbientLight() const { return ambientLight; }

        /** Adds a mesh instance to the scene. */
        void addInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance, bool async);

        /** Updates a mesh instance. */
        void updateInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance);

        /** Removes a node previously added to the scene. */
        void removeInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance, bool async);

        void processDeferredOperations(uint32 frameIndex);

        SceneFrameData& operator [](const uint32 frameIndex) const { return *framesData[frameIndex].scene; }

    private:
        /** Per‑frame state and deferred operations processed at frame boundaries. */
        struct FrameData {
            /** Nodes to add on the next frame (synchronous path). */
            std::list<std::variant<std::shared_ptr<MeshInstanceDesc>, std::shared_ptr<LightDesc>>> addedNodes;
            /** Nodes to add on the next frame (async path). */
            std::list<std::variant<std::shared_ptr<MeshInstanceDesc>, std::shared_ptr<LightDesc>>> addedNodesAsync;
            /** Nodes to add on the next frame (synchronous path). */
            std::list<std::variant<std::shared_ptr<MeshInstanceDesc>, std::shared_ptr<LightDesc>>> updatedNodes;
            /** Nodes to remove on the next frame (synchronous path). */
            std::list<std::variant<std::shared_ptr<MeshInstanceDesc>, std::shared_ptr<LightDesc>>> removedNodes;
            /** Nodes to remove on the next frame (async path). */
            std::list<std::variant<std::shared_ptr<MeshInstanceDesc>, std::shared_ptr<LightDesc>>> removedNodesAsync;
            /** Scene instance associated with this frame. */
            std::unique_ptr<SceneFrameData> scene;
        };
        const Context& ctx;
        const uint32 framesInFlight;
        const uint32 maxAsyncNodesUpdatedPerFrame;
        std::vector<FrameData> framesData;
        std::mutex frameDataMutex;
        float4 ambientLight;
    };

    class SceneContextManager : public ResourcesManager<SceneContext> {
    public:
        SceneContextManager(
            Context& ctx,
            size_t capacity,
            uint32 maxAsyncNodesUpdatedPerFrame,
            uint32 maxLights,
            uint32 maxMeshInstancesPerScene,
            uint32 maxMeshSurfacePerPipeline,
            uint32 maxShadowMaps,
            uint32 framesInFlight);

        ~SceneContextManager() override {
            cleanup();
        }

        SceneContext& create();

        uint32 getFramesInFlight() const { return framesInFlight; }

    private:
        const uint32 maxAsyncNodesUpdatedPerFrame;
        const uint32 maxLights;
        const uint32 maxMeshInstancesPerScene;
        const uint32 maxMeshSurfacePerPipeline;
        const uint32 maxShadowMaps;
        const uint32 framesInFlight;
    };

}

