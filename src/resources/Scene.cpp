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
        Context& ctx,
        const SceneConfiguration& config) :
        ctx(ctx),
        imageManager(ctx.res.get<ImageManager>()),
        materialManager(ctx.res.get<MaterialManager>()),
        meshManager(ctx.res.get<MeshManager>()),
        maxAsyncNodesUpdatedPerFrame(maxAsyncNodesUpdatedPerFrame) {
        framesData.resize(ctx.framesInFlight);
        for (auto& data : framesData) {
            data.scene =std::make_unique<SceneFrameData>(
                ctx,
                config.maxLights,
                config.maxMeshInstances,
                config.maxMeshSurfacePerPipeline,
                ctx.framesInFlight,
                ctx.maxShadowMapsPerScene);
        }
    }

    Scene::~Scene() {
        ctx.graphicQueue->waitIdle();
    }

    void Scene::setEnvironment(const Environment& environmentId) {
        environment = environmentId;
        for (const auto& data : framesData) {
            data.scene->setEnvironment(environment);
        }
    }

    void Scene::addInstance(const std::shared_ptr<MeshInstance>& meshInstance, const bool async) {
        assert([&]{return meshInstance != nullptr && !meshInstances.contains(meshInstance);}, "Invalid meshInstance");
        meshInstances.insert(meshInstance);
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.addedNodesAsync.insert(meshInstance);
            } else {
                frame.addedNodes.insert(meshInstance);
            }
        }
    }

    void Scene::updateInstance(const std::shared_ptr<MeshInstance>& meshInstance) {
        assert([&]{return meshInstance != nullptr && meshInstances.contains(meshInstance);}, "Invalid meshInstance");
        updatedNodes.insert(meshInstance);
    }

    void Scene::removeInstance(const std::shared_ptr<MeshInstance>& meshInstance, const bool async) {
        assert([&]{return meshInstance != nullptr && meshInstances.contains(meshInstance);}, "Invalid meshInstance");
        meshInstances.erase(meshInstance);
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.removedNodesAsync.insert(meshInstance);
            } else {
                frame.removedNodes.insert(meshInstance);
            }
        }
    }

    void Scene::processDeferredOperations(const uint32 frameIndex) {
        assert([&]{ return environment.id != INVALID_ID; }, "Scene environment not set");
        auto lock = std::lock_guard(frameDataMutex);
        auto &data = framesData[frameIndex];
        // Remove from the renderer the nodes previously removed from the scene tree
        // Immediate removes
        if (!data.removedNodes.empty()) {
            for (const auto &node : data.removedNodes) {
                data.scene->removeInstance(node);
                updatedNodes.erase(node);
            }
            data.removedNodes.clear();
        }
        // Async removes
        if (!data.removedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.removedNodesAsync.begin(); it != data.removedNodesAsync.end();) {
                auto& mi = *it;
                data.scene->removeInstance(mi);
                updatedNodes.erase(mi);
                it = data.removedNodesAsync.erase(it);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        // Add to the scene the nodes previously added to the scene tree
        // Immediate additions
        if (!data.addedNodes.empty()) {
            for (const auto &node : data.addedNodes) {
                data.scene->addInstance(node);
                updatedNodes.erase(node);
            }
            data.addedNodes.clear();
        }
        // Async additions
        if (!data.addedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.addedNodesAsync.begin(); it != data.addedNodesAsync.end();) {
                auto& mi = *it;
                data.scene->addInstance(mi);
                updatedNodes.erase(mi);
                it = data.addedNodesAsync.erase(it);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        for (const auto& mi : updatedNodes) {
            data.scene->updateInstance(mi);
        }
    }

}