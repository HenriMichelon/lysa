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
            frame.updateSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY, "Update");
            frame.computeSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY, "Compute");
            frame.prepareSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY, "Prepare");
            frame.updateCommandList = frame.commandAllocator->createCommandList();
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

    void RenderTarget::addView(RenderView& view) {
        assert([&]{ return std::ranges::find(views, view) == views.end(); },
            "RenderView already attached to the RenderTarget");
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
        auto lock = std::unique_lock{viewsMutex};
        swapChain->waitIdle();
        views.push_back(view);
    }

    void RenderTarget::removeView(const RenderView& view) {
        assert([&]{ return std::ranges::find(views, view) != views.end(); },
            "RenderView not attached to the RenderTarget");
        auto lock = std::unique_lock{viewsMutex};
        swapChain->waitIdle();
        views.remove(view);
    }

    void RenderTarget::updateView(RenderView& view) {
        removeView(view);
        addView(view);
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
        auto lock = std::unique_lock{viewsMutex};
        const auto frameIndex = swapChain->getCurrentFrameIndex();
        const auto& frame = framesData[frameIndex];
        if (!swapChain->acquire(frame.inFlightFence)) { return; }
        frame.commandAllocator->reset();
        for (auto& view : views) {
            view.scene.processDeferredOperations(frameIndex);
            auto& data = view.scene.get(frameIndex);
            if (data.isMaterialsUpdated()) {
                renderer->updatePipelines(data);
                data.resetMaterialsUpdated();
            }
        }
        renderer->update(frameIndex);

        frame.updateCommandList->begin();
        for (auto& view : views) {
            view.scene.get(frameIndex).update(*frame.updateCommandList, view.camera);
        }
        frame.updateCommandList->end();
        ctx.graphicQueue->submit(
            vireo::WaitStage::TRANSFER,
            frame.updateSemaphore,
            {frame.updateCommandList});

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
        frame.updateSemaphore,
            vireo::WaitStage::COMPUTE_SHADER,
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

}