/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderer;

import vireo;

import lysa.context;
import lysa.lua;
import lysa.math;
import lysa.types;

export namespace lysa {

    enum class RendererType : uint32 {
        FORWARD  = 0,
        DEFERRED = 1,
    };

    /**
    * Default clear color for windows and color frame buffers
    */
    const float3 DEFAULT_CLEAR_COLOR{0.0f, 0.0f, 0.0f};

    struct SwapChainConfiguration {
        //! Postprocessing & swap chain image format
        vireo::ImageFormat swapChainFormat{vireo::ImageFormat::R8G8B8A8_UNORM};
        //! Presentation mode
        vireo::PresentMode presentMode{vireo::PresentMode::IMMEDIATE};
        //! Number of simultaneous frames during rendering
        uint32 framesInFlight{2};
    };

    struct RendererConfiguration {
        RendererType rendererType{static_cast<int>(RendererType::DEFERRED)};
        //! Main color pass frame buffer format
        vireo::ImageFormat colorRenderingFormat{vireo::ImageFormat::R16G16B16A16_UNORM};
        //! Depth and stencil buffer format
        vireo::ImageFormat depthStencilFormat{vireo::ImageFormat::D32_SFLOAT_S8_UINT};
        //! Frame buffer clear color
        float3             clearColor{DEFAULT_CLEAR_COLOR};
        //! MSAA samples count
        vireo::MSAA        msaa{vireo::MSAA::NONE};

        /*
        //! Gamma correction factor when using *_UNORM, *_SNORM or *_SFLOAT format
        float              gamma{2.4f};
        //! Exposure correction factor
        float              exposure{1.0f};
        //! Type of tone mapping shader when using HDR rendering formats R16G16B16A16_UNORM, R16G16B16A16_SFLOAT or R32G32B32A32_SFLOAT
        ToneMappingType    toneMappingType{ToneMappingType::ACES};
        //! Type of antialiasing post-processing shader
        AntiAliasingType   antiAliasingType{AntiAliasingType::SMAA};
        float              fxaaSpanMax{8.0f};
        float              fxaaReduceMul{1.0f / 8.0f};
        float              fxaaReduceMin{1.0f / 128.0f};
        float              smaaEdgeThreshold{0.15f};
        int                smaaBlendMaxSteps{4};
        //! Enable the bloom post-processing effect
        bool               bloomEnabled{true};
        //! Bloom effect blur kernel size
        uint32             bloomBlurKernelSize{5};
        //! Bloom effect blur strength
        float              bloomBlurStrength{1.2f};
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
        float              ssaoStrength{2.0f};*/
    };

    /**
     * @breif High-level scene renderer
     *  - Own and manage the set of render passes required by a rendering path
     *    (depth pre-pass, opaque/transparent color, shader-material passes, SMAA,
     *    bloom and other post-processing).
     *  - Allocate per-frame color/depth attachments and expose them to callers.
     *  - Update and (re)build graphics pipelines when the set of materials changes.
     */
    class Renderer {
    public:
        static std::unique_ptr<Renderer> create(
            Context& ctx,
            const RendererConfiguration& config,
            const SwapChainConfiguration& swapChainConfig);

        /** Returns the color attachment of the current renderer for the frame. */
        std::shared_ptr<vireo::RenderTarget> getCurrentColorAttachment(uint32 frameIndex) const;

        /** Accessor for the color render target of the frame. */
        auto getFrameColorAttachment(const uint32 frameIndex) const {
            return framesData[frameIndex].colorAttachment;
        }

        /** Accessor for the depth render target of the frame. */
        auto getFrameDepthAttachment(const uint32 frameIndex) const {
            return framesData[frameIndex].depthAttachment;
        }

        /**
         * Recreates attachments/pipelines after a resize.
         * @param extent       New swap chain extent.
         * @param commandList  Command list used for any required transitions/copies.
         */
        virtual void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList);

        virtual ~Renderer() = default;
        Renderer(Renderer&) = delete;
        Renderer& operator=(Renderer&) = delete;

        static void _register(const Lua& lua);

    protected:
        Context& ctx;
        const bool withStencil;
        const RendererConfiguration config;

        Renderer(
            Context& ctx,
            const RendererConfiguration& config,
            const SwapChainConfiguration& swapChainConfig,
            bool withStencil);

    private:
        /** Per-frame attachments owned by the renderer. */
        struct FrameData {
            std::shared_ptr<vireo::RenderTarget> colorAttachment;
            std::shared_ptr<vireo::RenderTarget> depthAttachment;
        };

        vireo::Extent currentExtent{};
        std::vector<FrameData> framesData;

    };
}

template <> struct luabridge::Stack<lysa::RendererType> : Enum<lysa::RendererType> {};
