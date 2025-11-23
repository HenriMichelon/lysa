/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.event;

import std;
import lysa.lua;
import lysa.types;

export namespace lysa {

    /**
     * @brief Alias representing an application-defined event kind.
     * specific subscribers.
     */
    using event_type = std::string;

    /**
     * @brief Event message.
     */
    struct Event {
        //! Target object of resource unique id
        unique_id id;
        //! Event type
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
     * Supports both C++ and Lua handlers. Events are queued via @ref push
     * and delivered in FIFO order each frame.
     * The C++ handlers are called before the Lua ones.
     */
    class EventManager {
    public:
        /**
         * @brief Enqueue an event to be delivered on next processing.
         * @param e The event to push into the queue.
         */
        void push(const Event& e);

        /**
         * @brief Subscribe a C++ handler to a given event type.
         * @param type The event kind to listen to.
         * @param handler Reference to a callable receiving the event.
         */
        void subscribe(const event_type& type, EventHandler& handler);

        /**
         * @brief Subscribe a Lua handler to a given event type.
         * @param type The event kind to listen to.
         * @param handler Lua function to be called with the event.
         */
        void subscribe(const event_type& type, luabridge::LuaRef handler);

        /**
         * @brief Deliver all queued events to matching subscribers.
         *
         * Calls C++ and Lua handlers registered for each event type,
         * in the order subscriptions were added.
         */
        void _process();

        /**
         * @brief Destructor that clears handlers and pending events.
         */
        ~EventManager();

        /**
         * @brief Register Event and EventManager bindings in the Lua state.
         * @param lua The Lua instance used to expose the API.
         */
        static void _register(const Lua& lua);

    private:
        /** @brief Pending events waiting to be processed. */
        std::vector<Event> queue{};
        /** @brief C++ subscribers keyed by event type. */
        std::unordered_map<event_type, std::vector<EventHandler>> handlers{};
        /** @brief Lua subscribers keyed by event type. */
        std::unordered_map<event_type, std::vector<luabridge::LuaRef>> handlersLua{};
    };

}