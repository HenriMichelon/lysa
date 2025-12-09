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

    void RenderTarget::update() const {
        if (paused) return;
        const auto frameIndex = swapChain->getCurrentFrameIndex();
        // viewportManager.update(id, *renderer, frameIndex);
    }

    void RenderTarget::render(
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors,
        const CameraDesc& camera,
        SceneContext& scene) const {
        if (paused) return;
        const auto frameIndex =swapChain->getCurrentFrameIndex();
        const auto& frame = framesData[frameIndex];

        if (!swapChain->acquire(frame.inFlightFence)) { return; }
        frame.commandAllocator->reset();

        frame.prepareCommandList->begin();
        // viewportManager.prepare(id, *renderer, *frame.prepareCommandList, frameIndex);
        frame.prepareCommandList->end();
        ctx.graphicQueue->submit(
                   vireo::WaitStage::ALL_COMMANDS,
                   frame.prepareSemaphore,
                   {frame.prepareCommandList});

        auto& commandList = frame.renderCommandList;
        commandList->begin();
        // viewportManager.render(id, *renderer, *frame.renderCommandList, frameIndex);

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
                Manager::destroy(renderTarget->id);
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

    void RenderTargetManager::update() const {
        for (auto& renderTarget : getResources()) {
            renderTarget->update();
        }
    }

}