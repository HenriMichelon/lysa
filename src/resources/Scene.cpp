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
        assert([&]{return meshInstance != nullptr;}, "Invalid meshInstance");
        meshInstance->setPendingUpdates(ctx.framesInFlight);
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

    void Scene::updateInstance(const std::shared_ptr<MeshInstance>& meshInstance) {
        assert([&]{return meshInstance != nullptr;}, "Invalid meshInstance");
        meshInstance->setPendingUpdates(ctx.framesInFlight);
        updatedNodes.push_back(meshInstance);
    }

    void Scene::removeInstance(const std::shared_ptr<MeshInstance>& meshInstance, const bool async) {
        assert([&]{return meshInstance != nullptr;}, "Invalid meshInstance");
        meshInstances.remove(meshInstance);
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
        assert([&]{ return environment.id != INVALID_ID; }, "Scene environment not set");
        auto lock = std::lock_guard(frameDataMutex);
        auto &data = framesData[frameIndex];
        // Remove from the renderer the nodes previously removed from the scene tree
        // Immediate removes
        if (!data.removedNodes.empty()) {
            for (const auto &node : data.removedNodes) {
                data.scene->removeInstance(node);
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
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        for (const auto& frame : framesData) {
            for (const auto& mi : updatedNodes) {
                frame.scene->updateInstance(mi);
            }
        }
        updatedNodes.clear();
    }

}