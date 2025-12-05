/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module lysa.ecs.components.transform;

import lysa.math;

namespace lysa::ecs {

    #define CHECK_TRANSFORM(e) assert([&]{ return e.has<Transform>();}, "No Transform component in entity");

    float3 getPositionGlobal(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return e.get_ref<Transform>()->global[3].xyz;
    }

    float3 getPosition(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return e.get_ref<Transform>()->local[3].xyz;
    }

    void setPosition(const flecs::entity e, const float3& position) {
        if (any(position != getPosition(e))) {
            auto t = e.get<Transform>();
            t.local[3] = float4{position, 1.0f};
            e.set(t);
        }
    }

    void setPositionGlobal(const flecs::entity e, const float3& position) {
        if (any(position != getPositionGlobal(e))) {
            if (!e.parent()) {
                setPosition(e, position);
                return;
            }
            CHECK_TRANSFORM(e.parent());
            auto t = e.parent().get<Transform>();
            t.local[3] = mul(float4{position, 1.0}, inverse(t.global));
            e.set(t);
        }
    }

    void translate(const flecs::entity e, const float3& localOffset) {
        CHECK_TRANSFORM(e);
        auto t = e.get<Transform>();
        t.local = mul(t.local, float4x4::translation(localOffset));
        e.set(t);
    }

    void scale(const flecs::entity e, const float& scale) {
        CHECK_TRANSFORM(e);
        auto t = e.get<Transform>();
        t.local = mul(float4x4::scale(scale), t.local);
        e.set(t);
    }

    float3 getScale(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        auto t = e.get_ref<Transform>();
        return {
            length(t->local[0].xyz),
            length(t->local[1].xyz),
            length(t->local[2].xyz),
        };
    }

    float3 getScaleGlobal(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        auto t = e.get_ref<Transform>();
        return {
            length(t->global[0].xyz),
            length(t->global[1].xyz),
            length(t->global[2].xyz),
        };
    }

    quaternion getRotation(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return quaternion{float3x3{e.get_ref<Transform>()->local}};
    }

    quaternion getRotationGlobal(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return quaternion{float3x3{e.get_ref<Transform>()->global}};
    }

    void setRotation(const flecs::entity e, const quaternion& quat) {
        if (any(quat != getRotation(e))) {
            const auto tm = float4x4::translation(getPosition(e));
            const auto rm = float4x4{quat};
            const auto sm = float4x4::scale(getScale(e));
            auto t = e.get<Transform>();
            t.local = mul(mul(rm, sm), tm);
            e.set(t);
        }
    }

    void rotateX(const flecs::entity e, const float angle) {
        CHECK_TRANSFORM(e);
        auto t = e.get<Transform>();
        t.local = mul(float4x4::rotation_x(angle), t.local);
        e.set(t);
    }

    void rotateY(const flecs::entity e, const float angle) {
        CHECK_TRANSFORM(e);
        auto t = e.get<Transform>();
        t.local = mul(float4x4::rotation_y(angle), t.local);
        e.set(t);
    }

    void rotateZ(const flecs::entity e, const float angle) {
        CHECK_TRANSFORM(e);
        auto t = e.get<Transform>();
        t.local = mul(float4x4::rotation_z(angle), t.local);
        e.set(t);
    }

    float3 toGlobal(const flecs::entity e, const float3& local) {
        CHECK_TRANSFORM(e);
        auto t = e.get_ref<Transform>();
        return mul(float4(local, 1.0f), t->global).xyz;
    }

    float3 toLocal(const flecs::entity e, const float3& global) {
        CHECK_TRANSFORM(e);
        auto t = e.get_ref<Transform>();
        return mul(float4(global, 1.0f), mul(t->local, inverse(t->global))).xyz;
    }

    float3 getRightVector(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return normalize(e.get_ref<Transform>()->global[0].xyz);
    }

    float3 getLeftVector(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return -normalize(e.get_ref<Transform>()->global[0].xyz);
    }

    float3 getFrontVector(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return -normalize(e.get_ref<Transform>()->global[2].xyz);
    }

    float3 getBackVector(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return normalize(e.get_ref<Transform>()->global[2].xyz);
    }

    float3 getUpVector(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return normalize(e.get_ref<Transform>()->global[1].xyz);
    }

    float3 getDownVector(const flecs::entity e) {
        CHECK_TRANSFORM(e);
        return -normalize(e.get_ref<Transform>()->global[1].xyz);
    }

    void lookAt(const flecs::entity e, const float3& target) {
        const auto newGlobalTransform = inverse(
            look_at(getPositionGlobal(e), target, getUpVector(e)));
        auto t = e.get<Transform>();
        if (e.parent()) {
            CHECK_TRANSFORM(e.parent());
            t.local = mul(newGlobalTransform, inverse(e.parent().get_ref<Transform>()->global));
        } else {
            t.local = newGlobalTransform;
        }
        e.set(t);
    }

}
