/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
module lysa.event;

import lysa.exception;
import lysa.log;

namespace lysa {

    void EventManager::push(const Event& e) {
        Log::trace();
        queue.push_back(e);
    }

    void EventManager::subscribe(const event_type type, EventHandler& handler) {
        Log::trace();
        handlers[type].push_back(std::move(handler));
    }

    void EventManager::subscribeLua(const event_type type, sol::function fn) {
        Log::trace();
        handlersLua[type].push_back(std::move(fn));
    }
    void EventManager::_process() {
        for (const Event& e : queue) {
            Log::trace();
            const auto it = handlers.find(e.type);
            if (it != handlers.end()) {
                for (auto& handler : it->second) {
                    handler(e);
                }
            }
        }
        queue.clear();
    }

    EventManager::~EventManager() {
        queue.clear();
        handlers.clear();
    }

    void EventManager::_register(Lua& lua) {
        lua.get().new_usertype<Event>(
            "Event",
            "id", &Event::id,
            "type", &Event::type
        );

        sol::table event_manager = lua.get().create_named_table("EventManager");
        event_manager.set_function("push", &EventManager::push);
        event_manager.set_function("subscribe", &EventManager::subscribeLua);
    }

}