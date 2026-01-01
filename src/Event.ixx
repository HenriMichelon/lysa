/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.event;

#ifdef LUA_BINDING
import lua_bridge;
#endif
import lysa.types;

export namespace lysa {

    /**
     * @brief Alias representing an application-defined event kind.
     */
    using event_type = std::string;

    /**
     * @brief %Event message.
     */
    struct Event {
        /** @brief Event source or target, if any. */
        unique_id id{INVALID_ID};
        /** @brief Event type name. */
        event_type type;
        /** @brief Event payload. */
        std::any payload;
    };

    /**
     * @brief Callback signature for event handlers.
     * @param e The event received by the handler.
     */
    using EventHandlerCallback = std::function<void(const Event&)>;

    /**
     * @brief Internal structure for an event handler.
     */
    struct EventHandler {
        /** @brief The unique identifier of the handler. */
        unique_id id;
        /** @brief The callback function to be executed. */
        EventHandlerCallback fn;
    };

#ifdef LUA_BINDING
    /**
     * @brief Internal structure for a Lua event handler.
     */
    struct EventHandlerLua {
        /** @brief The unique identifier of the handler. */
        unique_id id;
        /** @brief The callback function to be executed. */
        luabridge::LuaRef fn;
    };
#endif

    /**
     * @brief Simple event manager.
     *
     * Supports both C++ and %Lua handlers. Events are queued via @ref push
     * and delivered in FIFO order each frame.
     * The C++ handlers are called before the %Lua ones.
     */
    class EventManager {
    public:
        /**
         * @brief Enqueue an event to be delivered on next processing.
         * @param e The event to push into the queue.
         */
        void push(const Event& e);

        /**
         * @brief Subscribe a C++ handler to a given event type and target id.
         * @param type The event kind to listen to.
         * @param id The specific target id to filter on.
         * @param callback Reference to a callable receiving the event.
         */
        unique_id subscribe(const event_type& type, unique_id id, EventHandlerCallback callback);

        /**
         * @brief Subscribe a C++ handler to a given global event type
         * @param type The event kind to listen to.
         * @param callback Reference to a callable receiving the event.
         */
        unique_id subscribe(const event_type& type, EventHandlerCallback callback);

        /**
         * @brief Unsubscribe a C++ handler to a given global event type
         * @param id Previously registered handler
         */
        void unsubscribe(unique_id id);


#ifdef LUA_BINDING
        /**
         * @brief Subscribe a Lua handler to a given event type and target id.
         * @param type The event kind to listen to.
         * @param id The specific target id to filter on.
         * @param handler Lua function to be called with the event.
         */
        unique_id subscribe(const event_type& type, unique_id id, const luabridge::LuaRef& handler);

        /**
         * @brief Subscribe a Lua handler to a given global event type
         * @param type The event kind to listen to.
         * @param handler Reference to a callable receiving the event.
         */
        unique_id subscribe(const event_type& type, const luabridge::LuaRef& handler);

#endif

        void _process();

        /**
         * @brief Immediately deliver an event to all interested handlers.
         * @param event The event to fire.
         */
        void fire(const Event& event);

        /**
         * @brief Creates an EventManager with an initial capacity for the event queue.
         * @param reservedCapacity The initial number of events to reserve space for.
         */
        EventManager(size_t reservedCapacity);

        ~EventManager();

    private:
        /*Pending events waiting to be processed. */
        std::vector<Event> queue;
        /* Backup of events for processing without blocking the main queue. */
        std::vector<Event> processingQueue;
        /* Protects the event queue during data operations. */
        std::mutex queueMutex;
        /* C++ subscribers to global events. */
        std::unordered_map<event_type, std::vector<EventHandler>> globalHandlers{};
        /* Protects the global events subscribers map. */
        std::mutex globalHandlersMutex;
        /* C++ subscribers to targeted events. */
        std::unordered_map<event_type, std::unordered_map<unique_id, std::vector<EventHandler>>> handlers{};
        /* Protects the targeted events subscribers map. */
        std::mutex handlersMutex;
        /* The identifier to be assigned to the next subscriber. */
        std::atomic<unique_id> nextId{1};
#ifdef LUA_BINDING
        /* Lua subscribers to global events. */
        std::unordered_map<event_type, std::vector<EventHandlerLua>> globalHandlersLua{};
        /* Lua subscribers to targeted events. */
        std::unordered_map<event_type, std::unordered_map<unique_id, std::vector<EventHandlerLua>>> handlersLua{};
#endif
    };

}