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
        auto lock = std::lock_guard(queueMutex);
        queue.push_back(e);
    }

    void EventManager::subscribe(const event_type& type, const unique_id id, const EventHandler& handler) {
        auto lock = std::lock_guard(handlersMutex);
        handlers[type][id].push_back(std::move(handler));
    }

    void EventManager::subscribe(const event_type& type, const EventHandler& handler) {
        auto lock = std::lock_guard(globalHandlersMutex);
        globalHandlers[type].push_back(std::move(handler));
    }

#ifdef LUA_BINDING
    void EventManager::subscribe(const event_type& type, const unique_id id, luabridge::LuaRef handler) {
        auto lock = std::lock_guard(handlersMutex);
        handlersLua[type][id].push_back(std::move(handler));
    }
    void EventManager::subscribe(const event_type& type, luabridge::LuaRef handler) {
        auto lock = std::lock_guard(globalHandlersMutex);
        globalHandlersLua[type].push_back(std::move(handler));
    }
#endif

    void EventManager::fire(const Event& event) {
        auto lock = std::lock_guard(globalHandlersMutex);
        {
            const auto itType = globalHandlers.find(event.type);
            if (itType != globalHandlers.end()) {
                for (auto& handler : itType->second) {
                    handler(event);
                }
            }
        }
#ifdef LUA_BINDING
        {
            const auto itType = globalHandlers.find(event.type);
            if (itType != globalHandlers.end()) {
                for (auto& handler : itType->second) {
                    handler(event);
                }
            }
        }
#endif
    }

    void EventManager::_process() {
        {
            auto lock = std::lock_guard(queueMutex);
            processingQueue.swap(queue);
        }
        auto lock = std::lock_guard(handlersMutex);
        for (const Event& e : processingQueue) {
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
        processingQueue.clear();
    }

    EventManager::EventManager(const size_t reservedCapacity) {
        queue.reserve(reservedCapacity);
        processingQueue.reserve(reservedCapacity);
    }

    EventManager::~EventManager() {
        queue.clear();
        handlers.clear();
#ifdef LUA_BINDING
        handlersLua.clear();
#endif
    }

}