/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.ecs.systems;

import lysa.log;
import lysa.math;

namespace lysa::ecs {

     void TransformModule::updateGlobalTransform(const flecs::entity e, Transform& t) {
          const float4x4 parentMatrix = e.parent() && e.parent().has<Transform>()
               ? e.parent().get<Transform>().global
               : float4x4::identity();
          t.global = mul(t.local, parentMatrix);
          e.add<Updated>();
          e.children([&](const flecs::entity child) {
             if (child.has<Transform>()) {
                  updateGlobalTransform(child, child.get_mut<Transform>());
             }
          });
     }

     TransformModule::TransformModule(const flecs::world& w) {
          w.module<TransformModule>();
          w.component<Transform>();
          w.observer<Transform>()
            .event(flecs::OnSet)
            .event(flecs::OnAdd)
            .each([](const flecs::entity e,Transform& t) {
                updateGlobalTransform(e, t);
          });
     }

}
