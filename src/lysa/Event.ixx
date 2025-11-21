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
        static void push(const Event& e);

        static void subscribe(event_type type, EventHandler& handler);

        static void _process();

        static void _init();

        static void _shutdown();

    private:
        static inline std::vector<Event> queue{};
        static inline std::unordered_map<event_type, std::vector<EventHandler>> handlers{};
    };

}