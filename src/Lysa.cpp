/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
module lysa;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& config) :
        ctx(config.backend,
            config.resourcesCapacity.samplers,
            config.virtualFsConfiguration
#ifdef LUA_BINDING
            ,config.luaConfiguration
#endif
            ),
        viewportManager(ctx, config.resourcesCapacity.viewports),
        renderTargetManager(ctx, config.resourcesCapacity.renderTarget),
        renderingWindowManager(ctx, config.resourcesCapacity.renderingWindow),
        imageManager(ctx, config.resourcesCapacity.images),
        imageTextureManager(ctx, config.resourcesCapacity.images),
        materialManager(ctx, config.resourcesCapacity.material),
        meshManager(ctx,
            config.resourcesCapacity.meshes,
            config.resourcesCapacity.vertices,
            config.resourcesCapacity.indices,
            config.resourcesCapacity.surfaces)
    {
        ctx.world.set<components::Context>({&ctx});
        ctx.world.import<TransformModule>();
        ctx.world.import<MeshInstanceModule>();
    }

    Lysa::~Lysa() {
        ctx.graphicQueue->waitIdle();
    }

    void Lysa::run(
        const std::function<void(float)>& onProcess,
        const std::function<void(float)>& onPhysicsProcess,
        const std::function<void()>& onQuit) {
        while (!ctx.exit) {
            processPlatformEvents();
            ctx.events._process();
            if (ctx.samplers.isUpdateNeeded()) {
                ctx.samplers.update();
            }
            ctx.world.progress();

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

#ifdef LUA_BINDING
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
#endif

}