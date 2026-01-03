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
     * Camera resource
     */
    struct Camera : UnmanagedResource {
        /** World space transform */
        float4x4 transform;
        /** View projection */
        float4x4 projection;
    };

}

