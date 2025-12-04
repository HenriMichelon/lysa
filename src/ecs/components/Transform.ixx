/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.ecs.components.transform;

import lysa.math;
import lysa.ecs.flecs;

export namespace lysa::ecs {

    struct Transform {
        float4x4 local{float4x4::identity()};
        float4x4 global{};
    };

    struct Translate {
        float3 offset;
    };

    float3 getPositionGlobal(flecs::entity e);

    float3 getPositionLocal(flecs::entity e);

    void setPositionGlobal(flecs::entity e, const float3& position);

    void setPositionLocal(flecs::entity e, const float3& position);

}
