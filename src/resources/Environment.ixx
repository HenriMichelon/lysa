/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.environment;

import lysa.math;
import lysa.resources;

export namespace lysa {

    struct Environment : UnmanagedResource {
        float3 color{1.0f, 1.0f, 1.0f};
        float intensity{1.0f};
    };

}

