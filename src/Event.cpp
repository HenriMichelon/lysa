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
import lysa.utils;

namespace lysa {

    void EventManager::push(const Event& e) {
        auto lock = std::lock_guard(queueMutex);
        queue.push_back(e);
    }

    unique_id EventManager::subscribe(const event_type& type, const unique_id id, EventHandlerCallback callback) {
        auto lock = std::lock_guard(handlersMutex);
        const auto hid = nextId++;
        handlers[type][id].push_back({hid, std::move(callback)});
        return hid;
    }

    unique_id EventManager::subscribe(const event_type& type, EventHandlerCallback callback) {
        auto lock = std::lock_guard(globalHandlersMutex);
        const auto hid = nextId++;
        globalHandlers[type].push_back({hid, std::move(callback)});
        return hid;
    }

    void EventManager::unsubscribe(const unique_id id) {
        {
            auto lock = std::lock_guard(globalHandlersMutex);
            for (auto& handlers : globalHandlers) {
                std::erase_if(handlers.second,[&](const EventHandler& e) { return e.id == id; });
            }
#ifdef LUA_BINDING
            for (auto& handlers : globalHandlersLua) {
                std::erase_if(handlers.second,[&](const EventHandlerLua& e) { return e.id == id; });
            }
#endif
        }
        {
            auto lock = std::lock_guard(handlersMutex);
            for (auto& types : handlers) {
                for (auto& handlers : types.second) {
                    std::erase_if(handlers.second,[&](const EventHandler& e) { return e.id == id; });
                }
            }
#ifdef LUA_BINDING
            for (auto& types : handlersLua) {
                for (auto& handlers : types.second) {
                    std::erase_if(handlers.second,[&](const EventHandlerLua& e) { return e.id == id; });
                }
            }
#endif
        }
    }

#ifdef LUA_BINDING
    unique_id EventManager::subscribe(const event_type& type, const unique_id id, const luabridge::LuaRef& handler) {
        auto lock = std::lock_guard(handlersMutex);
        const auto hid = nextId++;
        handlersLua[type][id].push_back({hid, handler});
        return hid;
    }

    unique_id EventManager::subscribe(const event_type& type, const luabridge::LuaRef& handler) {
        auto lock = std::lock_guard(globalHandlersMutex);
        const auto hid = nextId++;
        globalHandlersLua[type].push_back({hid, handler});
        return hid;
    }

#endif

    void EventManager::fire(const Event& event) {
        {
            const auto itType = globalHandlers.find(event.type);
            if (itType != globalHandlers.end()) {
                std::vector<EventHandler> queue;
                {
                    auto lock = std::lock_guard(globalHandlersMutex);
                    queue = itType->second;
                }
                for (auto& handler : queue) {
                    handler.fn(const_cast<Event&>(event));
                }
            }
        }
#ifdef LUA_BINDING
        {
            const auto itType = globalHandlersLua.find(event.type);
            if (itType != globalHandlersLua.end()) {
                std::vector<EventHandlerLua> queue;
                {
                    auto lock = std::lock_guard(globalHandlersMutex);
                    queue = itType->second;
                }
                for (auto& handler : queue) {
                    handler.fn(event);
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
        for (Event& e : processingQueue) {
            {
                const auto itType = handlers.find(e.type);
                if (itType != handlers.end()) {
                    const auto itId = itType->second.find(e.id);
                    if (itId != itType->second.end()) {
                        for (auto& handler : itId->second) {
                            handler.fn(e);
                            if (e.consumed) {
                                break;
                            }
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
                            handler.fn(e);
                            if (e.consumed) {
                                break;
                            }
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
        globalHandlers.clear();
#ifdef LUA_BINDING
        handlersLua.clear();
        globalHandlersLua.clear();
#endif
    }

}