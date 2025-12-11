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
        const uint32 maxAsyncNodesUpdatedPerFrame,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 framesInFlight,
        uint32 maxShadowMaps) :
        framesInFlight(framesInFlight),
        maxAsyncNodesUpdatedPerFrame(maxAsyncNodesUpdatedPerFrame) {
        framesData.resize(framesInFlight);
        for (auto& data : framesData) {
            data.scene =std::make_unique<SceneRenderContext>(
                ctx,
                maxLights,
                maxMeshInstancesPerScene,
                maxMeshSurfacePerPipeline,
                framesInFlight,
                maxShadowMaps);
        }
    }

    void Scene::addInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance, const bool async) {
        assert([&]{return meshInstance != nullptr;}, "meshInstance can't be null");
        meshInstance->pendingUpdates = framesInFlight;
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.addedNodesAsync.push_back(meshInstance);
            } else {
                frame.addedNodes.push_back(meshInstance);
            }
        }
    }

    void Scene::updateInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance) {
        assert([&]{return meshInstance != nullptr;}, "meshInstance can't be null");
        meshInstance->pendingUpdates = framesInFlight;
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            frame.updatedNodes.push_back(meshInstance);
        }
    }

    void Scene::removeInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance, const bool async) {
        assert([&]{return meshInstance != nullptr;}, "meshInstance can't be null");
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.removedNodesAsync.push_back(meshInstance);
            } else {
                frame.removedNodes.push_back(meshInstance);
            }
        }
    }

    void Scene::processDeferredOperations(const uint32 frameIndex) {
        auto lock = std::lock_guard(frameDataMutex);
        auto &data = framesData[frameIndex];
        // Remove from the renderer the nodes previously removed from the scene tree
        // Immediate removes
        if (!data.removedNodes.empty()) {
            for (const auto &node : data.removedNodes) {
                data.scene->removeInstance(std::get<std::shared_ptr<MeshInstanceDesc>>(node));
            }
            data.removedNodes.clear();
        }
        // Async removes
        if (!data.removedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.removedNodesAsync.begin(); it != data.removedNodesAsync.end();) {
                data.scene->removeInstance(std::get<std::shared_ptr<MeshInstanceDesc>>(*it));
                it = data.removedNodesAsync.erase(it);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        // Add to the scene the nodes previously added to the scene tree
        // Immediate additions
        if (!data.addedNodes.empty()) {
            for (const auto &node : data.addedNodes) {
                data.scene->addInstance(std::get<std::shared_ptr<MeshInstanceDesc>>(node));
            }
            data.addedNodes.clear();
        }
        // Async additions
        if (!data.addedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.addedNodesAsync.begin(); it != data.addedNodesAsync.end();) {
                data.scene->addInstance(std::get<std::shared_ptr<MeshInstanceDesc>>(*it));
                it = data.addedNodesAsync.erase(it);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
    }

    SceneManager::SceneManager(
        Context& ctx,
        const size_t capacity,
        const uint32 maxAsyncNodesUpdatedPerFrame,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 maxShadowMaps,
        const uint32 framesInFlight) :
        ResourcesManager(ctx, capacity),
        maxAsyncNodesUpdatedPerFrame(maxAsyncNodesUpdatedPerFrame),
        maxLights(maxLights),
        maxMeshInstancesPerScene(maxMeshInstancesPerScene),
        maxMeshSurfacePerPipeline(maxMeshSurfacePerPipeline),
        maxShadowMaps(maxShadowMaps),
        framesInFlight(framesInFlight){
            ctx.res.enroll(*this);
    }

    Scene& SceneManager::create() {
        return ResourcesManager::create(ctx,
            maxAsyncNodesUpdatedPerFrame,
            maxLights,
            maxMeshInstancesPerScene,
            maxMeshSurfacePerPipeline,
            framesInFlight,
            maxShadowMaps);
    }

}