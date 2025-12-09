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
        w.component<CameraProjection>();
        w.component<Camera>();
        w.component<Viewport>();
        w.component<RenderTarget>();
        w.component<Scene>();
        w.observer<CameraProjection>()
            .event(flecs::OnSet)
            .event(flecs::OnAdd)
            .each([&](CameraProjection& cp) {
                if (cp.isPerspective) {
                    cp.projection = perspective(radians(cp.fov), cp.aspectRatio, cp.near, cp.far);
                } else {
                    cp.projection = orthographic(cp.left, cp.right, cp.top, cp.bottom, cp.near, cp.far);
                }
            });
        w.system<const RenderTarget, const Viewport, const Camera>()
            .with<Scene>().parent()
            .kind(flecs::OnUpdate)
            .each([&](
                flecs::entity e,
                const RenderTarget& rt,
                const Viewport& vp,
                const Camera& camref) {
                    const auto& renderTarget = renderTargetManager[rt.renderTarget];
                    if (renderTarget.isPaused()) { return; }
                    auto& scene = sceneContextManager[e.parent().get<Scene>().sceneContext];
                    const auto cameraProjection = camref.camera.get<CameraProjection>();
                    const auto cameraTransform = camref.camera.get<Transform>().global;
                    auto cameraDesc = CameraDesc{
                        .position = cameraTransform[3].xyz,
                        .transform = cameraTransform,
                        .projection = cameraProjection.projection
                    };
                    renderTarget.render(vp.viewport, vp.scissors, cameraDesc, scene);
                });
            }


}
