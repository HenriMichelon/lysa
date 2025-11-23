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

    using event_type = uint32;

    struct Event {
        unique_id id;
        event_type type;
    };

    using EventHandler = std::function<void(const Event&)>;

    class EventManager {
    public:
        void push(const Event& e);

        void subscribe(event_type type, EventHandler& handler);

        void subscribe(event_type type, luabridge::LuaRef handler);

        void _process();

        ~EventManager();

        static void _register(const Lua& lua);

    private:
        std::vector<Event> queue{};
        std::unordered_map<event_type, std::vector<EventHandler>> handlers{};
        std::unordered_map<event_type, std::vector<luabridge::LuaRef>> handlersLua{};
    };

}