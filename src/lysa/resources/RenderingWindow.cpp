/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
module lysa.resources.rendering_window;

import lysa.resources.locator;

import lysa.log;
import lysa.lua;

namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(capacity),
        ctx{ctx} {
        ctx.resourcesLocator.enroll(RENDERING_WINDOW, *this);
    }

    void RenderingWindowManager::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginNamespace("RenderingWindowMode")
                .addVariable("WINDOWED", RenderingWindowMode::WINDOWED)
                .addVariable("WINDOWED_FULLSCREEN", RenderingWindowMode::WINDOWED_FULLSCREEN)
            .endNamespace()
            .beginNamespace("RenderingWindowEventType")
                .addVariable("READY", RenderingWindowEventType::READY)
            .endNamespace()
            .beginClass<RenderingWindowEvent>("RenderingWindowEvent")
                .addProperty("id", &RenderingWindowEvent::id)
            .endClass()
            .beginClass<RenderingWindowConfiguration>("RenderingWindowConfiguration")
                .addConstructor<void()>()
                .addProperty("title", &RenderingWindowConfiguration::title)
                .addProperty("mode", &RenderingWindowConfiguration::mode)
                .addProperty("width", &RenderingWindowConfiguration::width)
                .addProperty("height", &RenderingWindowConfiguration::height)
            .endClass()
            .beginClass<RenderingWindow>("RenderingWindow")
               .addProperty("id", &RenderingWindow::id)
            .endClass()
            .beginClass<RenderingWindowManager>("RenderingWindowManager")
                .addConstructor<void(Context&, unique_id)>()
               .addFunction("create", &RenderingWindowManager::create)
               .addFunction("show", &RenderingWindowManager::show)
            .endClass()
        .endNamespace();
    }

    void RenderingWindowManager::closing(const unique_id id) {
        auto& window = get(id);
        window.stopped = true;
        ctx.eventManager.push({window.id, static_cast<event_type>(RenderingWindowEventType::CLOSING)});
        if (destroy(id)) {
            ctx.exit = true;
        }
    }

    void RenderingWindowManager::resized(const unique_id id) const {
        const auto& window = get(id);
        if (window.stopped) { return; }
        ctx.eventManager.push({window.id, static_cast<event_type>(RenderingWindowEventType::RESIZED)});
    }

}