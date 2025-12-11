/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.ecs.systems;

import lysa.aabb;
import lysa.log;
import lysa.math;
import lysa.resources.mesh;
import lysa.resources.render_target;
import lysa.resources.scene_context;
import lysa.renderers.graphic_pipeline_data;

namespace lysa::ecs {

    Modules::Modules(flecs::world& w) {
        meshInstanceModule = w.import<MeshInstanceModule>();
        renderModule = w.import<RenderModule>();
        transformModule = w.import<TransformModule>();
    }

    Modules::~Modules() {
        transformModule.disable();
        renderModule.disable();
        meshInstanceModule.disable();
    }

    MeshInstanceModule::MeshInstanceModule(const flecs::world& w) {
        auto& meshManager = w.get<Context>().ctx->res.get<MeshManager>();
        auto& sceneManager = w.get<Context>().ctx->res.get<SceneManager>();
        w.module<MeshInstanceModule>();
        w.component<Visible>();
        w.component<CastShadows>();
        w.component<MeshInstance>();
        w.observer<const Scene, MeshInstance, const Transform>()
            .term_at(0).parent()
            .event(flecs::OnSet)
            .event(flecs::OnAdd)
            .each([&](const flecs::entity e, const Scene& sceneRef, MeshInstance& mi, const Transform& tr) {
                if (mi.mesh == INVALID_ID) { return; }
                auto& scene = sceneManager[sceneRef.scene];
                auto aabb = meshManager[mi.mesh].getAABB().toGlobal(tr.global);
                if (mi.meshInstance) {
                    mi.meshInstance->worldAABB = aabb;
                    mi.meshInstance->worldTransform = tr.global;
                    scene.updateInstance(mi.meshInstance);
                } else {
                    mi.meshInstance = std::make_shared<MeshInstanceDesc>(
                        meshManager[mi.mesh],
                        e.has<Visible>(),
                        e.has<CastShadows>(),
                        aabb,
                        tr.global,
                        sceneManager.getFramesInFlight());
                    scene.addInstance(mi.meshInstance, false);
                }
            });
        w.observer<const Scene, const MeshInstance, const Visible>()
           .term_at(0).parent()
           .event(flecs::OnAdd)
           .each([&](const Scene& sceneRef, const MeshInstance& mi, const Visible& _) {
               if (mi.mesh == INVALID_ID) { return; }
               if (mi.meshInstance) {
                   auto& scene = sceneManager[sceneRef.scene];
                   mi.meshInstance->visible = true;
                   scene.updateInstance(mi.meshInstance);
               }
           });
        w.observer<const Scene, const MeshInstance, const Visible>()
           .term_at(0).parent()
            .event(flecs::OnRemove)
            .each([&](const Scene& sceneRef, const MeshInstance& mi, const Visible& _) {
            if (mi.mesh == INVALID_ID) { return; }
            if (mi.meshInstance) {
                auto& scene = sceneManager[sceneRef.scene];
                mi.meshInstance->visible = false;
                scene.updateInstance(mi.meshInstance);
            }
        });
    }

    RenderModule::RenderModule(const flecs::world& w) {
        auto& renderTargetManager = w.get<Context>().ctx->res.get<RenderTargetManager>();
        auto& sceneManager = w.get<Context>().ctx->res.get<SceneManager>();
        w.module<RenderModule>();
        w.component<Scene>();
        w.component<SceneRef>();
        w.component<Camera>();
        w.component<CameraRef>();
        w.component<Viewport>();
        w.component<RenderTarget>();
        w.observer<Camera>()
            .event(flecs::OnSet)
            .event(flecs::OnAdd)
            .each([&](Camera& cp) {
                if (cp.isPerspective) {
                    cp.projection = perspective(radians(cp.fov), cp.aspectRatio, cp.near, cp.far);
                } else {
                    cp.projection = orthographic(cp.left, cp.right, cp.top, cp.bottom, cp.near, cp.far);
                }
            });
        w.system<const RenderTarget>()
            .kind(flecs::OnUpdate)
            .each([&](const flecs::entity e, const RenderTarget& rt) {
                const auto& renderTarget = renderTargetManager[rt.renderTarget];
                if (renderTarget.isPaused()) { return; }
                auto views = std::list<RenderView>();
                e.children([&](const flecs::entity child) {
                    if (child.has<CameraRef>() && child.has<SceneRef>()) {
                        auto& cameraRef = child.get<CameraRef>();
                        auto& sceneRef = child.get<SceneRef>();
                        auto& scene = sceneManager[sceneRef.scene.get<Scene>().scene];
                        const auto camera = cameraRef.camera.get<Camera>();
                        const auto cameraTransform = cameraRef.camera.get<Transform>().global;
                        const auto cameraDesc = CameraDesc{
                            .position = cameraTransform[3].xyz,
                            .transform = cameraTransform,
                            .projection = camera.projection
                        };
                        Viewport viewport;
                        if (child.has<Viewport>()) {
                            viewport = child.get<Viewport>();
                        }
                        views.push_back({viewport.viewport, viewport.scissors, cameraDesc, scene});
                    }
                });
                renderTarget.render(views);
            });
        }


}
