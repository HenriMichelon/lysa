/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.systems;

import lysa.aabb;
import lysa.components;
import lysa.math;
import lysa.types;
import lysa.resources.mesh;

namespace lysa {

     void TransformModule::updateGlobalTransform(
          const flecs::entity parent,
          const components::Transform& parentTransform) {
          parent.children([&](const flecs::entity child) {
             if (child.has<components::Transform>()) {
                  auto& t = child.get_mut<components::Transform>();
                  t.globalTransform = mul(t.localTransform, parentTransform.globalTransform);
                  child.add<tags::Updated>();
                  updateGlobalTransform(child, t);
             }
          });
     }

     TransformModule::TransformModule(const flecs::world& w) {
          w.module<TransformModule>();
          w.observer<const components::Position, components::Transform>()
          .event(flecs::OnSet)
          .event(flecs::OnAdd)
          .each([](const flecs::entity e, const components::Position &p, components::Transform& t) {
               t.localTransform[3] = float4{p.x, p.y, p.z, 1.0f};
               auto parentMatrix = float4x4::identity();
               if (e.parent() && e.has<components::Transform>()) {
                    parentMatrix = e.parent().get<components::Transform>().globalTransform;
               }
               t.globalTransform = mul(t.localTransform, parentMatrix);
               e.add<tags::Updated>();
               updateGlobalTransform(e, t);
          });
     }

     MeshInstanceModule::MeshInstanceModule(const flecs::world& w) {
          auto& meshManager = w.get<components::Context>().ctx->resources.get<MeshManager>();
          w.module<MeshInstanceModule>();
          w.observer<const components::Transform, components::MeshInstance>()
          .event(flecs::OnSet)
          .event(flecs::OnAdd)
          .each([&](flecs::entity _, const components::Transform& tr, components::MeshInstance& mi) {
               if (mi.mesh == INVALID_ID) { return; }
               const AABB local = meshManager[mi.mesh].getAABB();
               mi.worldAABB = local.toGlobal(tr.globalTransform);
          });
     }

}
