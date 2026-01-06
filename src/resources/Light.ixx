/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.light;

import lysa.math;
import lysa.resources;

export namespace lysa {

    /**
     * Light type
     */
    enum class LightType : int8 {
        LIGHT_UNKNOWN     = -1,
        LIGHT_DIRECTIONAL = 0,
        LIGHT_OMNI        = 1,
        LIGHT_SPOT        = 2
    };

    /**
      * Light data in GPU memory
      */
    struct LightData {
        // light params
        int32 type{-1}; // Light::LightType
        float range{0.0f};
        float cutOff{0.0f};
        float outerCutOff{0.0f};
        float4 position{0.0f};
        float4 direction{0.0f};
        float4 color{1.0f, 1.0f, 1.0f, 1.0f}; // RGB + Intensity;
        // shadow map params
        int32 mapIndex{-1};
        uint32 cascadesCount{0};
        float4 cascadeSplitDepth{0.0f};
        float4x4 lightSpace[6];
    };

    /**
    * %A Light
    */
    struct Light : UnmanagedResource {
        LightType type;
        float3 color;
        float intensity;
        /** World space transform */
        float4x4 transform;
        float range;
        float cutOff;  // radians
        float outerCutOff;  // radians
        bool castShadows;
        uint32 shadowMapSize;
        bool visible{true};
        float nearShadowClipDistance{0.01f};
        uint32 shadowMapCascadesCount{3};
        float cascadeSplitLambda{.98f};
        float shadowTransparencyScissors{0.25f};
        float shadowTransparencyColorScissors{0.50f};

        float3 getPosition() const { return transform[3].xyz; }

        float3 getFrontVector() const {  return -normalize(transform[2].xyz); }

        Light(
            const LightType type,
            const float3& color = {1.0f},
            const float intensity = 1.0f,
            const float4x4& transform = float4x4::identity(),
            const float range = 10.0f,
            const float cutOff = 1.3, // 75
            const float outerCutOff= 1.4, // 80
            const bool castShadows = false,
            const uint32 shadowMapSize = 512) :
            type(type),
            color(color),
            intensity(intensity),
            transform(transform),
            range(range),
            cutOff(cutOff),
            outerCutOff(outerCutOff),
            castShadows(castShadows),
            shadowMapSize(shadowMapSize) {}

        LightData getData() const {
            return {
                .type = static_cast<int32>(type),
                .range = range,
                .cutOff = std::cos(cutOff),
                .outerCutOff = std::cos(outerCutOff),
                .position = float4{getPosition(), 0.0f},
                .direction = float4(getFrontVector(), 0.0f),
                .color = float4(color, intensity)
            };
        }
    };

}
