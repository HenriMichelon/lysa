/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa;

#ifdef ECS_SCENES
import lysa.ecs.systems;
#endif
import lysa.renderers.scene_render_context;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& config) :
        ctx(config.backend,
            config.eventsReserveCapacity,
            config.commandsReserveCapacity,
            config.resourcesCapacity.samplers,
            config.virtualFsConfiguration
#ifdef LUA_BINDING
            ,config.luaConfiguration
#endif
            ),
        renderTargetManager(ctx, config.resourcesCapacity.renderTarget, config.framesInFlight),
        renderingWindowManager(ctx, config.resourcesCapacity.renderingWindow),
        imageManager(ctx, config.resourcesCapacity.images),
        imageTextureManager(ctx, config.resourcesCapacity.images),
        materialManager(ctx, config.resourcesCapacity.material),
        meshManager(ctx,
            config.resourcesCapacity.meshes,
            config.resourcesCapacity.vertices,
            config.resourcesCapacity.indices,
            config.resourcesCapacity.surfaces),
        sceneManager(ctx,
            config.asyncObjectUpdatesPerFrame,
            config.resourcesCapacity.lightsPerScene,
            config.resourcesCapacity.meshInstancesPerScene,
            config.resourcesCapacity.meshSurfacePerPipeline,
            config.resourcesCapacity.scenes,
            config.resourcesCapacity.shadowMapsPerScene,
            config.framesInFlight),
        globalDescriptors(ctx)
    {
        ctx.globalDescriptorLayout = globalDescriptors.getDescriptorLayout();
        ctx.globalDescriptorSet = globalDescriptors.getDescriptorSet();
        SceneRenderContext::createDescriptorLayouts(ctx.vireo, config.resourcesCapacity.shadowMapsPerScene);
#ifdef ECS_SCENES
        ctx.world.set<ecs::Context>({&ctx});
        ecsModules = std::make_unique<ecs::Modules>(ctx.world);
#endif
    }

    Lysa::~Lysa() {
        ctx.graphicQueue->waitIdle();
        ecsModules.reset();
        SceneRenderContext::destroyDescriptorLayouts();
    }

    void Lysa::run(
        const std::function<void(float)>& onProcess,
        const std::function<void(float)>& onPhysicsProcess,
        const std::function<void()>& onQuit) {
        while (!ctx.exit) {
            ctx.defer._process();
            ctx.threads._process();
            processPlatformEvents();
            ctx.events._process();
            if (ctx.samplers.isUpdateNeeded()) {
                ctx.samplers.update();
            }
            globalDescriptors.update();
#ifdef ECS_SCENES
            ctx.world.progress();
#endif

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
            if (onProcess) onProcess(static_cast<float>(accumulator / FIXED_DELTA_TIME));
        }
        ctx.defer._process();
        if (onQuit) onQuit();
    }

#ifdef LUA_BINDING
    void Lysa::runLua(
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