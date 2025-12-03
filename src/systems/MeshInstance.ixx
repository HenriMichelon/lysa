/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.systems.mesh_instance;

import lysa.aabb;
import lysa.components;
import lysa.flecs;
import lysa.types;
import lysa.resources.mesh;

export namespace lysa {

   class MeshInstanceModule {
       MeshInstanceModule(const flecs::world& w, MeshManager& meshManager);
   };

}
