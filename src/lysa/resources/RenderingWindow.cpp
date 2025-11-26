/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

import lysa.log;
import lysa.resources.locator;
import lysa.resources.render_target;

namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Context& ctx,const unique_id capacity) :
        ResourcesManager(ctx, ID, capacity) {
    }

    void RenderingWindow::_closing() {
        if (stopped) { return; }
        stopped = true;
        ctx.resourcesLocator.get<RenderTargetManager>(RenderTargetManager::ID).destroyAll(platformHandle);
        ctx.eventManager.push({id, static_cast<event_type>(RenderingWindowEvent::CLOSING)});
        ctx.resourcesLocator.get<RenderingWindowManager>(RenderingWindowManager::ID)._release(id);
    }

    void RenderingWindow::_resized() const {
        if (stopped) { return; }
        ctx.resourcesLocator.get<RenderTargetManager>(RenderTargetManager::ID).resize(platformHandle);
        ctx.eventManager.push({id, static_cast<event_type>(RenderingWindowEvent::RESIZED)});
    }

    RenderingWindow& RenderingWindowManager::create(const RenderingWindowConfiguration& configuration) {
        auto& instance = allocate(std::make_unique<RenderingWindow>(ctx, configuration));
        ctx.eventManager.push({instance.id, static_cast<event_type>(RenderingWindowEvent::READY)});
        return instance;
    }

    void RenderingWindowManager::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginNamespace("RenderingWindowMode")
                .addVariable("WINDOWED", RenderingWindowMode::WINDOWED)
                .addVariable("WINDOWED_MAXIMIZED", RenderingWindowMode::WINDOWED_MAXIMIZED)
                .addVariable("WINDOWED_FULLSCREEN", RenderingWindowMode::WINDOWED_FULLSCREEN)
                .addVariable("FULLSCREEN", RenderingWindowMode::FULLSCREEN)
            .endNamespace()
            .beginNamespace("RenderingWindowEventType")
                .addVariable("READY", &RenderingWindowEvent::READY)
                .addVariable("CLOSING", &RenderingWindowEvent::CLOSING)
                .addVariable("RESIZED", &RenderingWindowEvent::RESIZED)
            .endNamespace()
            .beginClass<RenderingWindowEvent>("RenderingWindowEvent")
                .addProperty("id", &RenderingWindowEvent::id)
                .addProperty("type", &RenderingWindowEvent::type)
            .endClass()
            .beginClass<RenderingWindowConfiguration>("RenderingWindowConfiguration")
                .addConstructor<void()>()
                .addProperty("title", &RenderingWindowConfiguration::title)
                .addProperty("mode", &RenderingWindowConfiguration::mode)
                .addProperty("x", &RenderingWindowConfiguration::x)
                .addProperty("y", &RenderingWindowConfiguration::y)
                .addProperty("width", &RenderingWindowConfiguration::width)
                .addProperty("height", &RenderingWindowConfiguration::height)
                .addProperty("monitor", &RenderingWindowConfiguration::monitor)
            .endClass()
            .beginClass<RenderingWindow>("RenderingWindow")
               .addProperty("id", &RenderingWindow::id)
               .addProperty("x", &RenderingWindow::getX)
               .addProperty("y", &RenderingWindow::getY)
               .addProperty("width", &RenderingWindow::getWidth)
               .addProperty("height", &RenderingWindow::getHeight)
               .addProperty("stopped", &RenderingWindow::isStopped)
               .addProperty("platform_handle", &RenderingWindow::getPlatformHandle)
                .addFunction("show", &RenderingWindow::show)
                .addFunction("close", &RenderingWindow::close)
            .endClass()
            .beginClass<RenderingWindowManager>("RenderingWindowManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderingWindowManager::ID)
               .addFunction("create", &RenderingWindowManager::create)
               .addFunction("get",
                   luabridge::nonConstOverload<const unique_id>(&RenderingWindowManager::get),
                   luabridge::constOverload<const unique_id>(&RenderingWindowManager::get)
                   )
            .endClass()
        .endNamespace();
    }

}