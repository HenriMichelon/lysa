/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

namespace lysa {

    RenderingWindow::RenderingWindow(Context& ctx, const RenderingWindowConfiguration& config):
        ctx(ctx),
        renderTargetManager(ctx.res.get<RenderTargetManager>()) {
        openPlatformWindow(config);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::READY)});
    }

    RenderingWindow::~RenderingWindow() {
        if (!closed) {
            paused = false;
            _closing();
            close();
        }
    }

    void RenderingWindow::_input(const InputEvent& inputEvent) const {
        if (closed || paused) { return; }
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::INPUT), inputEvent});
    }

    void RenderingWindow::_closing() {
        if (closed || paused) { return; }
        closed = true;
        renderTargetManager.destroy(platformHandle);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::CLOSING)});
    }

    void RenderingWindow::_resized() const {
        if (closed || paused) { return; }
        renderTargetManager.resize(platformHandle);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::RESIZED)});
    }

}