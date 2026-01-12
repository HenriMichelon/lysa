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

    /**
     * @brief Represents a specific view into a scene for rendering.
     *
     * A RenderView combines a camera, a scene, and viewport/scissor dimensions
     * to define how a scene should be rendered to a target.
     */
    struct RenderView : UnmanagedResource {
        /** @brief The viewport dimensions for rendering. */
        vireo::Viewport viewport{};
        /** @brief The scissor rectangle for rendering. */
        vireo::Rect     scissors{};
        /** @brief Reference to the camera used for this view. */
        const Camera&   camera;
        /** @brief Reference to the scene to be rendered. */
        Scene&          scene;

        /**
         * @brief Constructs a new RenderView.
         * @param camera The camera defining the view transformation and projection.
         * @param scene The scene to be rendered from this view.
         * @param viewport The viewport dimensions (optional).
         * @param scissors The scissor rectangle (optional).
         */
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

