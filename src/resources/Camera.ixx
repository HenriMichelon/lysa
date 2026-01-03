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
        float4x4 transform{float4x4::identity()};
        /** View projection */
        float4x4 projection{float4x4::identity()};
    };

}

