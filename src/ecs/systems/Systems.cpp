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
                if (mi.meshInstance) {
                    e.add<Updated>();
                } else {
                    auto& scene = sceneManager[sceneRef.scene];
                    auto aabb = meshManager[mi.mesh].getAABB().toGlobal(tr.global);
                    mi.meshInstance = std::make_shared<MeshInstanceDesc>(
                        meshManager[mi.mesh],
                        e.has<Visible>(),
                        e.has<CastShadows>(),
                        aabb,
                        tr.global);
                    scene.addInstance(mi.meshInstance, false);
                }
            });
        w.observer<const Scene, const MeshInstance, const Visible>()
            .term_at(0).parent()
            .event(flecs::OnAdd)
            .event(flecs::OnRemove)
            .each([&](const flecs::entity e, const Scene&, const MeshInstance& mi, const Visible&) {
               if (mi.mesh != INVALID_ID && mi.meshInstance) {
                   e.add<Updated>();
               }
           });
        w.observer<const Scene, MeshInstance, const MaterialOverride>()
            .term_at(0).parent()
            .event(flecs::OnSet)
            .each([&](const Scene&sc, MeshInstance& mi, const MaterialOverride&mo) {
               if (mi.mesh != INVALID_ID && mi.meshInstance) {
                    auto& scene = sceneManager[sc.scene];
                    scene.removeInstance(mi.meshInstance, false);
                    mi.meshInstance = std::make_shared<MeshInstanceDesc>(*mi.meshInstance);
                    mi.meshInstance->materialsOverride[mo.surfaceIndex] = mo.material;
                    scene.addInstance(mi.meshInstance, false);
               }
           });
        w.observer<const Scene, MeshInstance, const MaterialOverride>()
            .term_at(0).parent()
            .event(flecs::OnRemove)
            .each([&](const Scene&sc, MeshInstance& mi, const MaterialOverride&mo) {
               if (mi.mesh != INVALID_ID && mi.meshInstance) {
                    auto& scene = sceneManager[sc.scene];
                    scene.removeInstance(mi.meshInstance, false);
                    mi.meshInstance = std::make_shared<MeshInstanceDesc>(*mi.meshInstance);
                    mi.meshInstance->materialsOverride.erase(mo.surfaceIndex);
                    scene.addInstance(mi.meshInstance, false);
               }
           });
        w.system<const Scene, const MeshInstance, const Transform, const Updated>()
            .term_at(0).parent()
            .kind(flecs::OnUpdate)
            .each([&](const flecs::entity e, const Scene& sc, const MeshInstance& mi, const Transform& tr, const Updated&) {
                if (mi.mesh != INVALID_ID && mi.meshInstance) {
                    e.remove<Updated>();
                    mi.meshInstance->visible = e.has<Visible>();
                    mi.meshInstance->worldAABB = meshManager[mi.mesh].getAABB().toGlobal(tr.global);
                    mi.meshInstance->worldTransform = tr.global;
                    auto& scene = sceneManager[sc.scene];
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
        w.observer<Scene, const AmbientLight>()
            .event(flecs::OnSet)
            .event(flecs::OnAdd)
            .each([&](const Scene& sc, const AmbientLight& al) {
                auto& scene = sceneManager[sc.scene];
                scene.setAmbientLight(float4(al.color, al.intensity));
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
