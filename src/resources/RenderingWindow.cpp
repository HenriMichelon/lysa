/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;

namespace lysa {

    RenderingWindow::RenderingWindow(Context& ctx, const RenderingWindowConfiguration& config) :
        handle(openPlatformWindow(config)),
        renderTarget(ctx, config.renderTargetConfiguration, handle) {
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::READY)});
    }

    RenderingWindow::~RenderingWindow() {
        if (!closed) {
            renderTarget.setPause(false);
            _closing();
            close();
        }
    }

    void RenderingWindow::_input(const InputEvent& inputEvent) const {
        if (closed || renderTarget.isPaused()) { return; }
        renderTarget.getContext().events.push({id, static_cast<event_type>(RenderingWindowEvent::INPUT), inputEvent});
    }

    void RenderingWindow::_closing() {
        if (closed || renderTarget.isPaused()) { return; }
        renderTarget.setPause(true);
        closed = true;
        renderTarget.getContext().events.push({id, static_cast<event_type>(RenderingWindowEvent::CLOSING)});
    }

    void RenderingWindow::_resized(const Rect& rect) {
        if (closed || renderTarget.isPaused()) { return; }
        this->rect = rect;
        renderTarget.resize();
        renderTarget.getContext().events.push({
            id,
            static_cast<event_type>(RenderingWindowEvent::RESIZED),
            float2{renderTarget.getWidth(), renderTarget.getHeight()}
        });
    }

}