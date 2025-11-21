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

    void RenderingWindowManager::close(const unique_id id) {
        auto& window = get(id);
        window.stopped = true;
        if (window.onEvent) window.onEvent({id, RenderingWindowEventType::CLOSE});
        if (destroy(id)) {
            renderer.quit();
        }
    }

    void RenderingWindowManager::resize(const unique_id id) const {
        const auto& window = get(id);
        if (window.stopped) { return; }
        if (window.onEvent) window.onEvent({window.id, RenderingWindowEventType::RESIZE});
    }

}