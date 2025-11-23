/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
module lysa.event;

import lysa.exception;
import lysa.log;

namespace lysa {

    void EventManager::push(const Event& e) {
        queue.push_back(e);
    }

    void EventManager::subscribe(const event_type type, EventHandler& handler) {
        handlers[type].push_back(std::move(handler));
    }

    void EventManager::subscribe(const event_type type, luabridge::LuaRef handler) {
        handlersLua[type].push_back(std::move(handler));
    }

    void EventManager::_process() {
        for (const Event& e : queue) {
            {
                const auto it = handlers.find(e.type);
                if (it != handlers.end()) {
                    for (auto& handler : it->second) {
                        handler(e);
                    }
                }
            }
            {
                const auto it = handlersLua.find(e.type);
                if (it != handlersLua.end()) {
                    for (auto& handler : it->second) {
                        handler(e);
                    }
                }
            }
        }
        queue.clear();
    }

    EventManager::~EventManager() {
        queue.clear();
        handlers.clear();
    }

    void EventManager::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginClass<Event>("Event")
                .addProperty("id", &Event::id)
                .addProperty("type", &Event::type)
            .endClass()
            .beginClass<EventManager>("EventManager")
                .addFunction("push", &EventManager::push)
                .addFunction("subscribe", luabridge::overload<event_type, luabridge::LuaRef>(&EventManager::subscribe))
            .endClass()
        .endNamespace();

    }

}