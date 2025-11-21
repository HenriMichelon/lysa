/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.event;

namespace lysa {

    void EventManager::push(const Event& e) {
        queue.push_back(e);
    }

    void EventManager::subscribe(const event_type type, EventHandler& handler) {
        handlers[type].push_back(std::move(handler));
    }

    void EventManager::process() {
        for (const Event& e : queue) {
            const auto it = handlers.find(e.type);
            if (it != handlers.end()) {
                for (auto& handler : it->second) {
                    handler(e);
                }
            }
        }
        queue.clear();
    }

    void EventManager::reset() {
        queue.clear();
        handlers.clear();
    }

}