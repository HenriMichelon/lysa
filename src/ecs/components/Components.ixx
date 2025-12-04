/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.ecs.components;

import lysa.aabb;
import lysa.context;
import lysa.math;

export import lysa.ecs.components.transform;
export import lysa.ecs.flecs;

export namespace lysa::ecs {

    struct Updated { };

    struct Context {
        lysa::Context* ctx;
    };

    struct MeshInstance {
        unique_id mesh{INVALID_ID};
        bool castShadows{false};
        AABB worldAABB;
    };

}
