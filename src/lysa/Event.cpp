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

    void EventManager::subscribe(const event_type& type, const unique_id id, EventHandler& handler) {
        handlers[type][id].push_back(std::move(handler));
    }

#ifdef LUA_BINDING
    void EventManager::subscribe(const event_type& type, const unique_id id, luabridge::LuaRef handler) {
        handlersLua[type][id].push_back(std::move(handler));
    }
#endif

    void EventManager::_process() {
        for (const Event& e : queue) {
            {
                const auto itType = handlers.find(e.type);
                if (itType != handlers.end()) {
                    const auto itId = itType->second.find(e.id);
                    if (itId != itType->second.end()) {
                        for (auto& handler : itId->second) {
                            handler(e);
                        }
                    }
                }
            }
#ifdef LUA_BINDING
            {
                const auto itType = handlersLua.find(e.type);
                if (itType != handlersLua.end()) {
                    const auto itId = itType->second.find(e.id);
                    if (itId != itType->second.end()) {
                        for (auto& handler : itId->second) {
                            handler(e);
                        }
                    }
                }
            }
#endif
        }
        queue.clear();
    }

    EventManager::~EventManager() {
        queue.clear();
        handlers.clear();
#ifdef LUA_BINDING
        handlersLua.clear();
#endif
    }

}