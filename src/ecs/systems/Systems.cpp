/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.ecs.systems;

import lysa.aabb;
import lysa.math;
import lysa.resources.mesh;
import lysa.resources.render_target;
import lysa.resources.scene_context;
import lysa.renderers.graphic_pipeline_data;

namespace lysa::ecs {

    void _register(flecs::world& w) {
        w.import<TransformModule>();
        w.import<RenderModule>();
        w.import<MeshInstanceModule>();
    }

    MeshInstanceModule::MeshInstanceModule(const flecs::world& w) {
        auto& meshManager = w.get<Context>().ctx->res.get<MeshManager>();
        w.module<MeshInstanceModule>();
        w.component<MeshInstance>();
        w.observer<const Transform, MeshInstance>()
            .event(flecs::OnSet)
            .event(flecs::OnAdd)
            .each([&](const Transform& tr, MeshInstance& mi) {
                if (mi.mesh == INVALID_ID) { return; }
                const AABB local = meshManager[mi.mesh].getAABB();
                mi.worldAABB = local.toGlobal(tr.global);
            });
    }

    RenderModule::RenderModule(const flecs::world& w) {
        auto& renderTargetManager = w.get<Context>().ctx->res.get<RenderTargetManager>();
        auto& sceneContextManager = w.get<Context>().ctx->res.get<SceneContextManager>();
        w.module<RenderModule>();
        w.component<Camera>();
        w.component<CameraRef>();
        w.component<Viewport>();
        w.component<RenderTarget>();
        w.component<Scene>();
        w.component<SceneRef>();
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
            .each([&](flecs::entity e, const RenderTarget& rt) {
                const auto& renderTarget = renderTargetManager[rt.renderTarget];
                if (renderTarget.isPaused()) { return; }
                auto views = std::list<RenderView>();
                e.children([&](const flecs::entity child) {
                    if (child.has<CameraRef>() && child.has<SceneRef>()) {
                        auto& cameraRef = child.get<CameraRef>();
                        auto& sceneRef = child.get<SceneRef>();
                        auto& scene = sceneContextManager[sceneRef.scene.get<Scene>().sceneContext];
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
