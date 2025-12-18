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
        //! Event source or target, if any
        unique_id id{INVALID_ID};
        //! Event type name
        event_type type;
        //! Event payload
        std::any payload;
    };

    /**
     * @brief Callback signature for event handlers.
     * @param e The event received by the handler.
     */
    using EventHandlerCallback = std::function<void(const Event&)>;

    struct EventHandler {
        unique_id id;
        EventHandlerCallback fn;
    };

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
         * @param type The event kind.
         * @param handler Previously registered handler
         */
        void unsubscribe(const event_type& type, unique_id handler);

        /**
         * @brief Unsubscribe a C++ handler to a given event type and target id.
         * @param type The event kind
         * @param id The specific target id to filter on.
         * @param handler Previously registered handler
         */
        void unsubscribe(const event_type& type, unique_id id, unique_id handler);

#ifdef LUA_BINDING
        /**
         * @brief Subscribe a Lua handler to a given event type and target id.
         * @param type The event kind to listen to.
         * @param id The specific target id to filter on.
         * @param handler Lua function to be called with the event.
         */
        void subscribe(const event_type& type, unique_id id, const luabridge::LuaRef& handler);

        /**
         * @brief Subscribe a Lua handler to a given global event type
         * @param type The event kind to listen to.
         * @param handler Reference to a callable receiving the event.
         */
        void subscribe(const event_type& type, const luabridge::LuaRef& handler);

        void unsubscribe(const event_type& type,  unique_id id, const luabridge::LuaRef& handler);

        void unsubscribe(const event_type& type, const luabridge::LuaRef& handler);

#endif

        void _process();

        /**
         * Execute all handlers attached to a global event type
         */
        void fire(const Event& event);

        EventManager(size_t reservedCapacity);
        ~EventManager();

    private:
        // Pending of resources events waiting to be processed.
        std::vector<Event> queue;
        // Backup of resources events for processing without blocking too much the queue
        std::vector<Event> processingQueue;
        // Protect the resources events queue during data copy
        std::mutex queueMutex;
        // C++ subscribers to global events
        std::unordered_map<event_type, std::vector<EventHandler>> globalHandlers{};
        // Protect the global events subscribers map
        std::mutex globalHandlersMutex;
        // C++ subscribers to resources events
        std::unordered_map<event_type, std::unordered_map<unique_id, std::vector<EventHandler>>> handlers{};
        // Protect the resources events subscribers map
        std::mutex handlersMutex;
        std::atomic<unique_id> nextId{1};
#ifdef LUA_BINDING
        // Lua subscribers to global events
        std::unordered_map<event_type, std::vector<luabridge::LuaRef>> globalHandlersLua{};
        // Lua subscribers resources events
        std::unordered_map<event_type, std::unordered_map<unique_id, std::vector<luabridge::LuaRef>>> handlersLua{};
#endif
    };

}