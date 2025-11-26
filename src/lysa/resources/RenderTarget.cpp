/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.render_target;

import lysa.exception;
import lysa.log;
import lysa.resources.locator;

namespace lysa {

    RenderTargetManager::RenderTargetManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(ctx, ID, capacity),
        viewportManager{ctx.resourcesLocator.get<ViewportManager>(ViewportManager::ID)} {
    }

    RenderTarget& RenderTargetManager::create(const RenderTargetConfiguration& configuration) {
        if (configuration.renderingWindowHandle == nullptr) {
            throw Exception("RenderTargetConfiguration : need a least one physical target, window or memory");
        }
        if (configuration.framesInFlight <= 0) {
            throw Exception("RenderTargetConfiguration : need a least one frame in flight");
        }
        auto& renderTarget = allocate(std::make_unique<RenderTarget>(ctx));
        renderTarget.configuration = configuration;
        renderTarget.swapChain = ctx.vireo->createSwapChain(
            configuration.swapChainFormat,
            ctx.graphicQueue,
            configuration.renderingWindowHandle,
            configuration.presentMode,
            configuration.framesInFlight);
        renderTarget.framesData.resize(configuration.framesInFlight);
        for (auto& frame : renderTarget.framesData) {
            frame.inFlightFence = ctx.vireo->createFence(true, "inFlightFence");
            frame.commandAllocator = ctx.vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
            frame.prepareSemaphore = ctx.vireo->createSemaphore(vireo::SemaphoreType::BINARY);
            frame.prepareCommandList = frame.commandAllocator->createCommandList();
            frame.renderCommandList = frame.commandAllocator->createCommandList();
        }
        return renderTarget;
    }

    void RenderTargetManager::destroyAll(const void* renderingWindowHandle) {
        for (auto& renderTarget : getResources()) {
            if (renderTarget->configuration.renderingWindowHandle == renderingWindowHandle) {
                destroy(renderTarget->id);
            }
        }
    }

    void RenderTargetManager::destroy(const unique_id id) {
        auto& renderTarget = get(id);
        renderTarget.swapChain->waitIdle();
        viewportManager.destroyAll(renderTarget.id);
        renderTarget.swapChain.reset();
        renderTarget.framesData.clear();
        _release(renderTarget.id);
    }

    void RenderTargetManager::pause(const void* renderingWindowHandle, const bool pause) {
        for (auto& renderTarget : getResources()) {
            if (renderTarget->configuration.renderingWindowHandle != renderingWindowHandle) continue;
            renderTarget->paused = pause;
            ctx.eventManager.push({renderTarget->id, static_cast<event_type>(
                renderTarget->paused ? RenderTargetEvent::PAUSED : RenderTargetEvent::RESUMED)});
        }
    }

    void RenderTargetManager::resize(const void* renderingWindowHandle) const {
        for (auto& renderTarget : getResources()) {
            if (renderTarget->configuration.renderingWindowHandle != renderingWindowHandle) continue;
            const auto previousExtent = renderTarget->swapChain->getExtent();
            renderTarget->swapChain->recreate();
            const auto newExtent = renderTarget->swapChain->getExtent();
            if (previousExtent.width != newExtent.width || previousExtent.height != newExtent.height) {
                const auto& frame = renderTarget->framesData[0];
                viewportManager.resize(renderTarget->id, newExtent);
                /*frame.commandAllocator->reset();
                frame.prepareCommandList->begin();
                frame.prepareCommandList->end();
                ctx.graphicQueue->submit({frame.prepareCommandList});
                ctx.graphicQueue->waitIdle();*/
            }
            ctx.eventManager.push({renderTarget->id, static_cast<event_type>(RenderTargetEvent::RESIZED)});
        }
    }

    void RenderTargetManager::update() const {
        for (auto& renderTarget : getResources()) {
            if (renderTarget->paused) continue;
            const auto frameIndex = renderTarget->swapChain->getCurrentFrameIndex();
            viewportManager.update(renderTarget->id, frameIndex);
        }
    }

    void RenderTargetManager::render() const {
        for (auto& renderTarget : getResources()) {
            if (renderTarget->paused) continue;
            const auto frameIndex = renderTarget->swapChain->getCurrentFrameIndex();
            const auto& frame = renderTarget->framesData[frameIndex];
            const auto& swapChain = renderTarget->swapChain;

            if (!swapChain->acquire(frame.inFlightFence)) { return; }
            frame.commandAllocator->reset();

            frame.prepareCommandList->begin();
            viewportManager.prepare(renderTarget->id, frameIndex);
            frame.prepareCommandList->end();
            ctx.graphicQueue->submit(
                       vireo::WaitStage::ALL_COMMANDS,
                       frame.prepareSemaphore,
                       {frame.prepareCommandList});

            auto& commandList = frame.renderCommandList;
            commandList->begin();
            viewportManager.render(renderTarget->id, frameIndex);
            commandList->barrier(swapChain, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
            commandList->barrier(swapChain, vireo::ResourceState::COPY_DST, vireo::ResourceState::PRESENT);
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
    }

    void RenderTargetManager::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginClass<RenderTargetConfiguration>("RenderTargetConfiguration")
                .addConstructor<void()>()
        .       addProperty("rendering_window_handle", &RenderTargetConfiguration::renderingWindowHandle)
                .addProperty("swap_chain_format", &RenderTargetConfiguration::swapChainFormat)
                .addProperty("present_mode", &RenderTargetConfiguration::presentMode)
            .endClass()
            .beginNamespace("RenderTargetEventType")
                .addVariable("PAUSED", &RenderTargetEvent::PAUSED)
                .addVariable("RESUMED", &RenderTargetEvent::RESUMED)
                .addVariable("RESIZED", &RenderTargetEvent::RESIZED)
            .endNamespace()
            .beginClass<RenderTargetEvent>("RenderTargetEvent")
                .addProperty("id", &RenderTargetEvent::id)
                .addProperty("type", &RenderTargetEvent::type)
            .endClass()
            .beginClass<RenderTarget>("RenderTarget")
               .addProperty("id", &RenderTarget::id)
            .endClass()
            .beginClass<RenderTargetManager>("RenderTargetManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderTargetManager::ID)
                .addFunction("create", &RenderTargetManager::create)
                .addFunction("get",
                    luabridge::nonConstOverload<const unique_id>(&RenderTargetManager::get),
                    luabridge::constOverload<const unique_id>(&RenderTargetManager::get)
                    )
                .addFunction("destroyAll", &RenderTargetManager::destroyAll)
                .addFunction("destroy",
                   luabridge::overload<const unique_id>(&Manager::destroy)
                )
            .endClass()
        .endNamespace();
    }

}