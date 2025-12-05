/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.ecs.systems;

import lysa.aabb;
import lysa.resources.mesh;

namespace lysa::ecs {

     MeshInstanceModule::MeshInstanceModule(const flecs::world& w) {
          auto& meshManager = w.get<Context>().ctx->res.get<MeshManager>();
          w.module<MeshInstanceModule>();
          w.component<MeshInstance>();
          w.observer<const Transform, MeshInstance>()
          .event(flecs::OnSet)
          .event(flecs::OnAdd)
          .each([&](flecs::entity _, const Transform& tr, MeshInstance& mi) {
               if (mi.mesh == INVALID_ID) { return; }
               const AABB local = meshManager[mi.mesh].getAABB();
               mi.worldAABB = local.toGlobal(tr.global);
          });
     }

}
