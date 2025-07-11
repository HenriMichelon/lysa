/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#include "common.inc.slang"

struct DataFXAA {
    float spanMax;
    float reduceMul;
    float reduceMin;
};

[[vk::binding(1, 0)]] ConstantBuffer<DataFXAA> data : register(b1, space0);

float3 fxaa(Texture2D tex, SamplerState sampler, const float2 uv, const float2 texelSize) {
    static const float3 luma = float3(0.299, 0.587, 0.114);

    float3 rgbNW = tex.Sample(sampler, uv + float2(-1.0, -1.0) * texelSize).rgb;
    float3 rgbNE = tex.Sample(sampler, uv + float2(1.0, -1.0) * texelSize).rgb;
    float3 rgbSW = tex.Sample(sampler, uv + float2(-1.0, 1.0) * texelSize).rgb;
    float3 rgbSE = tex.Sample(sampler, uv + float2(1.0, 1.0) * texelSize).rgb;
    float3 rgbM  = tex.Sample(sampler, uv).rgb;

    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * data.reduceMul), data.reduceMin);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(float2(data.spanMax, data.spanMax), max(float2(-data.spanMax, -data.spanMax), dir * rcpDirMin)) * texelSize;

    float3 rgbA = 0.5 * (tex.Sample(sampler, uv + dir * (1.0 / 3.0 - 0.5)).rgb + tex.Sample(sampler, uv + dir * (2.0 / 3.0 - 0.5)).rgb);
    float3 rgbB = rgbA * 0.5 + 0.25 * (tex.Sample(sampler, uv + dir * -0.5).rgb + tex.Sample(sampler, uv + dir * 0.5).rgb);

    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        return rgbA;
    } else {
        return rgbB;
    }
}

float4 fragmentMain(VertexOutput input) : SV_TARGET {
    return float4(fxaa(
        textures[INPUT_BUFFER],
        samplers[SAMPLER_NEAREST_NEAREST_BORDER_LINEAR],
        input.uv,
        1.0 / params.imageSize),
        1.0);
}
