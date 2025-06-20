/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpass.post_processing;

import lysa.application;
import lysa.global;
import lysa.virtual_fs;

namespace lysa {
    PostProcessing::PostProcessing(
        const RenderingConfiguration& config,
        const std::wstring& fragShaderName,
        void* data,
        const uint32 dataSize,
        const std::wstring& name):
        Renderpass{config, name},
        fragShaderName{fragShaderName},
        data{data},
        descriptorLayout{Application::getVireo().createDescriptorLayout(name)} {
        descriptorLayout->add(BINDING_PARAMS, vireo::DescriptorType::UNIFORM);
        descriptorLayout->add(BINDING_INPUT, vireo::DescriptorType::SAMPLED_IMAGE);
        if (data) {
            descriptorLayout->add(BINDING_DATA, vireo::DescriptorType::UNIFORM);
            dataUniform = Application::getVireo().createBuffer(vireo::BufferType::UNIFORM, dataSize, 1, name + L" Data");
            dataUniform->map();
            dataUniform->write(data, dataSize);
            dataUniform->unmap();
        }
        descriptorLayout->build();

        const auto& vireo = Application::getVireo();
        pipelineConfig.colorRenderFormats.push_back(config.renderingFormat);
        pipelineConfig.resources = vireo.createPipelineResources({
            descriptorLayout,
            Application::getResources().getSamplers().getDescriptorLayout()},
            {},
            name);
        pipelineConfig.vertexShader = loadShader(VERTEX_SHADER);
        pipelineConfig.fragmentShader = loadShader(fragShaderName + L".frag");
        pipeline = vireo.createGraphicPipeline(pipelineConfig, name);

        framesData.resize(config.framesInFlight);
        for (auto& frame : framesData) {
            frame.paramsUniform = vireo.createBuffer(vireo::BufferType::UNIFORM, sizeof(PostProcessingParams), 1, name + L" Params");
            frame.paramsUniform->map();
            frame.descriptorSet = vireo.createDescriptorSet(descriptorLayout, name);
            frame.descriptorSet->update(BINDING_PARAMS, frame.paramsUniform);
            if (data) {
                frame.descriptorSet->update(BINDING_DATA, dataUniform);
            }
        }
    }

    void PostProcessing::update(const uint32 frameIndex) {
        auto& frame = framesData[frameIndex];
        frame.params.time = getCurrentTimeMilliseconds();
        frame.paramsUniform->write(&frame.params, sizeof(frame.params));
    }

    void PostProcessing::render(
        const uint32 frameIndex,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissor,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        vireo::CommandList& commandList,
        const bool) {
        auto& frame = framesData[frameIndex];

        frame.descriptorSet->update(BINDING_INPUT, colorAttachment->getImage());
        renderingConfig.colorRenderTargets[0].renderTarget = frame.colorAttachment;
        commandList.barrier(
            frame.colorAttachment,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.beginRendering(renderingConfig);
        commandList.setViewport(viewport);
        commandList.setScissors(scissor);
        commandList.bindPipeline(pipeline);
        commandList.bindDescriptors({
            frame.descriptorSet,
            Application::getResources().getSamplers().getDescriptorSet()});
        commandList.draw(3);
        commandList.endRendering();
        commandList.barrier(
            frame.colorAttachment,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::UNDEFINED);
    }

    void PostProcessing::resize(const vireo::Extent& extent) {
        for (auto& frame : framesData) {
            frame.colorAttachment = Application::getVireo().createRenderTarget(
                config.renderingFormat,
                extent.width, extent.height,
                vireo::RenderTargetType::COLOR,
    {},
                config.msaa,
                name);
            frame.params.imageSize.x = extent.width;
            frame.params.imageSize.y = extent.height;
        }
    }

}