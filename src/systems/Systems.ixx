/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.systems;

import lysa.components;
import lysa.flecs;

export namespace lysa {

    class TransformModule {
    public:
        TransformModule(const flecs::world& w);
    private:
        static void updateGlobalTransform(
            flecs::entity e,
            components::Transform& t);
    };

    struct MeshInstanceModule {
        MeshInstanceModule(const flecs::world& w);
    };

}
