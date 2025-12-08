/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.viewport;

import lysa.exception;
import lysa.log;
import lysa.resources.render_target;

namespace lysa {

    Viewport::Viewport(Context& ctx, const ViewportConfiguration& configuration):
        ctx(ctx) {
        const RenderTarget& renderTarget = ctx.res.get<RenderTargetManager>().get(configuration.renderTarget);
        this->renderTarget = configuration.renderTarget;
        this->configuration = configuration;
        resize(renderTarget.getSwapChain()->getExtent());
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

    void Viewport::update(Renderer& renderer, uint32 frameIndex) {
        renderer.update(frameIndex);
    }

    void Viewport::prepare(
        Renderer& renderer,
        const vireo::CommandList& commandList,
        uint32 frameIndex) {
        //renderer.preRender(commandList, frameIndex);

    }

    void Viewport::render(
        Renderer& renderer,
        const vireo::CommandList& commandList,
        uint32 frameIndex) {
        //renderer.render(commandList, frameIndex);
    }

    ViewportManager::ViewportManager(Context& ctx, const size_t capacity) :
        ResourcesManager(ctx, capacity) {
        ctx.res.enroll(*this);
    }    void ViewportManager::destroyByRenderTarget(const unique_id renderTarget) {
        for (const auto& viewport : getResources(renderTarget)) {
            destroy(viewport->id);
        }
    }

    Viewport& ViewportManager::create(const ViewportConfiguration& configuration) {
        return ResourcesManager::create(ctx, configuration);
    }

    void ViewportManager::resize(const unique_id renderTarget, const vireo::Extent &extent) {
        for (const auto& viewport : getResources(renderTarget)) {
            viewport->resize(extent);
        }
    }

    void ViewportManager::update(const unique_id renderTarget, Renderer& renderer, uint32 frameIndex) {
        for (const auto& viewport : getResources(renderTarget)) {
            viewport->update(renderer, frameIndex);
        }
    }

    void ViewportManager::prepare(const unique_id renderTarget,
        Renderer& renderer,
        const vireo::CommandList& commandList,
        uint32 frameIndex) {
        for (const auto& viewport : getResources(renderTarget)) {
            viewport->prepare(renderer, commandList, frameIndex);
        }
    }

    void ViewportManager::render(const unique_id renderTarget,
        Renderer& renderer,
        const vireo::CommandList& commandList,
        uint32 frameIndex) {
        for (const auto& viewport : getResources(renderTarget)) {
            viewport->render(renderer, commandList, frameIndex);
        }
    }

}