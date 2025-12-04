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
          const flecs::entity e,
          components::Transform& t) {
          auto parentMatrix = float4x4::identity();
          auto parent = e.parent();
          if (parent && parent.has<components::Transform>()) {
               parentMatrix = parent.get<components::Transform>().globalTransform;
          }
          t.globalTransform = mul(t.localTransform, parentMatrix);
          e.add<tags::Updated>();
          e.children([&](const flecs::entity child) {
             if (child.has<components::Transform>()) {
                  updateGlobalTransform(child, child.get_mut<components::Transform>());
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
