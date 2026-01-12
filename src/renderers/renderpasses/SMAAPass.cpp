/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.smaa_pass;


namespace lysa {

    SMAAPass::SMAAPass(
        const Context& ctx,
        const RendererConfiguration& config):
        Renderpass{ctx, config, "SMAA"},
        data{ .edgeThreshold = config.smaaEdgeThreshold, .blendMaxSteps = config.smaaBlendMaxSteps } {

        textures.resize(3);

        descriptorLayout = ctx.vireo->createDescriptorLayout();
        descriptorLayout->add(PostProcessing::BINDING_PARAMS, vireo::DescriptorType::UNIFORM);
        descriptorLayout->add(PostProcessing::BINDING_DATA, vireo::DescriptorType::UNIFORM);
        descriptorLayout->add(PostProcessing::BINDING_TEXTURES, vireo::DescriptorType::SAMPLED_IMAGE, textures.size());
        descriptorLayout->build();

        dataBuffer = ctx.vireo->createBuffer(vireo::BufferType::UNIFORM, sizeof(Data), 1, name + " Data");
        dataBuffer->map();
        dataBuffer->write(&data);
        dataBuffer->unmap();

        paramsBuffer = ctx.vireo->createBuffer(vireo::BufferType::UNIFORM, sizeof(PostProcessing::PostProcessingParams), 1, name + " Params");
        paramsBuffer->map();

        pipelineConfig.resources = ctx.vireo->createPipelineResources({
            descriptorLayout,
            ctx.samplers.getDescriptorLayout()},
        {}, name);
        pipelineConfig.vertexShader = loadShader(PostProcessing::VERTEX_SHADER);

        pipelineConfig.fragmentShader = loadShader(EDGE_DETECT_FRAGMENT_SHADER);
        edgeDetectPipeline = ctx.vireo->createGraphicPipeline(pipelineConfig, name);

        pipelineConfig.fragmentShader = loadShader(BLEND_WEIGHT_FRAGMENT_SHADER);
        blendWeightPipeline = ctx.vireo->createGraphicPipeline(pipelineConfig);

        pipelineConfig.colorRenderFormats[0] = config.swapChainFormat;
        pipelineConfig.fragmentShader = loadShader(BLEND_FRAGMENT_SHADER);
        blendPipeline = ctx.vireo->createGraphicPipeline(pipelineConfig);

        framesData.resize(ctx.config.framesInFlight);
        for (auto& frame : framesData) {
            frame.descriptorSet = ctx.vireo->createDescriptorSet(descriptorLayout);
            frame.descriptorSet->update(PostProcessing::BINDING_PARAMS, paramsBuffer);
            frame.descriptorSet->update(PostProcessing::BINDING_DATA, dataBuffer);
        }
    }

    void SMAAPass::render(
        vireo::CommandList& commandList,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];

        textures[PostProcessing::INPUT_BUFFER] = colorAttachment->getImage();
        textures[PostProcessing::DEPTH_BUFFER] = frame.edgeDetectBuffer->getImage();
        textures[PostProcessing::BLOOM_BUFFER] = frame.blendWeightBuffer->getImage();
        frame.descriptorSet->update(PostProcessing::BINDING_TEXTURES, textures);

        renderingConfig.colorRenderTargets[0].renderTarget = frame.edgeDetectBuffer;
        commandList.barrier(
           frame.edgeDetectBuffer,
           vireo::ResourceState::SHADER_READ,
           vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.bindPipeline(edgeDetectPipeline);
        commandList.bindDescriptors({
           frame.descriptorSet,
           ctx.samplers.getDescriptorSet()});
        commandList.beginRendering(renderingConfig);
        commandList.draw(3);
        commandList.endRendering();
        commandList.barrier(
            frame.edgeDetectBuffer,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::SHADER_READ);

        renderingConfig.colorRenderTargets[0].renderTarget = frame.blendWeightBuffer;
        commandList.barrier(
           frame.blendWeightBuffer,
           vireo::ResourceState::SHADER_READ,
           vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.bindPipeline(blendWeightPipeline);
        commandList.bindDescriptors({
           frame.descriptorSet,
           ctx.samplers.getDescriptorSet()});
        commandList.beginRendering(renderingConfig);
        commandList.draw(3);
        commandList.endRendering();
        commandList.barrier(
            frame.blendWeightBuffer,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::SHADER_READ);

        renderingConfig.colorRenderTargets[0].renderTarget = frame.colorBuffer;
        commandList.barrier(
           frame.colorBuffer,
           vireo::ResourceState::UNDEFINED,
           vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.bindPipeline(blendPipeline);
        commandList.bindDescriptors({
           frame.descriptorSet,
           ctx.samplers.getDescriptorSet()});
        commandList.beginRendering(renderingConfig);
        commandList.draw(3);
        commandList.endRendering();
        commandList.barrier(
            frame.colorBuffer,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::SHADER_READ);
    }

    void SMAAPass::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        for (auto& frame : framesData) {
            frame.edgeDetectBuffer = ctx.vireo->createRenderTarget(
                vireo::ImageFormat::R16G16_SFLOAT,
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[0].clearValue,
                1,
                vireo::MSAA::NONE,
                "SMAA Edge detect");
            frame.blendWeightBuffer = ctx.vireo->createRenderTarget(
                vireo::ImageFormat::R16G16_SFLOAT,
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[0].clearValue,
                1,
                vireo::MSAA::NONE,
                "SMAA Blend weight");
            frame.colorBuffer = ctx.vireo->createRenderTarget(
                config.swapChainFormat,
                extent.width,extent.height,
                vireo::RenderTargetType::COLOR,
                renderingConfig.colorRenderTargets[0].clearValue,
                1,
                vireo::MSAA::NONE,
                "SMAA Color");
            commandList->barrier(
                { frame.edgeDetectBuffer, frame.blendWeightBuffer },
                vireo::ResourceState::UNDEFINED,
                vireo::ResourceState::SHADER_READ);
        }
        params.imageSize = {extent.width, extent.height};
        paramsBuffer->write(&params);
    }

}