/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.math;


/**
 * @file
 * @brief Math helpers built on top of hlsl++ types.
 *
 * This module provides common math utilities and matrix/quaternion helpers
 * used across the engine, including view/projection matrix builders, a
 * quaternion-to-Euler conversion, tolerant comparisons, and simple random
 * number generation utilities.
 */

#include <cstdint>
using int32 = int32_t;
using uint32 = uint32_t;

#define HLSLPP_FEATURE_TRANSFORM
#define HLSLPP_MODULE_DECLARATION

#include "hlsl++/vector_float.h"
#include "hlsl++/vector_float8.h"

#include "hlsl++/vector_int.h"
#include "hlsl++/vector_uint.h"
#include "hlsl++/vector_double.h"

#include "hlsl++/matrix_float.h"

#include "hlsl++/quaternion.h"
#include "hlsl++/dependent.h"

#include "hlsl++/data_packing.h"

export import std;
export import lysa.types;

export namespace lysa {

    /**
    * X Axis
    */
    const float3 AXIS_X{1.0f, 0.0f, 0.0f};

    /**
    * Y Axis
    */
    const float3 AXIS_Y{0.0f, 1.0f, 0.0f};

    /**
    * Z Axis
    */
    const float3 AXIS_Z{0.0f, 0.0f, 1.0f};

    /**
    * UP Axis
    */
    const float3 AXIS_UP = {0.0f, 1.0f, 0.0f};

    /**
    * DOWN Axis
    */
    const float3 AXIS_DOWN = {0.0f, -1.0f, 0.0f};

    /**
    * FRONT Axis
    */
    const float3 AXIS_FRONT = {0.0f, 0.0f, -1.0f};

    /**
    * BACK Axis
    */
    const float3 AXIS_BACK = {0.0f, 0.0f, 1.0f};

    /**
    * RIGHT Axis
    */
    const float3 AXIS_RIGHT = {1.0f, 0.0f, 0.0f};

    /**
    * LEFT Axis
    */
    const float3 AXIS_LEFT = {-1.0f, 0.0f, 0.0f};

    /**
    * 2D zero initialized vector
    */
    const float2 FLOAT2ZERO{0.0f};

    /**
    * 3D zero initialized vector
    */
    const float3 FLOAT3ZERO{0.0f};

    /**
    * 4D zero initialized vector
    */
    const float4 FLOAT4ZERO{0.0f};

    /**
    * Unit quaternion with no rotation
    */
    const quaternion QUATERNION_IDENTITY{0.0f, 0.0f, 0.0f, 1.0f};

    /**
    * The Basis of 3D transform.
    * It is composed of 3 axes (Basis.x, Basis.y, and Basis.z).
    * Together, these represent the transform's rotation, scale, and shear.
    */
    const float3x3 TRANSFORM_BASIS{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    constexpr float HALF_PI = std::numbers::pi_v<float> / 2.0f;

    /**
     * @brief Convert a unit quaternion to Euler angles (XYZ order).
     *
     * The returned angles are in radians and follow the conventional
     * pitch (X), yaw (Y), roll (Z) ordering.
     *
     * @param q Input quaternion. It does not need to be normalized; the
     *          function normalizes it internally.
     * @return float3 containing {pitch, yaw, roll} in radians.
     * @note Angles are wrapped to the principal values typically produced by
     *       atan2/asin and may exhibit gimbal lock at +/- 90 degrees pitch.
     */
    float3 euler_angles(quaternion q);

    /**
     * @brief Convert a scalar angle in degrees to radians.
     *
     * Convenience overload that forwards to the vectorized version.
     *
     * @param angle Angle in degrees.
     * @return Angle in radians.
     */
    float radians(const float angle) { return radians(float1{angle}); }

    /**
     * @brief Compare two floating-point values with a relative tolerance.
     *
     * Uses max(abs(a), abs(b)) scaled by 1e-4 as tolerance.
     *
     * @param f1 First value.
     * @param f2 Second value.
     * @return true if the values are approximately equal; false otherwise.
     */
    inline bool almost_equals(const float f1, const float f2) {
        return (std::fabs(f1 - f2) <=  0.0001 * std::fmax(std::fabs(f1), std::fabs(f2)));
    }

    /**
     * @brief Approximate equality for quaternions.
     *
     * Each component is compared using almostEquals(float, float).
     *
     * @param f1 First quaternion.
     * @param f2 Second quaternion.
     * @return true if all components are approximately equal; false otherwise.
     */
    inline bool almost_equals(const quaternion& f1, const quaternion& f2) {
        return almost_equals(f1.x, f2.x) &&
            almost_equals(f1.y, f2.y) &&
            almost_equals(f1.z, f2.z) &&
            almost_equals(f1.w, f2.w);
    }

    /**
     * @brief Build a right-handed look-at view matrix.
     *
     * @param eye Camera position in world space.
     * @param center Target point the camera looks at.
     * @param up World up direction.
     * @return 4x4 view matrix.
     */
    float4x4 look_at(const float3& eye, const float3& center, const float3& up);

    /**
     * @brief Build a right-handed perspective projection matrix.
     *
     * @param fov Vertical field of view in radians.
     * @param aspect Aspect ratio (width/height).
     * @param near Near clipping plane distance (> 0).
     * @param far Far clipping plane distance (> near).
     * @return 4x4 projection matrix.
     */
    float4x4 perspective(float fov, float aspect, float near, float far);

    /**
     * @brief Build a right-handed orthographic projection matrix.
     *
     * @param left Left plane.
     * @param right Right plane.
     * @param top Top plane.
     * @param bottom Bottom plane.
     * @param znear Near clipping plane.
     * @param zfar Far clipping plane.
     * @return 4x4 orthographic projection matrix.
     */
    float4x4 orthographic(float left, float right,
                          float top, float  bottom,
                          float znear, float zfar);

    /**
     * @brief Extract a quaternion from a 4x4 rotation matrix.
     *
     * Converts the upper-left 3x3 rotation part of the 4x4 matrix into
     * a unit quaternion.
     *
     * @param m Input 4x4 matrix.
     * @return Unit quaternion representing the rotation.
     */
    quaternion to_quaternion(const float4x4& m);

    /**
     * @brief Generate a random unsigned integer in [0, max].
     *
     * @param max Inclusive upper bound.
     * @return Pseudo-random value uniformly distributed in [0, max].
     */
    uint32 randomi(uint32 max);

    /**
     * @brief Generate a random float in [0.0f, max].
     *
     * @param max Inclusive upper bound.
     * @return Pseudo-random value uniformly distributed in [0.0f, max].
     */
    float randomf(float max);

    float2 mul(const float2&a, const float b) { return a * b; }
    float3 mul(const float3&a, const float b) { return a * b; }
    float4 mul(const float4&a, const float b) { return a * b; }
    float2 mul(const float a, const float2&b) { return a * b; }
    float3 mul(const float a, const float3&b) { return a * b; }
    float4 mul(const float a, const float4&b) { return a * b; }
    float2 add(const float2& a, const float2&b) { return a + b; }
    float3 add(const float3& a, const float3&b) { return a + b; }
    float4 add(const float4& a, const float4&b) { return a + b; }

}

