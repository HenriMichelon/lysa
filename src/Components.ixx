/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.components;

import std;

import lysa.aabb;
import lysa.math;
import lysa.types;

export namespace lysa {

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
