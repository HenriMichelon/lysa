/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderer;

import lysa.exception;
import lysa.renderers.renderpasses.renderpass;
#ifdef FORWARD_RENDERER
import lysa.renderers.forward_renderer;
#endif

namespace lysa {

    std::unique_ptr<Renderer> Renderer::create(
        Context& ctx,
        const RendererConfiguration& config) {
#ifdef FORWARD_RENDERER
        if (config.rendererType == RendererType::FORWARD) {
            return std::make_unique<ForwardRenderer>(ctx, config);
        }
#endif
        throw Exception("Unknown renderer type");
    }

    Renderer::Renderer(
        Context& ctx,
        const RendererConfiguration& config,
        const bool withStencil):
        ctx(ctx), withStencil(withStencil), config(config){
        framesData.resize(config.framesInFlight);
    }

    void Renderer::update(const uint32 frameIndex) {
    }

    void Renderer::render(
       vireo::CommandList& commandList,
       const bool clearAttachment,
       const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];
        colorPass(commandList, frame.colorAttachment, frame.depthAttachment, clearAttachment, frameIndex);
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
        Renderpass::_register(lua);
        lua.beginNamespace()
            .beginNamespace("RendererType")
                .addVariable("FORWARD", RendererType::FORWARD)
                .addVariable("DEFERRED", RendererType::DEFERRED)
            .endNamespace()
            .beginClass<RendererConfiguration>("RendererConfiguration")
                .addConstructor<void()>()
                .addProperty("rendererType", &RendererConfiguration::rendererType)
                .addProperty("swapChainFormat", &RendererConfiguration::swapChainFormat)
                .addProperty("presentMode", &RendererConfiguration::presentMode)
                .addProperty("framesInFlight", &RendererConfiguration::framesInFlight)
                .addProperty("colorRenderingFormat", &RendererConfiguration::colorRenderingFormat)
                .addProperty("depthStencilFormat", &RendererConfiguration::depthStencilFormat)
                .addProperty("clearColor", &RendererConfiguration::clearColor)
                .addProperty("msaa", &RendererConfiguration::msaa)
            .endClass()
            .beginClass<Renderer>("Renderer")
            .endClass()
        .endNamespace();
    }
}