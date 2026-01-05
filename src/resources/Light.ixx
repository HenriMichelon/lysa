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
    * %A Ligth
    */
    struct Light : UnmanagedResource {
        LightType type;
        float3 color;
        float intensity;
        float3 position;
        float3 direction;
        float range;
        float cutOff{0.0f};
        float outerCutOff{0.0f};
        bool visible{true};

        Light(const Light& other):
            type(other.type),
            color(other.color),
            intensity(other.intensity),
            position(other.position),
            direction(other.direction),
            visible(other.visible) {
        }

        Light(
            const LightType type,
            const float3& color = {1.0f},
            const float intensity = 1.0f,
            const float3& position = {},
            const float3& direction = {},
            const float range = 10.0f,
            float cutOff = 0.1,
            float outerCutOff = 0.2) :
            type(type),
            color(color),
            intensity(intensity),
            position(position),
            direction(direction),
            range(range),
            cutOff(cutOff),
            outerCutOff(outerCutOff) {}

        LightData getData() const {
            return {
                .type = static_cast<int32>(type),
                .range = range,
                .cutOff = cutOff,
                .outerCutOff = outerCutOff,
                .position = float4{position, 0.0f},
                .direction = float4(direction, 0.0f),
                .color = float4(color, intensity)
            };
        }
    };

}
