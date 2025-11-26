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

    ViewportManager::ViewportManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(ctx, ID, capacity) {
    }

    Viewport ViewportManager::create(const ViewportConfiguration& configuration) {
        if (configuration.renderTarget == INVALID_ID) {
            throw Exception("ViewportConfiguration : parent render target not set");
        }
        const RenderTarget& renderTarget = ctx.resourcesLocator.get<RenderTargetManager>(RenderTargetManager::ID).get(configuration.renderTarget);

        auto& viewport = allocate(std::make_unique<Viewport>(ctx));
        viewport.renderTarget = configuration.renderTarget;
        viewport.configuration = configuration;
        viewport.framesData.resize(renderTarget.swapChain->getFramesInFlight());
        for (auto& frame : viewport.framesData) {
        }
        resize(viewport, renderTarget.swapChain->getExtent());
        return viewport;
    }

    void ViewportManager::resize(Viewport& viewport, const vireo::Extent &extent) const {
        if (viewport.configuration.viewport.width == 0.0f || viewport.configuration.viewport.height == 0.0f) {
            viewport.viewport = vireo::Viewport{
                .width = static_cast<float>(extent.width),
                .height = static_cast<float>(extent.height)
            };
        }
        if (viewport.configuration.scissors.width == 0.0f || viewport.configuration.scissors.height == 0.0f) {
            viewport.scissors = vireo::Rect{
                .x = static_cast<int32>(viewport.viewport.x),
                .y = static_cast<int32>(viewport.viewport.y),
                .width = static_cast<uint32>(viewport.viewport.width),
                .height = static_cast<uint32>(viewport.viewport.height)
            };
        }
    }

    void ViewportManager::resize(const unique_id renderTarget, const vireo::Extent &extent) {
        for (auto& viewport : getResources()) {
            if (viewport->renderTarget != renderTarget) continue;
            resize(*viewport, extent);
        }
    }

    void ViewportManager::destroyAll(const unique_id renderTarget) {
        for (auto& viewport : getResources()) {
            if (viewport->renderTarget != renderTarget) continue;
            destroy(viewport->id);
        }
    }

    void ViewportManager::destroy(const unique_id id) {
        auto& viewport = get(id);
        viewport.framesData.clear();
        _release(viewport.id);
    }

    void ViewportManager::update(const unique_id renderTarget, const uint32 frameIndex) const {
    }

    void ViewportManager::prepare(const unique_id renderTarget, const uint32 frameIndex) const {
    }

    void ViewportManager::render(const unique_id renderTarget, const uint32 frameIndex) const {
    }

    void ViewportManager::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginClass<ViewportConfiguration>("ViewportConfiguration")
                .addConstructor<void()>()
        .       addProperty("render_target", &ViewportConfiguration::renderTarget)
        .       addProperty("viewport", &ViewportConfiguration::viewport)
        .       addProperty("scissors", &ViewportConfiguration::scissors)
            .endClass()
            .beginClass<Viewport>("Viewport")
               .addProperty("id", &Viewport::id)
        .       addProperty("render_target", &Viewport::renderTarget)
            .endClass()
            .beginClass<ViewportManager>("ViewportManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &ViewportManager::ID)
                .addFunction("create", &ViewportManager::create)
                .addFunction("get",
                    luabridge::nonConstOverload<const unique_id>(&ViewportManager::get),
                    luabridge::constOverload<const unique_id>(&ViewportManager::get)
                )
                .addFunction("destroyAll", &ViewportManager::destroyAll)
                .addFunction("destroy",
                  luabridge::overload<const unique_id>(&Manager::destroy))
            .endClass()
        .endNamespace();
    }

}