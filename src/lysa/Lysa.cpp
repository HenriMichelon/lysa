/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
module lysa;

import vireo.lua;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& lysaConfiguration) :
        ctx(lysaConfiguration.backend,
            lysaConfiguration.luaConfiguration,
            lysaConfiguration.virtualFsConfiguration),
        viewportManager(ctx, lysaConfiguration.resourcesCapacity.viewports),
        renderTargetManager(ctx, lysaConfiguration.resourcesCapacity.renderTarget) {
        _register(ctx);
    }

    Lysa::~Lysa() {
        ctx.graphicQueue->waitIdle();
        viewportManager.cleanup();
        renderTargetManager.cleanup();
    }

    void Lysa::run(
        const std::function<void(float)>& onProcess,
        const std::function<void(float)>& onPhysicsProcess,
        const std::function<void()>& onQuit) {
        while (!ctx.exit) {
            processPlatformEvents();

            // https://gafferongames.com/post/fix_your_timestep/
            const double newTime = std::chrono::duration_cast<std::chrono::duration<double>>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
            double frameTime = newTime - currentTime;
            if (frameTime > 0.25) frameTime = 0.25; // Note: Max frame time to avoid spiral of death
            currentTime = newTime;
            accumulator += frameTime;
            while (accumulator >= FIXED_DELTA_TIME) {
                if (onPhysicsProcess) { onPhysicsProcess(FIXED_DELTA_TIME); }
                accumulator -= FIXED_DELTA_TIME;
            }
            onProcess(static_cast<float>(accumulator / FIXED_DELTA_TIME));

            renderTargetManager.update();
            renderTargetManager.render();
        }
        if (onQuit) onQuit();
    }

    void Lysa::run(
        const luabridge::LuaRef& onProcess,
        const luabridge::LuaRef& onPhysicsProcess,
        const luabridge::LuaRef& onQuit) {
        run(
            [&](float dt){ onProcess(dt); },
            [&](float dt){ if (onPhysicsProcess) onPhysicsProcess(dt); },
            [&]{ if (onQuit) onQuit(); }
            );
    }

    void Lysa::_register(Context& ctx) {
        vireo::LuaBindings::_register(ctx.lua.get());
        EventManager::_register(ctx.lua);
        ctx.lua.beginNamespace()
            .beginClass<Context>("Context")
                .addProperty("exit", &Context::exit)
                .addProperty("vireo", +[](const Context* self) { return self->vireo; })
                .addProperty("virtual_fs",  +[](const Context* self) { return self->virtualFs; })
                .addProperty("event_manager", &Context::eventManager)
                .addProperty("resources_locator", &Context::resourcesLocator)
                .addProperty("graphic_queue", +[](const Context* self) { return self->graphicQueue; })
            .endClass()
            .addVariable("ctx", &ctx)
        .endNamespace();
        ResourcesLocator::_register(ctx.lua);
    }

}