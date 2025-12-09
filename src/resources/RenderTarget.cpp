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
    RenderTarget::RenderTarget(Context& ctx, const RenderTargetConfiguration& configuration, const uint32 framesInFlight) :
        ctx(ctx) {
        if (configuration.renderingWindowHandle == nullptr) {
            throw Exception("RenderTargetConfiguration : need a least one physical target, window or memory");
        }
        if (framesInFlight <= 0) {
            throw Exception("RenderTargetConfiguration : need a least one frame in flight");
        }
        this->renderingWindowHandle = configuration.renderingWindowHandle;
        swapChain = ctx.vireo->createSwapChain(
            configuration.swapChainFormat,
            ctx.graphicQueue,
            configuration.renderingWindowHandle,
            configuration.presentMode,
            framesInFlight);
        renderer = Renderer::create(ctx, configuration.rendererConfiguration, framesInFlight);
        framesData.resize(framesInFlight);
        for (auto& frame : framesData) {
            frame.inFlightFence = ctx.vireo->createFence(true, "inFlightFence");
            frame.commandAllocator = ctx.vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
            frame.prepareSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY);
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
        // viewportManager.destroyByRenderTarget(id);
        swapChain.reset();
        framesData.clear();
    }

    void RenderTarget::pause(const bool pause) {
        if (paused != pause) {
            paused = pause;
            ctx.events.push({id, static_cast<event_type>(
                    paused ? RenderTargetEvent::PAUSED : RenderTargetEvent::RESUMED)});
        }
    }

    void RenderTarget::resize() const {
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
            ctx.events.push({id, static_cast<event_type>(RenderTargetEvent::RESIZED)});
        }
    }

    void RenderTarget::render(std::list<RenderView>& views) const {
        if (paused) return;
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
            if (view.scene.isMaterialsUpdated()) {
                renderer->updatePipelines(view.scene);
                view.scene.resetMaterialsUpdated();
            }
        }

        const auto frameIndex =swapChain->getCurrentFrameIndex();
        const auto& frame = framesData[frameIndex];

        renderer->update(frameIndex);

        if (!swapChain->acquire(frame.inFlightFence)) { return; }
        frame.commandAllocator->reset();

        frame.prepareCommandList->begin();
        for (auto& view : views) {
            view.scene.setInitialState(*frame.prepareCommandList, view.viewport, view.scissors);
            view.scene.update(view.camera, *frame.prepareCommandList);
        }
        frame.prepareCommandList->end();
        ctx.graphicQueue->submit(
                   vireo::WaitStage::ALL_COMMANDS,
                   frame.prepareSemaphore,
                   {frame.prepareCommandList});

        auto& commandList = frame.renderCommandList;
        commandList->begin();
        for (auto& view : views) {
            renderer->render(*commandList, view.scene, true, frameIndex);
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

    RenderTargetManager::RenderTargetManager(Context& ctx, const size_t capacity, const uint32 framesInFlight) :
        ResourcesManager(ctx, capacity), framesInFlight(framesInFlight) {
        ctx.res.enroll(*this);
    }

    RenderTarget& RenderTargetManager::create(const RenderTargetConfiguration& configuration) {
        return ResourcesManager::create(ctx, configuration, framesInFlight);
    }

    void RenderTargetManager::destroy(const void* renderingWindowHandle) {
        for (const auto& renderTarget : getResources()) {
            if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
                renderTarget->pause(true);
                ctx.defer.push([&] {
                    Manager::destroy(renderTarget->id);
                });
            }
        }
    }

    void RenderTargetManager::pause(const void* renderingWindowHandle, const bool pause) {
        for (const auto& renderTarget : getResources()) {
            if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
                renderTarget->pause(pause);
            }
        }
    }

    void RenderTargetManager::resize(const void* renderingWindowHandle) const {
        for (const auto& renderTarget : getResources()) {
            if (renderTarget->getRenderingWindowHandle() == renderingWindowHandle) {
                renderTarget->resize();
            }
        }
    }

}