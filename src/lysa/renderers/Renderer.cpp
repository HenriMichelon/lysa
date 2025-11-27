/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderer;

import lysa.exception;
#ifdef FORWARD_RENDERER
import lysa.renderers.forward_renderer;
#endif

namespace lysa {

    std::unique_ptr<Renderer> Renderer::create(
        Context& ctx,
        const RendererConfiguration& config,
        const SwapChainConfiguration& swapChainConfig) {
#ifdef FORWARD_RENDERER
        if (config.rendererType == RendererType::FORWARD) {
            return std::make_unique<ForwardRenderer>(ctx, config, swapChainConfig);
        }
#endif
        throw Exception("Unknown renderer type");
    }

    Renderer::Renderer(
        Context& ctx,
        const RendererConfiguration& config,
        const SwapChainConfiguration& swapChainConfig,
        const bool withStencil):
        ctx(ctx), withStencil(withStencil), config(config){
        framesData.resize(swapChainConfig.framesInFlight);
    }

    void Renderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        currentExtent = extent;
        for (auto& frame : framesData) {
            frame.colorAttachment = ctx.vireo->createRenderTarget(
               config.colorRenderingFormat,
               extent.width, extent.height,
               vireo::RenderTargetType::COLOR,
               {config.clearColor.r, config.clearColor.g, config.clearColor.b, 1.0f},
               1,
               config.msaa,
               "Main color attachment");
            frame.depthAttachment = ctx.vireo->createRenderTarget(
                config.depthStencilFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::DEPTH,
                { .depthStencil = { .depth = 1.0f, .stencil = 0 } },
                1,
                config.msaa,
                "Main depth stencil attachment");
            const auto depthStage =
               config.depthStencilFormat == vireo::ImageFormat::D32_SFLOAT_S8_UINT ||
               config.depthStencilFormat == vireo::ImageFormat::D24_UNORM_S8_UINT   ?
               vireo::ResourceState::RENDER_TARGET_DEPTH_STENCIL :
               vireo::ResourceState::RENDER_TARGET_DEPTH;
            commandList->barrier(
                frame.depthAttachment,
                vireo::ResourceState::UNDEFINED,
                depthStage);
        }
    }

    std::shared_ptr<vireo::RenderTarget> Renderer::getCurrentColorAttachment(const uint32 frameIndex) const {
        return getFrameColorAttachment(frameIndex);
    }

    void Renderer::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginNamespace("RendererType")
                .addVariable("FORWARD", RendererType::FORWARD)
                .addVariable("DEFERRED", RendererType::DEFERRED)
            .endNamespace()
            .beginClass<SwapChainConfiguration>("SwapChainConfiguration")
                .addConstructor<void()>()
                .addProperty("swap_chain_format", &SwapChainConfiguration::swapChainFormat)
                .addProperty("present_mode", &SwapChainConfiguration::presentMode)
                .addProperty("frames_in_flight", &SwapChainConfiguration::framesInFlight)
            .endClass()
            .beginClass<RendererConfiguration>("RendererConfiguration")
                .addConstructor<void()>()
                .addProperty("renderer_type", &RendererConfiguration::rendererType)
                .addProperty("color_rendering_format", &RendererConfiguration::colorRenderingFormat)
                .addProperty("depth_stencil_format", &RendererConfiguration::depthStencilFormat)
            .endClass()
    .endNamespace();
    }
}