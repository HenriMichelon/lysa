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
        ResourcesManager(ctx, ID, capacity) {
    }

    unique_id RenderTargetManager::create(const RenderTargetConfiguration& configuration) {
        if (configuration.renderingWindowHandle == nullptr) {
            throw Exception("RenderTargetConfiguration : need a least one physical target, window or memory");
        }
        if (configuration.framesInFlight <= 0) {
            throw Exception("RenderTargetConfiguration : need a least one frame in flight");
        }
        auto& renderTarget = allocate();
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
            frame.renderCommandList = frame.commandAllocator->createCommandList();
        }
        return renderTarget.id;
    }

    void RenderTargetManager::destroy(RenderTarget& renderTarget) {
        renderTarget.swapChain->waitIdle();
        renderTarget.swapChain.reset();
        renderTarget.framesData.clear();
    }

    void RenderTargetManager::update() const {
        for (auto& renderTarget : getResources()) {
            if (renderTarget.paused) continue;
            const auto frameIndex = renderTarget.swapChain->getCurrentFrameIndex();
            //...
        }
    }

    void RenderTargetManager::drawFrame() const {
        for (auto& renderTarget : getResources()) {
            if (renderTarget.paused) continue;
            const auto frameIndex = renderTarget.swapChain->getCurrentFrameIndex();
            const auto& frame = renderTarget.framesData[frameIndex];
            const auto& swapChain = renderTarget.swapChain;

            if (!swapChain->acquire(frame.inFlightFence)) { return; }
            frame.commandAllocator->reset();

            auto& commandList = frame.renderCommandList;
            commandList->begin();
            commandList->barrier(swapChain, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
            commandList->barrier(swapChain, vireo::ResourceState::COPY_DST, vireo::ResourceState::PRESENT);
            commandList->end();

            ctx.graphicQueue->submit(
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
        .       addProperty("renderingWindowHandle", &RenderTargetConfiguration::renderingWindowHandle)
                .addProperty("swapChainFormat", &RenderTargetConfiguration::swapChainFormat)
                .addProperty("presentMode", &RenderTargetConfiguration::presentMode)
            .endClass()
            .beginClass<RenderTarget>("RenderTarget")
               .addProperty("id", &RenderTarget::id)
            .endClass()
            .beginClass<RenderTargetManager>("RenderTargetManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderTargetManager::ID)
               .addFunction("create", &RenderTargetManager::create)
                .addFunction("get", &RenderTargetManager::getById)
               .addFunction("destroy", &RenderTargetManager::destroy)
            .endClass()
        .endNamespace();
    }

}