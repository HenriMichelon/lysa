/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.blur_data;

import std;
import vireo;
import lysa.math;

export namespace lysa {

    /** Constant buffer data used by Gaussian blur post-process. */
    struct BlurData {
        uint32 kernelSize;
        float4 weights[9*9]; // float4 for correct alignment
        float2 texelSize;

        void update(const vireo::Extent& extent, const float strength) {
            // Pre-compute Gaussian weights
            if (kernelSize > 9) { kernelSize = 9; }
            texelSize = (1.0 / float2(extent.width, extent.height)) * strength;
            const int halfKernel = kernelSize / 2;
            float sum = 0.0;
            for (int i = 0; i < kernelSize; i++) {
                for (int j = 0; j < kernelSize; j++) {
                    const int index = i * kernelSize + j;
                    const float x = static_cast<float>(i - halfKernel) * texelSize.x;
                    const float y = static_cast<float>(j - halfKernel) * texelSize.y;
                    weights[index].x = std::exp(-(x * x + y * y) / 2.0);
                    sum += weights[index].x;
                }
            }
            // Normalize weights
            for (int i = 0; i < kernelSize * kernelSize; i++) {
                weights[i].x /= sum;
            }
        }
    };

}