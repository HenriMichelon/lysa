/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.scene_context;

import lysa.exception;
import lysa.log;

namespace lysa {

    SceneContext::SceneContext(
        const Context& ctx,
        const uint32 maxAsyncNodesUpdatedPerFrame,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 framesInFlight,
        uint32 maxShadowMaps) :
        ctx(ctx),
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

    SceneContext::~SceneContext() {
        ctx.graphicQueue->waitIdle();
    }

    void SceneContext::addInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance, const bool async) {
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

    void SceneContext::updateInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance) {
        assert([&]{return meshInstance != nullptr;}, "meshInstance can't be null");
        meshInstance->pendingUpdates = framesInFlight;
        auto lock = std::lock_guard(frameDataMutex);
        for (auto& frame : framesData) {
            frame.updatedNodes.push_back(meshInstance);
        }
    }

    void SceneContext::removeInstance(const std::shared_ptr<MeshInstanceDesc> &meshInstance, const bool async) {
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

    void SceneContext::processDeferredOperations(const uint32 frameIndex) {
        auto lock = std::lock_guard(frameDataMutex);
        auto &data = framesData[frameIndex];
        data.scene->setAmbientLight(ambientLight);
        // Remove from the renderer the nodes previously removed from the scene tree
        // Immediate removes
        if (!data.removedNodes.empty()) {
            for (const auto &node : data.removedNodes) {
                auto& mi = std::get<std::shared_ptr<MeshInstanceDesc>>(node);
                data.scene->removeInstance(mi);
                data.updatedNodes.remove(mi);
            }
            data.removedNodes.clear();
        }
        // Async removes
        if (!data.removedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.removedNodesAsync.begin(); it != data.removedNodesAsync.end();) {
                auto& mi = std::get<std::shared_ptr<MeshInstanceDesc>>(*it);
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
                auto& mi = std::get<std::shared_ptr<MeshInstanceDesc>>(node);
                data.scene->addInstance(mi);
                data.updatedNodes.remove(mi);
            }
            data.addedNodes.clear();
        }
        // Async additions
        if (!data.addedNodesAsync.empty()) {
            auto count = 0;
            for (auto it = data.addedNodesAsync.begin(); it != data.addedNodesAsync.end();) {
                auto& mi = std::get<std::shared_ptr<MeshInstanceDesc>>(*it);
                data.scene->addInstance(mi);
                it = data.addedNodesAsync.erase(it);
                data.updatedNodes.remove(mi);
                count += 1;
                if (count > maxAsyncNodesUpdatedPerFrame) { break; }
            }
        }
        if (!data.updatedNodes.empty()) {
            for (const auto &node : data.updatedNodes) {
                data.scene->updateInstance(std::get<std::shared_ptr<MeshInstanceDesc>>(node));
            }
            data.updatedNodes.clear();
        }
    }

    SceneContextManager::SceneContextManager(
        Context& ctx,
        const size_t capacity,
        const uint32 maxAsyncNodesUpdatedPerFrame,
        const uint32 maxLights,
        const uint32 maxMeshInstancesPerScene,
        const uint32 maxMeshSurfacePerPipeline,
        const uint32 maxShadowMaps,
        const uint32 framesInFlight) :
        ResourcesManager(ctx, capacity, "SceneContextManager"),
        maxAsyncNodesUpdatedPerFrame(maxAsyncNodesUpdatedPerFrame),
        maxLights(maxLights),
        maxMeshInstancesPerScene(maxMeshInstancesPerScene),
        maxMeshSurfacePerPipeline(maxMeshSurfacePerPipeline),
        maxShadowMaps(maxShadowMaps),
        framesInFlight(framesInFlight){
            ctx.res.enroll(*this);
    }

    SceneContext& SceneContextManager::create() {
        return ResourcesManager::create(
            maxAsyncNodesUpdatedPerFrame,
            maxLights,
            maxMeshInstancesPerScene,
            maxMeshSurfacePerPipeline,
            framesInFlight,
            maxShadowMaps);
    }

}