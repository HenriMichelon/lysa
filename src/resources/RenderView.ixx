/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.render_view;

import vireo;
import lysa.types;
import lysa.resources;
import lysa.resources.camera;

export namespace lysa {

    struct RenderView : UnmanagedResource {
        vireo::Viewport viewport{};
        vireo::Rect scissors{};
        Camera camera;
        unique_id scene{INVALID_ID};
    };

}

