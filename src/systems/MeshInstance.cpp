/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.systems.mesh_instance;

namespace lysa {

       MeshInstanceModule::MeshInstanceModule(const flecs::world& w, MeshManager& meshManager) {
           w.module<MeshInstanceModule>();
           w.observer<MeshInstance, const Transform>()
               .event(flecs::OnAdd)
               .each([&](flecs::entity _, MeshInstance& mi, const Transform& tr) {
                    const AABB local = meshManager[mi.mesh].getAABB();
                    mi.worldAABB = local.toGlobal(tr.globalTransform);
               });
           w.observer<Transform, MeshInstance>()
               .event(flecs::OnSet)
               .each([&](flecs::entity e, Transform& tr, MeshInstance& mi) {
                    const AABB local = meshManager[mi.mesh].getAABB();
                    mi.worldAABB = local.toGlobal(tr.globalTransform);
               });
       }

}
