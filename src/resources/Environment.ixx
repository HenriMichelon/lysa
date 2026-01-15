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

    /**
     * Represents the environmental settings of the scene.
     */
    struct Environment : UnmanagedResource {
        /** The color of the ambient light. */
        float3 color{1.0f, 1.0f, 1.0f};
        /** The intensity of the ambient light. */
        float intensity{0.0f};

        /** Default constructor for Environment. */
        Environment() = default;

        /**
         * Constructs an Environment with specific color and intensity.
         * @param color The color of the ambient light.
         * @param intensity The intensity of the ambient light.
         */
        Environment(const float3& color, const float intensity) :
            color(color),
            intensity(intensity) {}
    };

}

