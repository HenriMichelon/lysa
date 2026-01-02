/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.scene;

import lysa.exception;
import lysa.log;
import lysa.resources.environment;

namespace lysa {

    Scene::Scene(
        const Context& ctx,
        const uint32 maxAsyncNodesUpdatedPerFrame,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 framesInFlight,
        uint32 maxShadowMaps) :
        ctx(ctx),
        meshInstanceManager(ctx.res.get<MeshInstanceManager>()),
        framesInFlight(framesInFlight),
        maxAsyncNodesUpdatedPerFrame(maxAsyncNodesUpdatedPerFrame) {
        framesData.resize(framesInFlight);
        for (auto& data : framesData) {
            data.scene =std::make_unique<SceneFrameData>(
                ctx,
                maxLights,
                maxMeshInstancesPerScene,
                maxMeshSurfacePerPipeline,
                framesInFlight,
                maxShadowMaps);
        }
    }

    Scene::~Scene() {
        ctx.graphicQueue->waitIdle();
        if (environment != INVALID_ID) {
            ctx.res.get<EnvironmentManager>().destroy(environment);
        }
        for (const auto meshInstance : meshInstances) {
            meshInstanceManager.destroy(meshInstance);
        }
    }

    void Scene::setEnvironment(const unique_id environmentId) {
        environment = environmentId;
        ctx.res.get<EnvironmentManager>().use(environment);
    }

    void Scene::addInstance(const unique_id meshInstance, const bool async) {
        assert([&]{return meshInstance != INVALID_ID;}, "Invalid meshInstance");
        meshInstanceManager.use(meshInstance);
        meshInstanceManager[meshInstance].setPendingUpdates(framesInFlight);
        meshInstances.push_back(meshInstance);
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.addedNodesAsync.push_back(meshInstance);
            } else {
                frame.addedNodes.push_back(meshInstance);
            }
        }
    }

    void Scene::updateInstance(const unique_id meshInstance) {
        assert([&]{return meshInstance != INVALID_ID;}, "Invalid meshInstance");
        meshInstanceManager[meshInstance].setPendingUpdates(framesInFlight);
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            frame.updatedNodes.push_back(meshInstance);
        }
    }

    void Scene::removeInstance(const unique_id meshInstance, const bool async) {
        assert([&]{return meshInstance != INVALID_ID;}, "Invalid meshInstance");
        meshInstances.remove(meshInstance);
        meshInstanceManager.destroy(meshInstance);
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
        assert([&]{ return environment != INVALID_ID; }, "Scene environment not set");
        auto lock = std::lock_guard(frameDataMutex);
        auto &data = framesData[frameIndex];
        data.scene->setEnvironment(environment);
        // Remove from the renderer the nodes previously removed from the scene tree
        // Immediate removes
        if (!data.removedNodes.empty()) {
            for (const auto &node : data.removedNodes) {
                data.scene->removeInstance(node);
                data.updatedNodes.remove(node);
            }
            data.removedNodes.clear();
        }
        // Async removes
        if (!data.removedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.removedNodesAsync.begin(); it != data.removedNodesAsync.end();) {
                auto& mi = *it;
                data.scene->removeInstance(mi);
                it = data.removedNodesAsync.erase(it);
                data.updatedNodes.remove(mi);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        // Add to the scene the nodes previously added to the scene tree
        // Immediate additions
        // Log::info("pDO", frameIndex);
        if (!data.addedNodes.empty()) {
            for (const auto &node : data.addedNodes) {
                data.scene->addInstance(node);
                data.updatedNodes.remove(node);
            }
            data.addedNodes.clear();
        }
        // Async additions
        if (!data.addedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.addedNodesAsync.begin(); it != data.addedNodesAsync.end();) {
                auto& mi = *it;
                data.scene->addInstance(mi);
                it = data.addedNodesAsync.erase(it);
                data.updatedNodes.remove(mi);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        if (!data.updatedNodes.empty()) {
            for (const auto &node : data.updatedNodes) {
                data.scene->updateInstance(node);
            }
            data.updatedNodes.clear();
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
        ResourcesManager(ctx, capacity, "SceneManager"),
        maxAsyncNodesUpdatedPerFrame(maxAsyncNodesUpdatedPerFrame),
        maxLights(maxLights),
        maxMeshInstancesPerScene(maxMeshInstancesPerScene),
        maxMeshSurfacePerPipeline(maxMeshSurfacePerPipeline),
        maxShadowMaps(maxShadowMaps),
        framesInFlight(framesInFlight){
            ctx.res.enroll(*this);
    }

    Scene& SceneManager::create() {
        return ResourcesManager::create(
            maxAsyncNodesUpdatedPerFrame,
            maxLights,
            maxMeshInstancesPerScene,
            maxMeshSurfacePerPipeline,
            framesInFlight,
            maxShadowMaps);
    }

}