/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.render_target;

import vireo;
import lysa.exception;
import lysa.log;
import lysa.renderers.renderer;

namespace lysa {

    RenderTarget::RenderTarget(
        Context& ctx,
        const RenderTargetConfiguration& configuration,
        const RenderingWindowHandle renderingWindowHandle) :
        ctx(ctx)  {
        if (renderingWindowHandle == nullptr) {
            throw Exception("RenderTargetConfiguration : need a least one physical target, window or memory");
        }
        if (ctx.framesInFlight <= 0) {
            throw Exception("RenderTargetConfiguration : need a least one frame in flight");
        }
        swapChain = ctx.vireo->createSwapChain(
            configuration.swapChainFormat,
            ctx.graphicQueue,
            renderingWindowHandle,
            configuration.presentMode,
            ctx.framesInFlight);
        renderer = Renderer::create(ctx, configuration.rendererConfiguration, ctx.framesInFlight);
        framesData.resize(ctx.framesInFlight);
        for (auto& frame : framesData) {
            frame.inFlightFence = ctx.vireo->createFence(true, "inFlightFence");
            frame.commandAllocator = ctx.vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
            frame.computeSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY, "Compute");
            frame.prepareSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY, "Prepare");
            frame.computeCommandList = frame.commandAllocator->createCommandList();
            frame.prepareCommandList = frame.commandAllocator->createCommandList();
            frame.renderCommandList = frame.commandAllocator->createCommandList();
        }

        // Create the main rendering attachments
        const auto& frame = framesData[0];
        frame.commandAllocator->reset();
        frame.prepareCommandList->begin();
        renderer->resize(swapChain->getExtent(), frame.prepareCommandList);
        frame.prepareCommandList->end();
        ctx.graphicQueue->submit({frame.prepareCommandList});
        ctx.graphicQueue->waitIdle();
    }

    RenderTarget::~RenderTarget() {
        swapChain->waitIdle();
        swapChain.reset();
        framesData.clear();
        views.clear();
    }

    void RenderTarget::addView(const RenderView& view) {
        views.push_back(view);
    }

    void RenderTarget::removeView(const RenderView& view) {
        views.remove(view);
    }

    void RenderTarget::setPause(const bool pause) {
        paused = pause;
        ctx.events.push({id, static_cast<event_type>(
                paused ? RenderTargetEvent::PAUSED : RenderTargetEvent::RESUMED)});
    }

    void RenderTarget::resize() {
        setPause(true);
        const auto previousExtent = swapChain->getExtent();
        swapChain->recreate();
        const auto newExtent = swapChain->getExtent();
        if (previousExtent.width != newExtent.width || previousExtent.height != newExtent.height) {
            const auto& frame = framesData[0];
            // viewportManager.resize(id, newExtent);
            frame.commandAllocator->reset();
            frame.prepareCommandList->begin();
            renderer->resize(newExtent, frame.prepareCommandList);
            frame.prepareCommandList->end();
            ctx.graphicQueue->submit({frame.prepareCommandList});
            ctx.graphicQueue->waitIdle();
            ctx.events.push({id, static_cast<event_type>(RenderTargetEvent::RESIZED), newExtent});
        }
        setPause(false);
    }

    void RenderTarget::render() {
        if (isPaused()) return;
        const auto frameIndex = swapChain->getCurrentFrameIndex();
        for (auto& view : views) {
            if (view.viewport.width == 0.0f || view.viewport.height == 0.0f) {
                view.viewport.width = static_cast<float>(swapChain->getExtent().width);
                view.viewport.height = static_cast<float>(swapChain->getExtent().height);
            }
            if (view.scissors.width   == 0.0f || view.scissors.height == 0.0f) {
                view.scissors.x = static_cast<int32>(view.viewport.x);
                view.scissors.y = static_cast<int32>(view.viewport.y);
                view.scissors.width = static_cast<int32>(view.viewport.width);
                view.scissors.height = static_cast<int32>(view.viewport.height);
            }
            view.scene.processDeferredOperations(frameIndex);
            auto& data = view.scene.get(frameIndex);
            if (data.isMaterialsUpdated()) {
                renderer->updatePipelines(data);
                data.resetMaterialsUpdated();
            }
        }

        const auto& frame = framesData[frameIndex];
        renderer->update(frameIndex);

        if (!swapChain->acquire(frame.inFlightFence)) { return; }
        frame.commandAllocator->reset();

        frame.computeCommandList->begin();
        for (auto& view : views) {
            renderer->compute(
                *frame.computeCommandList,
                view.scene.get(frameIndex),
                view.camera,
                frameIndex);
        }
        frame.computeCommandList->end();
        ctx.graphicQueue->submit(
            vireo::WaitStage::COMPUTE_SHADER,
            frame.computeSemaphore,
            {frame.computeCommandList});

        frame.prepareCommandList->begin();
        for (auto& view : views) {
            auto& data = view.scene.get(frameIndex);
            data.prepare(*frame.prepareCommandList, view.viewport, view.scissors);
            renderer->prepare(*frame.prepareCommandList, data, frameIndex);
        }
        frame.prepareCommandList->end();
        ctx.graphicQueue->submit(
            frame.computeSemaphore,
            vireo::WaitStage::VERTEX_INPUT,
            vireo::WaitStage::ALL_COMMANDS,
            frame.prepareSemaphore,
            {frame.prepareCommandList});

        auto& commandList = frame.renderCommandList;
        commandList->begin();
        auto clearAttachment{true};
        for (auto& view : views) {
            auto& data = view.scene.get(frameIndex);
            data.prepare(*commandList, view.viewport, view.scissors);
            renderer->render(*commandList, data, clearAttachment, frameIndex);
            clearAttachment = false;
        }

        const auto colorAttachment = renderer->getCurrentColorAttachment(frameIndex);

        commandList->barrier(colorAttachment, vireo::ResourceState::UNDEFINED,vireo::ResourceState::COPY_SRC);
        commandList->barrier(swapChain, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
        commandList->copy(colorAttachment->getImage(), swapChain);
        commandList->barrier(swapChain, vireo::ResourceState::COPY_DST, vireo::ResourceState::PRESENT);
        commandList->barrier(colorAttachment, vireo::ResourceState::COPY_SRC,vireo::ResourceState::UNDEFINED);
        commandList->end();

        ctx.graphicQueue->submit(
            frame.prepareSemaphore,
            vireo::WaitStage::VERTEX_INPUT,
            frame.inFlightFence,
            swapChain,
            {commandList});
        swapChain->present();
        swapChain->nextFrameIndex();
    }

    void RenderTarget::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) const {
        renderer->updatePipelines(pipelineIds);
    }
    //
    // RenderTargetManager::RenderTargetManager(Context& ctx, const size_t capacity, const uint32 framesInFlight) :
    //     ResourcesManager(ctx, capacity, "RenderTargetManager"),
    //     framesInFlight(framesInFlight) {
    //     ctx.res.enroll(*this);
    // }
    //
    // RenderTarget& RenderTargetManager::create(const RenderTargetConfiguration& configuration) {
    //     return ResourcesManager::create(configuration, framesInFlight);
    // }
    //
    // void RenderTargetManager::destroy(const void* renderingWindowHandle) {
    //     for (const auto& renderTarget : getResources()) {
    //         if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
    //             renderTarget->pause(true);
    //             ctx.defer.push([&] {
    //                 ResourcesManager::destroy(renderTarget->id);
    //             });
    //         }
    //     }
    // }
    //
    // void RenderTargetManager::pause(const void* renderingWindowHandle, const bool pause) {
    //     for (const auto& renderTarget : getResources()) {
    //         if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
    //             renderTarget->pause(pause);
    //         }
    //     }
    // }
    //
    // void RenderTargetManager::resize(const void* renderingWindowHandle) const {
    //     for (const auto& renderTarget : getResources()) {
    //         if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
    //             renderTarget->resize();
    //         }
    //     }
    // }
    //
    // // void RenderTargetManager::input(const void* renderingWindowHandle, const InputEvent& inputEvent) const {
    // //     for (const auto& renderTarget : getResources()) {
    // //         if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
    // //             renderTarget->input(inputEvent);
    // //         }
    // //     }
    // // }
    //
    // void RenderTargetManager::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) const {
    //     for (const auto& renderTarget : getResources()) {
    //         renderTarget->updatePipelines(pipelineIds);
    //     }
    // }

}