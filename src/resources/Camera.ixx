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
     * %A Camera
     */
    struct Camera : UnmanagedResource {
        /** World space transform */
        float4x4 transform;
        /** View projection */
        float4x4 projection;

        Camera(const float4x4& transform, const float4x4& projection) :
            transform(transform), projection(projection) {}
    };

}

