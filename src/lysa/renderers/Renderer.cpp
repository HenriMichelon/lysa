/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderer;

import lysa.application;
import lysa.log;

namespace lysa {
    Renderer::Renderer(
        const RenderingConfiguration& config,
        const bool withStencil,
        const std::wstring& name) :
        config{config},
        name{name},
        withStencil{withStencil},
        bloomBlurData{ config.bloomSize, config.bloomStrength },
        depthPrePass{config, withStencil},
        shaderMaterialPass{config},
        transparencyPass{config} {
        if (config.bloomEnabled) {
            bloomBlurPass = std::make_unique<PostProcessing>(
                config,
                L"blur",
                true,
                &bloomBlurData,
                sizeof(bloomBlurData),
                L"Bloom blur");
        }
        framesData.resize(config.framesInFlight);
    }

    void Renderer::update(const uint32 frameIndex) {
        depthPrePass.update(frameIndex);
        for (const auto& postProcessingPass : postProcessingPasses) {
            postProcessingPass->update(frameIndex);
        }
        if (config.bloomEnabled) { bloomBlurPass->update(frameIndex); }
    }

    void Renderer::updatePipelines(const Scene& scene) {
        const auto& pipelineIds = scene.getPipelineIds();
        for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            static_pointer_cast<ShadowMapPass>(shadowMapRenderer)->updatePipelines(pipelineIds);
        }
        depthPrePass.updatePipelines(pipelineIds);
        shaderMaterialPass.updatePipelines(pipelineIds);
        transparencyPass.updatePipelines(pipelineIds);
        updatePipelines(pipelineIds);
    }

    void Renderer::compute(
       vireo::CommandList& commandList,
       Scene& scene,
       const uint32 frameIndex) const {
        auto resourcesLock = std::lock_guard{Application::getResources().getMutex()};
        for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            shadowMapRenderer->update(frameIndex);
        }
        scene.update(commandList);
        scene.compute(commandList);
    }

    void Renderer::preRender(
        vireo::CommandList& commandList,
        const Scene& scene,
        const uint32 frameIndex) {
        auto resourcesLock = std::lock_guard{Application::getResources().getMutex()};
        commandList.bindVertexBuffer(Application::getResources().getVertexArray().getBuffer());
        commandList.bindIndexBuffer(Application::getResources().getIndexArray().getBuffer());
        for (const auto& shadowMapRenderer : scene.getShadowMapRenderers()) {
            static_pointer_cast<ShadowMapPass>(shadowMapRenderer)->render(commandList, scene);
        }
        scene.setInitialState(commandList);
        depthPrePass.render(commandList, scene, framesData[frameIndex].depthAttachment);
    }

    void Renderer::render(
        vireo::CommandList& commandList,
        const Scene& scene,
        const bool clearAttachment,
        const uint32 frameIndex) {
        auto resourcesLock = std::lock_guard{Application::getResources().getMutex()};
        const auto& frame = framesData[frameIndex];
        commandList.bindVertexBuffer(Application::getResources().getVertexArray().getBuffer());
        commandList.bindIndexBuffer(Application::getResources().getIndexArray().getBuffer());
        scene.setInitialState(commandList);
        colorPass(commandList, scene, frame.colorAttachment, frame.depthAttachment, clearAttachment, frameIndex);
        shaderMaterialPass.render(commandList, scene, frame.colorAttachment, frame.depthAttachment, false, frameIndex);
        transparencyPass.render(commandList, scene, frame.colorAttachment, frame.depthAttachment, false, frameIndex);
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
            auto colorAttachment = frame.colorAttachment;
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
        commandList.barrier(
            frame.colorAttachment,
            vireo::ResourceState::SHADER_READ,
            vireo::ResourceState::UNDEFINED);
    }

    std::shared_ptr<vireo::RenderTarget> Renderer::getColorAttachment(const uint32 frameIndex) const {
        if (postProcessingPasses.empty()) {
            return framesData[frameIndex].colorAttachment;
        }
        return postProcessingPasses.back()->getColorAttachment(frameIndex);
    }

    void Renderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        currentExtent = extent;
        for (auto& frame : framesData) {
            frame.colorAttachment = Application::getVireo().createRenderTarget(
                config.colorRenderingFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::COLOR,
                {config.clearColor.r, config.clearColor.g, config.clearColor.b, 1.0f},
                1,
                config.msaa,
                name);
            frame.depthAttachment = Application::getVireo().createRenderTarget(
                config.depthStencilFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::DEPTH,
                { .depthStencil = { .depth = 1.0f, .stencil = 0 } },
                1,
                config.msaa,
                name + L" DepthStencil");
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
        transparencyPass.resize(extent, commandList);
        if (config.bloomEnabled) { bloomBlurPass->resize(extent, commandList); }
        for (const auto& postProcessingPass : postProcessingPasses) {
            postProcessingPass->resize(extent, commandList);
        }
    }

    void Renderer::addPostprocessing(
        const std::wstring& fragShaderName,
        const bool useRenderingColorAttachmentFormat,
        void* data,
        const uint32 dataSize) {
        const auto postProcessingPass = std::make_shared<PostProcessing>(
            config,
            fragShaderName,
            useRenderingColorAttachmentFormat,
            data,
            dataSize,
            fragShaderName);
        postProcessingPass->resize(currentExtent, nullptr);
        postProcessingPasses.push_back(postProcessingPass);
    }

    void Renderer::removePostprocessing(const std::wstring& fragShaderName) {
        std::erase_if(postProcessingPasses, [&fragShaderName](const std::shared_ptr<PostProcessing>& item) {
            return item->getFragShaderName() == fragShaderName;
        });
    }

}