/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

import lysa.log;
import lysa.resources.locator;

namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Context& ctx,const unique_id capacity) :
        ResourcesManager(ctx, ID, capacity) {
    }

    void RenderingWindowManager::_closing(const unique_id id) {
        auto& window = get(id);
        window.stopped = true;
        ctx.eventManager.push({window.id, static_cast<event_type>(RenderingWindowEvent::CLOSING)});
        release(id);
    }

    void RenderingWindowManager::_resized(const unique_id id) const {
        const auto& window = get(id);
        if (window.stopped) { return; }
        ctx.eventManager.push({window.id, static_cast<event_type>(RenderingWindowEvent::RESIZED)});
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
               .addProperty("x", &RenderingWindow::x)
               .addProperty("y", &RenderingWindow::y)
               .addProperty("width", &RenderingWindow::width)
               .addProperty("height", &RenderingWindow::height)
               .addProperty("stopped", &RenderingWindow::stopped)
               .addProperty("platform_handle", &RenderingWindow::platformHandle)
            .endClass()
            .beginClass<RenderingWindowManager>("RenderingWindowManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderingWindowManager::ID)
               .addFunction("create", &RenderingWindowManager::create)
               .addFunction("get", &RenderingWindowManager::getById)
               .addFunction("show", &RenderingWindowManager::show)
               .addFunction("destroy", &RenderingWindowManager::destroy)
            .endClass()
        .endNamespace();
    }

}