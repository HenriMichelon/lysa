/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#include "common.inc.slang"

struct Data {
    float near;
    float far;
};

[[vk::binding(1, 0)]] ConstantBuffer<Data> data : register(b1, space0);

float linearizeDepth(float z01, float n, float f) {
    return (n * f) / (f - z01 * (f - n));
}

float4 fragmentMain(VertexOutput input) : SV_TARGET {
    float z = textures[INPUT_BUFFER].Sample(samplers[SAMPLER_NEAREST_NEAREST_BORDER_LINEAR], input.uv).r;
    float lin = linearizeDepth(z, data.near, data.far);
    float v = sqrt(saturate((lin - data.near) / (data.far - data.near)));
    return float4(v, v, v, 1.0);
}
