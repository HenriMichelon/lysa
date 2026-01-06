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
        maxAsyncNodesUpdatedPerFrame(config.asyncObjectUpdatesPerFrame) {
        framesData.resize(ctx.config.framesInFlight);
        for (auto& data : framesData) {
            data.scene =std::make_unique<SceneFrameData>(
                ctx,
                config.maxLights,
                config.maxMeshInstances,
                config.maxMeshSurfacePerPipeline);
        }
    }

    Scene::~Scene() {
        ctx.graphicQueue->waitIdle();
    }

    void Scene::setEnvironment(const Environment& environment) {
        auto lock = std::lock_guard(frameDataMutex);
        for (const auto& data : framesData) {
            data.scene->setEnvironment(environment);
        }
    }

    bool Scene::haveInstance(const MeshInstance& meshInstance) const {
        return meshInstances.contains(&meshInstance);
    }

    void Scene::addLight(const Light& light) {
        auto lock = std::lock_guard(frameDataMutex);
        for (const auto& frame : framesData) {
            frame.scene->addLight(&light);
        }
    }

    void Scene::removeLight(const Light& light) {
        auto lock = std::lock_guard(frameDataMutex);
        for (const auto& frame : framesData) {
            frame.scene->addLight(&light);
        }
    }

    void Scene::addInstance(const MeshInstance& meshInstance, const bool async) {
        const auto* pMeshInstance = &meshInstance;
        assert([&]{return !meshInstances.contains(pMeshInstance);}, "MeshInstance already in scene");
        meshInstances.insert(pMeshInstance);
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.addedNodesAsync.insert(pMeshInstance);
            } else {
                frame.addedNodes.insert(pMeshInstance);
            }
        }
    }

    void Scene::updateInstance(const MeshInstance& meshInstance) {
        const auto* pMeshInstance = &meshInstance;
        assert([&]{return meshInstances.contains(pMeshInstance);}, "MeshInstance not in scene");
        updatedNodes.insert(pMeshInstance);
    }

    void Scene::removeInstance(const MeshInstance& meshInstance, const bool async) {
        const auto* pMeshInstance = &meshInstance;
        assert([&]{return meshInstances.contains(pMeshInstance);}, "MeshInstance not in scene");
        meshInstances.erase(pMeshInstance);
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            if (async) {
                frame.removedNodesAsync.insert(pMeshInstance);
            } else {
                frame.removedNodes.insert(pMeshInstance);
            }
        }
    }

    void Scene::processDeferredOperations(const uint32 frameIndex) {
        auto lock = std::lock_guard(frameDataMutex);
        auto &data = framesData[frameIndex];
        // Remove from the renderer the nodes previously removed from the scene tree
        // Immediate removes
        if (!data.removedNodes.empty()) {
            for (const auto *mi : data.removedNodes) {
                data.scene->removeInstance(mi);
                updatedNodes.erase(mi);
            }
            data.removedNodes.clear();
        }
        // Async removes
        if (!data.removedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.removedNodesAsync.begin(); it != data.removedNodesAsync.end();) {
                const auto* mi = *it;
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
            for (const auto* mi : data.addedNodes) {
                data.scene->addInstance(mi);
                updatedNodes.erase(mi);
            }
            data.addedNodes.clear();
        }
        // Async additions
        if (!data.addedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.addedNodesAsync.begin(); it != data.addedNodesAsync.end();) {
                const auto* mi = *it;
                data.scene->addInstance(mi);
                updatedNodes.erase(mi);
                it = data.addedNodesAsync.erase(it);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        for (const auto* mi : updatedNodes) {
            data.scene->updateInstance(mi);
        }
    }

}