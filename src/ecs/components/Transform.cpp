/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.ecs.components.transform;

namespace lysa::ecs {

    #define CHECK_TRANSFORM(e) assert([&]{ return e.has<Transform>();}, "No Transform component in entity");


    float3 getPositionGlobal(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return e.get<Transform>().global[3].xyz;
    }

    float3 getPositionLocal(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return e.get<Transform>().local[3].xyz;
    }

    void setPositionLocal(const flecs::entity e, const float3& position) {
        CHECK_TRANSFORM(e);
        if (any(position != getPositionLocal(e))) {
            auto t = e.get<Transform>();
            t.local[3] = float4{position, 1.0f};
            e.set(t);
        }
    }

    void setPositionGlobal(const flecs::entity e, const float3& position) {
        if (any(position != getPositionGlobal(e))) {
            if (!e.parent()) {
                setPositionLocal(e, position);
                return;
            }
            CHECK_TRANSFORM(e.parent());
            auto t = e.parent().get<Transform>();
            t.local[3] = mul(float4{position, 1.0}, inverse(t.global));
            e.set(t);
        }
    }

}
