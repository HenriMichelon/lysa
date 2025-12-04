/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.event;

import std;
#ifdef LUA_BINDING
import lysa.lua;
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
        //! Target object of resource unique id
        unique_id id;
        //! Event type name
        event_type type;
    };

    /**
     * @brief Callback signature for event handlers.
     * @param e The event received by the handler.
     */
    using EventHandler = std::function<void(const Event&)>;

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
         * @param handler Reference to a callable receiving the event.
         */
        void subscribe(const event_type& type, unique_id id, const EventHandler& handler);

#ifdef LUA_BINDING
        /**
         * @brief Subscribe a Lua handler to a given event type and target id.
         * @param type The event kind to listen to.
         * @param id The specific target id to filter on.
         * @param handler Lua function to be called with the event.
         */
        void subscribe(const event_type& type, unique_id id, luabridge::LuaRef handler);
#endif

        void _process();

        EventManager(size_t reservedCapacity);
        ~EventManager();

    private:
        // Pending events waiting to be processed.
        std::vector<Event> queue;
        // Backup of events for processing without blocking too much the queue
        std::vector<Event> processingQueue;
        std::mutex queueMutex;
        // C++ subscribers keyed by event type then id.
        std::unordered_map<event_type, std::unordered_map<unique_id, std::vector<EventHandler>>> handlers{};
#ifdef LUA_BINDING
        // Lua subscribers keyed by event type then id.
        std::unordered_map<event_type, std::unordered_map<unique_id, std::vector<luabridge::LuaRef>>> handlersLua{};
#endif
    };

}