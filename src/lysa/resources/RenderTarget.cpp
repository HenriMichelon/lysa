/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.render_target;

import lysa.exception;
import lysa.log;
import lysa.renderers.renderer;
import lysa.resources.locator;

namespace lysa {

    RenderTarget::RenderTarget(Context& ctx, const RenderTargetConfiguration& configuration) :
        Resource(ctx),
        viewportManager(ctx.resourcesLocator.get<ViewportManager>(ViewportManager::ID)){
        if (configuration.renderingWindowHandle == nullptr) {
            throw Exception("RenderTargetConfiguration : need a least one physical target, window or memory");
        }
        if (configuration.swapChainConfiguration.framesInFlight <= 0) {
            throw Exception("RenderTargetConfiguration : need a least one frame in flight");
        }
        this->renderingWindowHandle = configuration.renderingWindowHandle;
        swapChain = ctx.vireo->createSwapChain(
            configuration.swapChainConfiguration.swapChainFormat,
            ctx.graphicQueue,
            configuration.renderingWindowHandle,
            configuration.swapChainConfiguration.presentMode,
            configuration.swapChainConfiguration.framesInFlight);
        renderer = Renderer::create(ctx, configuration.rendererConfiguration, configuration.swapChainConfiguration);
        framesData.resize(configuration.swapChainConfiguration.framesInFlight);
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
        viewportManager.destroyByRenderTarget(id);
        swapChain.reset();
        framesData.clear();
    }

    void RenderTarget::pause(const bool pause) {
        if (paused != pause) {
            paused = pause;
            ctx.eventManager.push({id, static_cast<event_type>(
                    paused ? RenderTargetEvent::PAUSED : RenderTargetEvent::RESUMED)});
        }
    }

    void RenderTarget::resize() const {
        const auto previousExtent = swapChain->getExtent();
        swapChain->recreate();
        const auto newExtent = swapChain->getExtent();
        if (previousExtent.width != newExtent.width || previousExtent.height != newExtent.height) {
            const auto& frame = framesData[0];
            viewportManager.resize(id, newExtent);
            frame.commandAllocator->reset();
            frame.prepareCommandList->begin();
            renderer->resize(newExtent, frame.prepareCommandList);
            frame.prepareCommandList->end();
            ctx.graphicQueue->submit({frame.prepareCommandList});
            ctx.graphicQueue->waitIdle();
            ctx.eventManager.push({id, static_cast<event_type>(RenderTargetEvent::RESIZED)});
        }
    }

    void RenderTarget::update() const {
        if (paused) return;
        const auto frameIndex = swapChain->getCurrentFrameIndex();
        // viewportManager.update(renderTarget->id, frameIndex);
    }

    void RenderTarget::render() const {
        if (paused) return;
        const auto frameIndex =swapChain->getCurrentFrameIndex();
        const auto& frame = framesData[frameIndex];

        if (!swapChain->acquire(frame.inFlightFence)) { return; }
        frame.commandAllocator->reset();

        frame.prepareCommandList->begin();
        //viewportManager.prepare(renderTarget->id, frameIndex);
        frame.prepareCommandList->end();
        ctx.graphicQueue->submit(
                   vireo::WaitStage::ALL_COMMANDS,
                   frame.prepareSemaphore,
                   {frame.prepareCommandList});

        auto& commandList = frame.renderCommandList;
        commandList->begin();
        //viewportManager.render(renderTarget->id, frameIndex);

        const auto colorAttachment = renderer->getCurrentColorAttachment(frameIndex);

        commandList->barrier(swapChain, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
        commandList->copy(colorAttachment->getImage(), swapChain);
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

    RenderTargetManager::RenderTargetManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(ctx, ID, capacity) {
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

    void RenderTargetManager::render() const {
        for (auto& renderTarget : getResources()) {
            renderTarget->render();
        }
    }

    void RenderTargetManager::_register(const Lua& lua) {
        Renderer::_register(lua);
        lua.beginNamespace()
            .beginClass<RenderTargetConfiguration>("RenderTargetConfiguration")
                .addConstructor<void()>()
                .addProperty("rendering_window_handle", &RenderTargetConfiguration::renderingWindowHandle)
                .addProperty("renderer_configuration", &RenderTargetConfiguration::rendererConfiguration)
                .addProperty("swap_chain_configuration", &RenderTargetConfiguration::swapChainConfiguration)
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
               .addFunction("pause", &RenderTarget::pause)
               .addFunction("resize", &RenderTarget::resize)
               .addFunction("swap_chain", &RenderTarget::getSwapChain)
               .addFunction("rendering_window_handle", &RenderTarget::getRenderingWindowHandle)
            .endClass()
            .beginClass<RenderTargetManager>("RenderTargetManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderTargetManager::ID)
                .addFunction("create", &ResourcesManager::create<RenderTargetConfiguration>)
                .addFunction("get",
                    luabridge::nonConstOverload<const unique_id>(&RenderTargetManager::get),
                    luabridge::constOverload<const unique_id>(&RenderTargetManager::get)
                    )
                .addFunction("destroy",
                   luabridge::overload<const unique_id>(&Manager::destroy),
                   luabridge::overload<const void*>(&RenderTargetManager::destroy)
                )
            .endClass()
        .endNamespace();
    }

}