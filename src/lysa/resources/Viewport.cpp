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
        const RenderTarget& renderTarget = ctx.resourcesLocator.get<RenderTargetManager>(RenderTargetManager::ID).get(configuration.renderTarget);
        this->renderTarget = renderTarget.id;
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
        ResourcesManager(ctx, ID, capacity) {
    }

    Viewport& ViewportManager::create(const ViewportConfiguration& configuration) {
        if (configuration.renderTarget == INVALID_ID) {
            throw Exception("ViewportConfiguration : parent render target not set");
        }
        return allocate(std::make_unique<Viewport>(ctx, configuration));
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
        .       addProperty("render_target", &Viewport::getRenderTarget)
            .endClass()
            .beginClass<ViewportManager>("ViewportManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &ViewportManager::ID)
                .addFunction("create", &ViewportManager::create)
                .addFunction("get",
                    luabridge::nonConstOverload<const unique_id>(&ViewportManager::get),
                    luabridge::constOverload<const unique_id>(&ViewportManager::get)
                )
                .addFunction("destroyAll", &ViewportManager::destroy)
                .addFunction("destroy",
                  luabridge::overload<const unique_id>(&Manager::destroy))
            .endClass()
        .endNamespace();
    }

}