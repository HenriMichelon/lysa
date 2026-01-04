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
     * Image-based texture
     */
    struct ImageTexture : UnmanagedResource {
        unique_id image{INVALID_ID};
        uint32 samplerIndex{0};
        float3x3 transform{float3x3::identity()};

        ImageTexture() = default;
        ImageTexture(const unique_id image, const uint32 samplerIndex): image(image), samplerIndex(samplerIndex) {}
    };

}

