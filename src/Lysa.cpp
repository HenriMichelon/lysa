/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa;

import lysa.renderers.scene_frame_data;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& config) :
        ctx(config.backend,
            config.eventsReserveCapacity,
            config.commandsReserveCapacity,
            config.resourcesCapacity.samplers,
            config.virtualFsConfiguration
            ),
        fixedDeltaTime(config.deltaTime),
        cameraManager(ctx, config.resourcesCapacity.camera),
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
        meshInstanceManager(ctx, config.resourcesCapacity.meshesInstances),
        sceneManager(ctx,
            config.resourcesCapacity.scenes,
            config.asyncObjectUpdatesPerFrame,
            config.resourcesCapacity.lightsPerScene,
            config.resourcesCapacity.meshInstancesPerScene,
            config.resourcesCapacity.meshSurfacePerPipeline,
            config.resourcesCapacity.shadowMapsPerScene,
            config.framesInFlight),
        renderViewManager(ctx, config.resourcesCapacity.renderViews),
        globalDescriptors(ctx)
    {
        ctx.globalDescriptorLayout = globalDescriptors.getDescriptorLayout();
        ctx.globalDescriptorSet = globalDescriptors.getDescriptorSet();
        SceneFrameData::createDescriptorLayouts(ctx.vireo, config.resourcesCapacity.shadowMapsPerScene);
    }

    Lysa::~Lysa() {
        ctx.graphicQueue->waitIdle();
        SceneFrameData::destroyDescriptorLayouts();
        Renderpass::destroyShaderModules();
        FrustumCulling::cleanup();
    }

    void Lysa::uploadData() {
        if (ctx.samplers.isUpdateNeeded()) {
            ctx.samplers.update();
        }
        materialManager.flush();
        meshManager.flush();
        globalDescriptors.update();
    }

    void Lysa::run() {
        while (!ctx.exit) {
            uploadData();
            ctx.defer._process();
            ctx.threads._process();
            processPlatformEvents();
            ctx.events._process();
            uploadData();

            // https://gafferongames.com/post/fix_your_timestep/
            const auto newTime = std::chrono::duration_cast<std::chrono::duration<double>>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
            auto frameTime = newTime - currentTime;
            if (frameTime > 0.25) frameTime = 0.25; // Note: Max frame time to avoid spiral of death
            currentTime = newTime;
            accumulator += frameTime;
            while (accumulator >= fixedDeltaTime) {
                ctx.events.fire({
                    .type = MainLoopEvent::PHYSICS_PROCESS,
                    .payload = fixedDeltaTime,
                });
                accumulator -= fixedDeltaTime;
            }
            ctx.events.fire({
                .type = MainLoopEvent::PROCESS,
                .payload = accumulator / fixedDeltaTime,
            });
        }
        ctx.defer._process();
        ctx.events.fire({
            .type = MainLoopEvent::QUIT,
        });
    }

}