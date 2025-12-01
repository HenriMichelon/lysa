/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.viewport;

import lysa.exception;
import lysa.log;
import lysa.resources.locator;
import lysa.resources.render_target;

namespace lysa {

    Viewport::Viewport(Context& ctx, const ViewportConfiguration& configuration):
        Resource(ctx) {
        const RenderTarget& renderTarget = ctx.resourcesLocator.get<RenderTargetManager>().get(configuration.renderTarget);
        this->renderTarget = configuration.renderTarget;
        this->configuration = configuration;
        framesData.resize(renderTarget.getSwapChain()->getFramesInFlight());
        for (auto& frame : framesData) {
        }
        resize(renderTarget.getSwapChain()->getExtent());
    }

    Viewport::~Viewport() {
        framesData.clear();
    }

    ViewportManager::ViewportManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(ctx, capacity) {
        ctx.resourcesLocator.enroll(*this);
    }

    void Viewport::resize(const vireo::Extent &extent) {
        if (configuration.viewport.width == 0.0f || configuration.viewport.height == 0.0f) {
            viewport = vireo::Viewport{
                .width = static_cast<float>(extent.width),
                .height = static_cast<float>(extent.height)
            };
        }
        if (configuration.scissors.width == 0.0f || configuration.scissors.height == 0.0f) {
            scissors = vireo::Rect{
                .x = static_cast<int32>(viewport.x),
                .y = static_cast<int32>(viewport.y),
                .width = static_cast<uint32>(viewport.width),
                .height = static_cast<uint32>(viewport.height)
            };
        }
    }

    void Viewport::render(
        const Renderer& renderer,
        const vireo::CommandList& commandList,
        uint32 frameIndex) {
    }

    void ViewportManager::resize(const unique_id renderTarget, const vireo::Extent &extent) {
        for (const auto& viewport : getResources(renderTarget)) {
            viewport->resize(extent);
        }
    }

    void ViewportManager::destroyByRenderTarget(const unique_id renderTarget) {
        for (const auto& viewport : getResources(renderTarget)) {
            destroy(viewport->id);
        }
    }

}