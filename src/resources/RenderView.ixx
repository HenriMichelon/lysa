/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.render_view;

import vireo;
import lysa.resources;
import lysa.resources.camera;
import lysa.resources.scene;

export namespace lysa {

    struct RenderView : UnmanagedResource {
        vireo::Viewport viewport{};
        vireo::Rect scissors{};
        const Camera& camera;
        Scene& scene;

        RenderView(
            const Camera& camera,
            Scene& scene,
            const vireo::Viewport& viewport = {},
            const vireo::Rect& scissors = {}) :
            viewport(viewport),
            scissors(scissors),
            camera(camera),
            scene(scene) {}
    };

}

