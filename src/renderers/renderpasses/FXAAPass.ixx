/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.fxaa_pass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.renderpasses.post_processing;

export namespace lysa {

    /**
     * @brief Render pass for Fast Approximate Anti-Aliasing
     */
    class FXAAPass : public PostProcessing {
    public:
        /**
         * @brief Constructs a FXAAPass
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        FXAAPass(
            const Context& ctx,
            const RendererConfiguration& config) :
            PostProcessing(
                ctx,
                config,
                "fxaa",
                config.swapChainFormat,
                &fxaaData, sizeof(fxaaData),
            "FXAA"),
            fxaaData{ .spanMax = config.fxaaSpanMax, .reduceMul = config.fxaaReduceMul, .reduceMin = config.fxaaReduceMin} {
        }

    private:
        /** FXAA configuration. */
        struct {
            float spanMax{8.0f};
            float reduceMul{1.0f / 8.0f};
            float reduceMin{1.0f / 128.0f};
        } fxaaData;
    };
}