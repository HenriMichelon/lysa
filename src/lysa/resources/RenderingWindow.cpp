/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

import lysa.resources.locator;

namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Renderer& renderer, const unique_id capacity) :
        ResourcesManager(capacity),
        renderer{renderer} {
        ResourcesLocator::enroll(RENDERING_WINDOW, *this);
    }

    void RenderingWindowManager::closing(const unique_id id) {
        auto& window = get(id);
        window.stopped = true;
        EventManager::push({window.id, static_cast<event_type>(RenderingWindowEventType::CLOSING)});
        if (destroy(id)) {
            renderer.quit();
        }
    }

    void RenderingWindowManager::resized(const unique_id id) const {
        const auto& window = get(id);
        if (window.stopped) { return; }
        EventManager::push({window.id, static_cast<event_type>(RenderingWindowEventType::RESIZED)});
    }

}