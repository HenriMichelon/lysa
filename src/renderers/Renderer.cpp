/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderer;

import lysa.exception;
import lysa.renderers.renderpasses.renderpass;
import lysa.renderers.renderpasses.shadow_map_pass;
#ifdef FORWARD_RENDERER
import lysa.renderers.forward_renderer;
#endif
#ifdef DEFERRED_RENDERER
import lysa.renderers.deferred_renderer;
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
#ifdef DEFERRED_RENDERER
        if (config.rendererType == RendererType::DEFERRED) {
            return std::make_unique<DeferredRenderer>(ctx, config);
        }
#endif
        throw Exception("Unknown renderer type");
    }

    Renderer::Renderer(
        const Context& ctx,
        const RendererConfiguration& config,
        const bool withStencil):
        ctx(ctx),
        withStencil(withStencil),
        config(config),
        depthPrePass(ctx, config, withStencil),
        meshManager(ctx.res.get<MeshManager>()),
        bloomBlurData{ .kernelSize = config.bloomBlurKernelSize },
        shaderMaterialPass(ctx, config),
        transparencyPass(ctx, config) {
        if (config.bloomEnabled) {
            bloomBlurPass = std::make_unique<PostProcessing>(
                ctx,
                config,
                "bloom_blur",
                config.colorRenderingFormat,
                &bloomBlurData,
                sizeof(bloomBlurData),
                "Bloom blur");
        }
        const auto needToneMapping =
            config.colorRenderingFormat == vireo::ImageFormat::R16G16B16A16_UNORM ||
            config.colorRenderingFormat == vireo::ImageFormat::R32G32B32A32_SFLOAT ||
            config.colorRenderingFormat == vireo::ImageFormat::R16G16B16A16_SFLOAT;
        const auto needGammaCorrection =
            config.colorRenderingFormat == vireo::ImageFormat::R8G8B8A8_UNORM ||
            config.colorRenderingFormat == vireo::ImageFormat::R8G8B8A8_SNORM;
        if (!needGammaCorrection && !needToneMapping && config.bloomEnabled) {
            addPostprocessing(
               "bloom",
               config.colorRenderingFormat,
               nullptr);
        }

        framesData.resize(ctx.config.framesInFlight);
    }

    void Renderer::update(const uint32 frameIndex) {
        depthPrePass.update(frameIndex);
        if (config.bloomEnabled) {
            bloomBlurPass->update(frameIndex);
        }
    }

    void Renderer::updatePipelines(const SceneFrameData& scene) {
        const auto& pipelineIds = scene.getPipelineIds();
        updatePipelines(pipelineIds);
    }

    void Renderer::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        depthPrePass.updatePipelines(pipelineIds);
        shaderMaterialPass.updatePipelines(pipelineIds);
        transparencyPass.updatePipelines(pipelineIds);
    }

    void Renderer::prepare(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors,
        const uint32 frameIndex) {
        commandList.bindVertexBuffer(meshManager.getVertexBuffer());
        commandList.bindIndexBuffer(meshManager.getIndexBuffer());
        for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            static_pointer_cast<ShadowMapPass>(shadowMapRenderer)->render(commandList, scene);
        }
        commandList.setViewport(viewport);
        commandList.setScissors(scissors);
        depthPrePass.render(commandList, scene, framesData[frameIndex].depthAttachment, frameIndex);
    }

    void Renderer::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors,
        const bool clearAttachment,
        const uint32 frameIndex) {
        commandList.bindVertexBuffer(meshManager.getVertexBuffer());
        commandList.bindIndexBuffer(meshManager.getIndexBuffer());
        commandList.setViewport(viewport);
        commandList.setScissors(scissors);
        colorPass(
            commandList,
            scene,
            viewport,
            scissors,
            clearAttachment,
            frameIndex);
        const auto& frame = framesData[frameIndex];
        shaderMaterialPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            false,
            frameIndex);
        transparencyPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            false,
            frameIndex);
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
               vireo::MSAA::NONE,
               "Main color attachment");
            frame.depthAttachment = ctx.vireo->createRenderTarget(
                config.depthStencilFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::DEPTH,
                { .depthStencil = { .depth = 1.0f, .stencil = 0 } },
                1,
                vireo::MSAA::NONE,
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
        depthPrePass.resize(extent, commandList);
        shaderMaterialPass.resize(extent, commandList);
        transparencyPass.resize(extent, commandList);
        if (bloomBlurPass) {
            updateBlurData(bloomBlurData, extent, config.bloomBlurStrength);
            bloomBlurPass->resize(extent, commandList);
        }
        // if (smaaPass) {
        //     smaaPass->resize(extent, commandList);
        // }
        for (const auto& postProcessingPass : postProcessingPasses) {
            postProcessingPass->resize(extent, commandList);
        }
    }

    void Renderer::postprocess(
        vireo::CommandList& commandList,
        const vireo::Viewport&viewport,
        const vireo::Rect&scissor,
        uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];
        commandList.barrier(
            frame.colorAttachment,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::SHADER_READ);
        std::shared_ptr<vireo::RenderTarget> bloomColorAttachment;
        if (config.bloomEnabled) {
            bloomBlurPass->render(
                frameIndex,
                viewport,
                scissor,
                getBloomColorAttachment(frameIndex),
                nullptr,
                nullptr,
                commandList);
            bloomColorAttachment = bloomBlurPass->getColorAttachment(frameIndex);
        } else {
            bloomColorAttachment = getBloomColorAttachment(frameIndex);
        }
        auto colorAttachment = frame.colorAttachment;
        if (!postProcessingPasses.empty()) {
            const auto depthStage =
               config.depthStencilFormat == vireo::ImageFormat::D32_SFLOAT_S8_UINT ||
               config.depthStencilFormat == vireo::ImageFormat::D24_UNORM_S8_UINT   ?
               vireo::ResourceState::RENDER_TARGET_DEPTH_STENCIL :
               vireo::ResourceState::RENDER_TARGET_DEPTH;
            commandList.barrier(
               frame.depthAttachment,
               depthStage,
               vireo::ResourceState::SHADER_READ);
            std::ranges::for_each(postProcessingPasses, [&](const auto& postProcessingPass) {
                postProcessingPass->render(
                    frameIndex,
                    viewport,
                    scissor,
                    colorAttachment,
                    frame.depthAttachment,
                    bloomColorAttachment,
                    commandList);
                colorAttachment = postProcessingPass->getColorAttachment(frameIndex);
            });
            commandList.barrier(
               frame.depthAttachment,
               vireo::ResourceState::SHADER_READ,
               depthStage);
            std::ranges::for_each(postProcessingPasses, [&](const auto& postProcessingPass) {
                commandList.barrier(
                    postProcessingPass->getColorAttachment(frameIndex),
                    vireo::ResourceState::SHADER_READ,
                    vireo::ResourceState::UNDEFINED);
            });
        }
        // if (smaaPass) {
        //     smaaPass->render(
        //         commandList,
        //         colorAttachment,
        //         frameIndex);
        //     commandList.barrier(
        //         smaaPass->getColorAttachment(frameIndex),
        //        vireo::ResourceState::SHADER_READ,
        //        vireo::ResourceState::UNDEFINED);
        // }
        commandList.barrier(
            frame.colorAttachment,
            vireo::ResourceState::SHADER_READ,
            vireo::ResourceState::UNDEFINED);
        if (config.bloomEnabled) {
            commandList.barrier(
                bloomColorAttachment,
                vireo::ResourceState::SHADER_READ,
                vireo::ResourceState::UNDEFINED);
        }
    }

    std::shared_ptr<vireo::RenderTarget> Renderer::getCurrentColorAttachment(const uint32 frameIndex) const {
        // if (smaaPass) {
            // return smaaPass->getColorAttachment(frameIndex);
        // }
        if (postProcessingPasses.empty()) {
            return framesData[frameIndex].colorAttachment;
        }
        return postProcessingPasses.back()->getColorAttachment(frameIndex);
    }

    void Renderer::addPostprocessing(
       const std::string& fragShaderName,
       const vireo::ImageFormat outputFormat,
       void* data,
       const uint32 dataSize) {
        const auto postProcessingPass = std::make_shared<PostProcessing>(
            ctx,
            config,
            fragShaderName,
            outputFormat,
            data,
            dataSize,
            fragShaderName);
        postProcessingPass->resize(currentExtent, nullptr);
        postProcessingPasses.push_back(postProcessingPass);
    }

    void Renderer::removePostprocessing(const std::string& fragShaderName) {
        std::erase_if(postProcessingPasses, [&fragShaderName](const std::shared_ptr<PostProcessing>& item) {
            return item->getFragShaderName() == fragShaderName;
        });
    }

    void Renderer::updateBlurData(BlurData& blurData, const vireo::Extent& extent, const float strength) const {
        // Pre-compute Gaussian weights
        if (blurData.kernelSize > 9) { blurData.kernelSize = 9; }
        blurData.texelSize = (1.0 / float2(extent.width, extent.height)) * strength;
        const int halfKernel = blurData.kernelSize / 2;
        float sum = 0.0;
        for (int i = 0; i < blurData.kernelSize; i++) {
            for (int j = 0; j < blurData.kernelSize; j++) {
                const int index = i * blurData.kernelSize + j;
                const float x = static_cast<float>(i - halfKernel) * blurData.texelSize.x;
                const float y = static_cast<float>(j - halfKernel) * blurData.texelSize.y;
                blurData.weights[index].x = std::exp(-(x * x + y * y) / 2.0);
                sum += blurData.weights[index].x;
            }
        }
        // Normalize weights
        for (int i = 0; i < blurData.kernelSize * blurData.kernelSize; i++) {
            blurData.weights[i].x /= sum;
        }
    }
}