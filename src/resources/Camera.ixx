/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.camera;

import lysa.math;
import lysa.resources;

export namespace lysa {

    /**
     * @brief Represents a camera in the 3D scene.
     */
    struct Camera : UnmanagedResource {
        /** @brief World-space transformation matrix (world-from-local). */
        float4x4 transform;
        /** @brief Projection matrix (clip-from-view). */
        float4x4 projection;
        /** @brief Near clipping plane distance. */
        float near;
        /** @brief Far clipping plane distance. */
        float far;

        /**
         * @brief Constructs a new Camera object.
         * @param transform World-space transformation matrix.
         * @param projection Projection matrix.
         * @param near Near clipping plane distance.
         * @param far Far clipping plane distance.
         */
        Camera(const float4x4& transform, const float4x4& projection, const float near, const float far) :
            transform(transform), projection(projection), near(near), far(far) {}
    };

}

