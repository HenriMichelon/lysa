/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
module lysa;



namespace lysa {

    Lysa::Lysa(const LysaConfiguration& lysaConfiguration) :
        lua(lysaConfiguration.luaConfiguration),
        viewportManager(ctx, lysaConfiguration.resourcesCapacity.viewports),
        renderTargetManager(ctx, lysaConfiguration.resourcesCapacity.renderTarget) {

        // Graphic backend objects
        ctx.vireo = vireo::Vireo::create(lysaConfiguration.backend);
        ctx.graphicQueue = ctx.vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue"),

        // Lua bindings
        LuaVireo::_register(lua);
        EventManager::_register(lua);
        lua.beginNamespace()
            .beginClass<Context>("Context")
                .addProperty("exit", &Context::exit)
                .addProperty("event_manager", &Context::eventManager)
                .addProperty("resources_locator", &Context::resourcesLocator)
            .endClass()
            .addProperty("ctx", &ctx)
        .endNamespace();
        ResourcesLocator::_register(lua);
    }

    Lysa::~Lysa() {
        viewportManager.cleanup();
        renderTargetManager.cleanup();
        ctx.graphicQueue->waitIdle();
        ctx.vireo.reset();
    }

    void Lysa::run(
        const std::function<void(float)>& onProcess,
        const std::function<void(float)>& onPhysicsProcess,
        const std::function<void()>& onQuit) {
        while (!ctx.exit) {
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

            processPlatformEvents();
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

}