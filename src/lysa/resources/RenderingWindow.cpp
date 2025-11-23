/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
module lysa.resources.rendering_window;

import lysa.resources.locator;

import lysa.log;
import lysa.lua;

namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(capacity),
        ctx{ctx} {
        Log::log("foo");
        ctx.resourcesLocator.enroll(RENDERING_WINDOW, *this);
    }

    void RenderingWindowManager::_register(Lua& lua) {
        auto& l = lua.get();
        l.new_enum("RenderingWindowMode",
            "WINDOWED",   RenderingWindowMode::WINDOWED,
            "WINDOWED_FULLSCREEN", RenderingWindowMode::WINDOWED_FULLSCREEN,
            "WINDOWED_MAXIMIZED", RenderingWindowMode::WINDOWED_MAXIMIZED,
            "FULLSCREEN", RenderingWindowMode::FULLSCREEN
        );
        l.new_enum("RenderingWindowEventType",
            "READY", RenderingWindowEventType::READY,
            "CLOSING", RenderingWindowEventType::CLOSING,
            "RESIZED", RenderingWindowEventType::RESIZED
        );
        l.new_usertype<RenderingWindowEvent>(
            "RenderingWindowEvent",
            "id", &RenderingWindowEvent::id,
            "type", &RenderingWindowEvent::type
        );
        l.new_usertype<RenderingWindowConfiguration>(
            "RenderingWindowConfiguration",
            sol::constructors<
                RenderingWindowConfiguration(),
                RenderingWindowConfiguration(const std::string&, RenderingWindowMode, int32_t, int32_t, uint32_t, uint32_t, int32_t)
            >(),
            "title",   &RenderingWindowConfiguration::title,
            "mode",    &RenderingWindowConfiguration::mode,
            "x",       &RenderingWindowConfiguration::x,
            "y",       &RenderingWindowConfiguration::y,
            "width",   &RenderingWindowConfiguration::width,
            "height",  &RenderingWindowConfiguration::height,
            "monitor", &RenderingWindowConfiguration::monitor
        );
        l.new_usertype<RenderingWindow>("RenderingWindow",
            "id", &RenderingWindow::id,
            "x", &RenderingWindow::x,
            "y", &RenderingWindow::y,
            "width", &RenderingWindow::width,
            "height", &RenderingWindow::height,
            "stopped", &RenderingWindow::stopped,
            "platform_handle", &RenderingWindow::platformHandle
        );
        l.new_usertype<RenderingWindowManager>(
            "RenderingWindowManager",
            sol::constructors<RenderingWindowManager(Context&, unique_id)>(),
            "create", &RenderingWindowManager::create,
            "show", &RenderingWindowManager::show,
            "get", &RenderingWindowManager::getById
        );
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