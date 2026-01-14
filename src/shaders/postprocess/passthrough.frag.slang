/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#include "common.inc.slang"

float4 fragmentMain(VertexOutput input) : SV_TARGET {
    return textures[INPUT_BUFFER].Sample(samplers[SAMPLER_NEAREST_NEAREST_BORDER_LINEAR], input.uv);
}
