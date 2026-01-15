/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.texture;

import vireo;

import lysa.context;
import lysa.math;
import lysa.resources;
import lysa.resources.image;
import lysa.resources.manager;

export namespace lysa {

    /**
     * Represents an image-based texture resource.
     *
     * Combines an image with a sampler and an optional transformation matrix.
     */
    struct ImageTexture : UnmanagedResource {
        /** Unique identifier of the associated image resource. */
        unique_id image{INVALID_ID};

        /** Index of the sampler to be used for this texture. */
        uint32 samplerIndex{0};

        /** 3x3 transformation matrix for texture coordinates. */
        float3x3 transform{float3x3::identity()};

        /** Default constructor for ImageTexture. */
        ImageTexture() = default;

        /**
         * Constructs an ImageTexture with a specific image and sampler.
         * @param image Unique identifier of the image resource.
         * @param samplerIndex Index of the sampler to use.
         */
        ImageTexture(const unique_id image, const uint32 samplerIndex): image(image), samplerIndex(samplerIndex) {}
    };

}

