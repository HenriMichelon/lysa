/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.ecs.components.transform;

import lysa.exception;
import lysa.math;
import lysa.ecs.flecs;

export namespace lysa::ecs {

    /**
     * Object local & world transforms.
     *
     * World (global) transform is computed each time the Transform is set or added to an entity
     */
    struct Transform {
        /** Local space transform matrix */
        float4x4 local{float4x4::identity()};
        /** World space transform matrix */
        float4x4 global{};
    };

    /**
    * Returns the world space position
    */
    float3 getPositionGlobal(flecs::entity e);

    /**
    * Returns the local space position
    */
    float3 getPosition(flecs::entity e);

    /**
    * Sets the world space position
    */
    void setPositionGlobal(flecs::entity e, const float3& position);

    /**
    * Sets the world space position
    */
    void setPositionGlobal(const flecs::entity e, const float x, const float y, const float z) {
        setPositionGlobal(e, float3{x, y, z});
    }

    /**
    * Sets the local space position
    */
    void setPosition(flecs::entity e, const float3& position);

    /**
    * Sets the local space position
    */
    void setPosition(const flecs::entity e, const float x, const float y, const float z) {
        setPosition(e, float3{x, y, z});
    }

    /**
    * Changes the entity's position by the given offset vector in local space.
    */
    void translate(flecs::entity e, const float3& localOffset);

    /**
    * Changes the entity's position by the given offset vector in local space.
    */
    void translate(const flecs::entity e, const float localOffsetX, const float localOffsetY, const float localOffsetZ) {
        translate(e, float3{localOffsetX, localOffsetY, localOffsetZ});
    }

    /**
    * Scale the local transformation
    */
    void scale(flecs::entity e, const float& scale);

    /**
    * Returns the scale part of the local transformation.
    */
    float3 getScale(flecs::entity e);

    /**
    * Returns the scale part of the world transformation.
    */
    float3 getScaleGlobal(flecs::entity e);

    /**
    * Returns the rotation of the local transformation
    */
    quaternion getRotation(flecs::entity e);

    /**
    * Returns the rotation of the world transformation
    */
    quaternion getRotationGlobal(flecs::entity e);

    /**
    * Returns the rotation of the local transformation, in euler angles in radians
    */
    float3 getRotationRadians(const flecs::entity e) {
        return euler_angles(getRotation(e));
    }

    /**
     * Returns the rotation of the world transformation, in euler angles in radians
     */
    float3 getRotationRadiansGlobal(const flecs::entity e) {
        return euler_angles(getRotationGlobal(e));
    }

    /**
    * Sets the rotation of local transformation
    */
    void setRotation(flecs::entity e, const quaternion& quat);

    /**
    * Sets the rotation world local transformation
    */
    void setRotationGlobal(flecs::entity e, const quaternion& quat);

    /**
    * Rotates the local transformation around the X axis by angle in radians.
    */
    void rotateX(flecs::entity e, float angle);

    /**
     * Rotates the local transformation around the Y axis by angle in radians.
     */
    void rotateY(flecs::entity e, float angle);

    /**
     * Rotates the local transformation around the Z axis by angle in radians.
     */
    void rotateZ(flecs::entity e, float angle);

    /**
     * Returns the X axis rotation of the local transformation in radians
     */
    float getRotationX(const flecs::entity e) {
        return euler_angles(getRotation(e)).x;
    }

    /**
     * Returns the Y axis rotation of the local transformation in radians
     */
    float getRotationY(const flecs::entity e) {
        return euler_angles(getRotation(e)).y;
    }

    /**
     * Returns the Z axis rotation of the local transformation in radians
     */
    float getRotationZ(const flecs::entity e) {
        return euler_angles(getRotation(e)).z;
    }

    /**
    * Sets the X axis rotation of the local transformation by angle in radians.
    */
    void setRotationX(const flecs::entity e, const float angle)  {
        rotateX(e, angle - getRotationX(e));
    }

    /**
     * Sets the Y axis rotation of the local transformation by angle in radians.
     */
    void setRotationY(const flecs::entity e, const float angle) {
        rotateY(e, angle - getRotationY(e));
    }

    /**
     * Sets the Z axis rotation of the local transformation by angle in radians.
     */
    void setRotationZ(const flecs::entity e, const float angle) {
        rotateZ(e, angle - getRotationZ(e));
    }

    /**
     * Transforms a local vector from this node's local space to world space.
     */
    float3 toGlobal(flecs::entity e, const float3& local);

    /**
     * Transforms a world space vector to this node's local space.
     */
    float3 toLocal(flecs::entity e, const float3& global);

    /**
     * Returns the normalized right vector
     */
    float3 getRightVector(flecs::entity e);

    /**
     * Returns the normalized left vector
     */
    float3 getLeftVector(flecs::entity e);

    /**
     * Returns the normalized front vector
     */
    float3 getFrontVector(flecs::entity e);

    /**
     * Returns the normalized back vector
     */
    float3 getBackVector(flecs::entity e);

    /**
     * Returns the normalized up vector
     */
    float3 getUpVector(flecs::entity e);

    /**
     * Returns the normalized down vector
     */
    float3 getDownVector(flecs::entity e);

    /**
     * Rotates the entity toward the target
     */
    void lookAt(flecs::entity e, const float3& target);


}
