/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.configuration;

import vireo;

import lysa.context;
import lysa.math;

export namespace lysa {

    enum class RendererType : uint32 {
        FORWARD  = 0,
        DEFERRED = 1,
    };

    /**
    * Default clear color for windows and color frame buffers
    */
    const float3 DEFAULT_CLEAR_COLOR{0.0f, 0.0f, 0.0f};

    struct RendererConfiguration {
        RendererType rendererType{static_cast<int>(RendererType::DEFERRED)};
        //! Main color pass frame buffer format
        vireo::ImageFormat colorRenderingFormat{vireo::ImageFormat::R16G16B16A16_UNORM};
        //! Depth and stencil buffer format
        vireo::ImageFormat depthStencilFormat{vireo::ImageFormat::D32_SFLOAT_S8_UINT};
        //! Frame buffer clear color
        float3 clearColor{DEFAULT_CLEAR_COLOR};
        //! MSAA samples count
        vireo::MSAA msaa{vireo::MSAA::NONE};
        //
        // //! Gamma correction factor when using *_UNORM, *_SNORM or *_SFLOAT format
        // float              gamma{2.4f};
        // //! Exposure correction factor
        // float              exposure{1.0f};
        // //! Type of tone mapping shader when using HDR rendering formats R16G16B16A16_UNORM, R16G16B16A16_SFLOAT or R32G32B32A32_SFLOAT
        // ToneMappingType    toneMappingType{ToneMappingType::ACES};
        // //! Type of antialiasing post-processing shader
        // AntiAliasingType   antiAliasingType{AntiAliasingType::SMAA};
        // float              fxaaSpanMax{8.0f};
        // float              fxaaReduceMul{1.0f / 8.0f};
        // float              fxaaReduceMin{1.0f / 128.0f};
        // float              smaaEdgeThreshold{0.15f};
        // int                smaaBlendMaxSteps{4};
        // //! Enable the bloom post-processing effect
        // bool               bloomEnabled{true};
        // //! Bloom effect blur kernel size
        // uint32             bloomBlurKernelSize{5};
        // //! Bloom effect blur strength
        // float              bloomBlurStrength{1.2f};
#ifdef DEFERRED_RENDERER
        //! Enable SSAO in the deferred renderer
        bool               ssaoEnabled{true};
        //! SSAO blur kernel size
        uint32             ssaoBlurKernelSize{3};
        //! SSAO sampling count
        uint32             ssaoSampleCount{16};
        //! SSAO sampling radius
        float              ssaoRadius{0.5f};
        //! SSAO self-shadowing bias
        float              ssaoBias{0.025f};
        //! SSAO strength
        float              ssaoStrength{2.0f};
#endif
    };


}
