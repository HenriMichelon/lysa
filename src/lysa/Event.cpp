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

namespace lysa {

    void EventManager::push(const Event& e) {
        queue.push_back(e);
    }

    void EventManager::subscribe(const event_type type, EventHandler& handler) {
        handlers[type].push_back(std::move(handler));
    }

    void EventManager::_process() {
        for (const Event& e : queue) {
            const auto it = handlers.find(e.type);
            if (it != handlers.end()) {
                for (auto& handler : it->second) {
                    handler(e);
                }
            }
        }
        queue.clear();
    }

    void EventManager::_shutdown() {
        queue.clear();
        handlers.clear();
    }

    void EventManager::_init() {
        Lua::get().new_usertype<Event>(
            "Event",
            "id", &Event::id,
            "type", &Event::type
        );

        sol::table event_manager = Lua::get().create_named_table("EventManager");
        event_manager.set_function("push", [](const unique_id id, const event_type type) {
            push(Event{id, type});
        });
        event_manager.set_function("subscribe", [&](const event_type type, sol::function fn) {
            EventHandler handler = [fn](const Event& e) {
                const sol::function pfn = fn;
                const sol::protected_function_result r = pfn(e);
                if (!r.valid()) {
                    const sol::error err = r;
                    throw Exception("Lua error in EventManager.subscribe handler :", err.what());
                }
            };
            subscribe(type, handler);
        });
    }

}