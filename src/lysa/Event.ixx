/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
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

        // void subscribeLua(event_type type, sol::function fn);

        void _process();

        ~EventManager();

        void _register(Lua& lua);

    private:
        std::vector<Event> queue{};
        std::unordered_map<event_type, std::vector<EventHandler>> handlers{};
        // std::unordered_map<event_type, std::vector<sol::function>> handlersLua{};
    };

}