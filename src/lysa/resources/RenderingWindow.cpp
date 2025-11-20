/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

namespace lysa {

    void RenderingWindowManager::close(RenderingWindow& window) {
        window.stopped = true;
        if (window.onEvent) window.onEvent({window.id, RenderingWindowEventType::CLOSE});
        if (destroy(window)) {
            renderer.quit();
        }

    }

    void RenderingWindowManager::resize(const RenderingWindow& window) const {
        if (window.stopped) { return; }
        if (window.onEvent) window.onEvent({window.id, RenderingWindowEventType::RESIZE});
    }

}