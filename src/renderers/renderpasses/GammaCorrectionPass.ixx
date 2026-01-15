/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.gamma_correction_pass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.renderpasses.post_processing;

export namespace lysa {

    /**
     * Render pass for gamma correction
     */
    class GammaCorrectionPass : public PostProcessing {
    public:
        /**
         * Constructs a GammaCorrectionPass
         * @param ctx The engine context
         * @param config The renderer configuration
         * @param toneMappingType
         */
        GammaCorrectionPass(
            const Context& ctx,
            const RendererConfiguration& config,
            const ToneMappingType toneMappingType) :
            PostProcessing(
                ctx,
                config,
                toneMappingType == ToneMappingType::REINHARD ? "reinhard" :
                toneMappingType == ToneMappingType::ACES ? "aces" :
                "gamma_correction",
                &gammaCorrectionData, sizeof(gammaCorrectionData),
                config.swapChainFormat,
                "Gamma correction"),
            gammaCorrectionData{ .gamma = config.gamma, .exposure = config.exposure } {
        }

    private:
        /** Gamma/exposure parameters. */
        struct {
            float gamma;
            float exposure;
        } gammaCorrectionData;
    };
}