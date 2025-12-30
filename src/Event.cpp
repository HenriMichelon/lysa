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
        const auto handler = EventHandler{nextId++, std::move(callback)};
        handlers[type][id].push_back(handler);
        return handler.id;
    }

    unique_id EventManager::subscribe(const event_type& type, EventHandlerCallback callback) {
        auto lock = std::lock_guard(globalHandlersMutex);
        const auto handler = EventHandler{nextId++, std::move(callback)};
        globalHandlers[type].push_back(handler);
        return handler.id;
    }

    void EventManager::unsubscribe(const event_type& type,  const unique_id id, const unique_id handler) {
        auto lock = std::lock_guard(handlersMutex);
        const auto it = handlers.find(type);
        if (it == handlers.end()) return;

        auto& perId = it->second;
        const auto itId = perId.find(id);
        if (itId == perId.end()) return;

        auto& vec = itId->second;
        std::erase_if(vec,[&](const EventHandler& e) { return e.id == handler; });

        if (vec.empty()) {
            perId.erase(itId);
            if (perId.empty()) {
                handlers.erase(it);
            }
        }
    }

    void EventManager::unsubscribe(const event_type& type, const unique_id handler) {
        auto lock = std::lock_guard(globalHandlersMutex);
        const auto it = globalHandlers.find(type);
        if (it == globalHandlers.end()) return;
        auto& vec = it->second;
        std::erase_if(vec,[&](const EventHandler& e) { return e.id == handler; });
        if (vec.empty()) {
            globalHandlers.erase(it);
        }
    }

#ifdef LUA_BINDING
    void EventManager::subscribe(const event_type& type, const unique_id id, const luabridge::LuaRef& handler) {
        auto lock = std::lock_guard(handlersMutex);
        handlersLua[type][id].push_back(handler);
    }

    void EventManager::subscribe(const event_type& type, const luabridge::LuaRef& handler) {
        auto lock = std::lock_guard(globalHandlersMutex);
        globalHandlersLua[type].push_back(handler);
    }

    void EventManager::unsubscribe(const event_type& type,  const unique_id id, const luabridge::LuaRef& handler) {
        auto lock = std::lock_guard(handlersMutex);
        const auto it = handlersLua.find(type);
        if (it == handlersLua.end()) return;

        auto& perId = it->second;
        const auto itId = perId.find(id);
        if (itId == perId.end()) return;

        auto& vec = itId->second;
        std::erase_if(vec,[&](const luabridge::LuaRef& e) { return e == handler; });

        if (vec.empty()) {
            perId.erase(itId);
            if (perId.empty()) {
                handlersLua.erase(it);
            }
        }
    }

    void EventManager::unsubscribe(const event_type& type, const luabridge::LuaRef& handler) {
        auto lock = std::lock_guard(globalHandlersMutex);
        const auto it = globalHandlersLua.find(type);
        if (it == globalHandlersLua.end()) return;
        auto& vec = it->second;
        std::erase_if(vec,[&](const luabridge::LuaRef& e) { return e == handler; });
        if (vec.empty()) {
            globalHandlersLua.erase(it);
        }
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
                    handler.fn(event);
                }
            }
        }
#ifdef LUA_BINDING
        {
            const auto itType = globalHandlersLua.find(event.type);
            if (itType != globalHandlersLua.end()) {
                std::vector<luabridge::LuaRef> queue;
                {
                    auto lock = std::lock_guard(globalHandlersMutex);
                    queue = itType->second;
                }
                for (auto& handler : queue) {
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
                            handler.fn(e);
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
        globalHandlers.clear();
#ifdef LUA_BINDING
        handlersLua.clear();
        globalHandlersLua.clear();
#endif
    }

}