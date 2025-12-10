/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.scene_context;

import lysa.context;
import lysa.types;
import lysa.renderers.scene_render_context;
import lysa.resources.resource_manager;

export namespace lysa {

    class Scene : public Resource {
    public:
        /**
         * Constructs a Scene for a given configuration
         *
         * @param ctx
         * @param config             Scene high-level configuration (buffers sizes, features).
         * @param framesInFlight     Number of buffered frames.
         * @param maxShadowMaps
         */
        Scene(
            const Context& ctx,
            uint32 maxLights,
            uint32 maxMeshInstancesPerScene,
            uint32 maxMeshSurfacePerPipeline,
            uint32 framesInFlight,
            uint32 maxShadowMaps);

        SceneRenderContext& operator [](const uint32 frameIndex) const { return *framesData[frameIndex]; }

    private:
        std::vector<std::unique_ptr<SceneRenderContext>> framesData;
    };

    class SceneManager : public ResourcesManager<Scene> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         * @param maxShadowMaps
         * @param framesInFlight
         */
        SceneManager(
            Context& ctx,
            size_t capacity,
            uint32 maxLights,
            uint32 maxMeshInstancesPerScene,
            uint32 maxMeshSurfacePerPipeline,
            uint32 maxShadowMaps,
            uint32 framesInFlight);

        ~SceneManager() override {
            cleanup();
        }

        Scene& create();

    private:
        const uint32 maxLights;
        const uint32 maxMeshInstancesPerScene;
        const uint32 maxMeshSurfacePerPipeline;
        const uint32 maxShadowMaps;
        const uint32 framesInFlight;
    };

}

