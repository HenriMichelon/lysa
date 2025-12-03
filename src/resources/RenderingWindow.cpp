/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

import lysa.exception;
import lysa.log;
import lysa.resources.locator;
import lysa.resources.render_target;

namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Context& ctx,const size_t capacity) :
        ResourcesManager(ctx, capacity) {
        ctx.resources.enroll(*this);
    }

    void RenderingWindow::_closing() {
        if (stopped) { return; }
        stopped = true;
        ctx.resources.get<RenderTargetManager>().destroy(platformHandle);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::CLOSING)});
        ctx.resources.get<RenderingWindowManager>().destroy(id);
    }

    void RenderingWindow::_resized() const {
        if (stopped) { return; }
        ctx.resources.get<RenderTargetManager>().resize(platformHandle);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::RESIZED)});
    }

    RenderingWindow& RenderingWindowManager::create(const RenderingWindowConfiguration& configuration) {
        auto& instance = ResourcesManager::create(ctx, configuration);
        ctx.events.push({instance.id, static_cast<event_type>(RenderingWindowEvent::READY)});
        return instance;
    }

    void RenderingWindowManager::destroy(const unique_id id) {
        const auto& window = get(id);
        window.close();
        Manager::destroy(id);
    }

}