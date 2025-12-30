/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.rendering_window;


namespace lysa {

    RenderingWindowManager::RenderingWindowManager(Context& ctx,const size_t capacity) :
        ResourcesManager(ctx, capacity, "RenderingWindowManager") {
        ctx.res.enroll(*this);
    }

    void RenderingWindow::_input(const InputEvent& inputEvent) const {
        if (stopped) { return; }
        renderTargetManager.input(platformHandle, inputEvent);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::INPUT), inputEvent});
    }

    void RenderingWindow::_closing() {
        if (stopped) { return; }
        stopped = true;
        renderTargetManager.destroy(platformHandle);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::CLOSING)});
        ctx.res.get<RenderingWindowManager>().destroy(id);
    }

    void RenderingWindow::_resized() const {
        if (stopped) { return; }
        renderTargetManager.resize(platformHandle);
        ctx.events.push({id, static_cast<event_type>(RenderingWindowEvent::RESIZED)});
    }

    RenderingWindow& RenderingWindowManager::create(const RenderingWindowConfiguration& configuration) {
        auto& instance = ResourcesManager::create(configuration);
        ctx.events.push({instance.id, static_cast<event_type>(RenderingWindowEvent::READY)});
        return instance;
    }

    bool RenderingWindowManager::destroy(const unique_id id) {
        const auto& window = (*this)[id];
        if (window.refCounter == 0) {
            window.close();
        }
        return ResourcesManager::destroy(id);
    }

}