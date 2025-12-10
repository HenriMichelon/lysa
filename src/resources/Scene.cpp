/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.scene_context;

import lysa.exception;

namespace lysa {

    Scene::Scene(
        const Context& ctx,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 framesInFlight,
        uint32 maxShadowMaps) {
        for (auto i = 0; i < framesInFlight; i++) {
            framesData.push_back(std::make_unique<SceneRenderContext>(
                ctx,
                maxLights,
                maxMeshInstancesPerScene,
                maxMeshSurfacePerPipeline,
                framesInFlight,
                maxShadowMaps));
        }
    }

    SceneManager::SceneManager(
        Context& ctx,
        const size_t capacity,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 maxShadowMaps,
        const uint32 framesInFlight) :
        ResourcesManager(ctx, capacity),
        maxLights(maxLights),
        maxMeshInstancesPerScene(maxMeshInstancesPerScene),
        maxMeshSurfacePerPipeline(maxMeshSurfacePerPipeline),
        maxShadowMaps(maxShadowMaps),
        framesInFlight(framesInFlight){
            ctx.res.enroll(*this);
    }

    Scene& SceneManager::create() {
        return ResourcesManager::create(ctx, maxLights, maxMeshInstancesPerScene, maxMeshSurfacePerPipeline, framesInFlight, maxShadowMaps);
    }

}