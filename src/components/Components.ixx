/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.components;

import std;

import lysa.aabb;
import lysa.context;
import lysa.math;
import lysa.types;

export namespace lysa::tags {
    struct Updated { };
}

export namespace lysa::components {

    struct Context {
        lysa::Context* ctx;
    };

    struct Position {
        float x;
        float y;
        float z;
    };

    struct Transform {
        float4x4 localTransform{};
        float4x4 globalTransform{};
    };

    struct MeshInstance {
        unique_id mesh{INVALID_ID};
        bool castShadows{false};
        AABB worldAABB;
    };



}
