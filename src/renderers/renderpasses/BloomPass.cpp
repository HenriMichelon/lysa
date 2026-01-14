/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.bloom_pass;

import lysa.resources.image;

namespace lysa {

    BloomPass::BloomPass(
        const Context& ctx,
        const RendererConfiguration& config):
        PostProcessing(
            ctx,
            config,
            "bloom",
            config.swapChainFormat,
            nullptr, 0,
    "Bloom"),
        blurData{ .kernelSize = config.bloomBlurKernelSize },
        blurPass(ctx,
            config,
            "bloom_blur",
            config.colorRenderingFormat,
            &blurData,
            sizeof(blurData),
            "Bloom blur")
    {

    }

    void BloomPass::update(const uint32 frameIndex) {
        PostProcessing::update(frameIndex);
        blurPass.update(frameIndex);
    }

    void BloomPass::render(
        vireo::CommandList& commandList,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissor,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const std::shared_ptr<vireo::RenderTarget>& bloomAttachment,
        const uint32 frameIndex) {
        blurPass.render(
                        commandList,
                        viewport,
                        scissor,
                        bloomAttachment,
                        nullptr,
                        frameIndex);
        PostProcessing::render(
            commandList,
            viewport,
            scissor,
            colorAttachment,
            nullptr,
            blurPass.getColorAttachment(frameIndex),
            frameIndex);
        commandList.barrier(
            framesData[frameIndex].colorAttachment,
            vireo::ResourceState::SHADER_READ,
            vireo::ResourceState::UNDEFINED);
        commandList.barrier(
                blurPass.getColorAttachment(frameIndex),
                vireo::ResourceState::SHADER_READ,
                vireo::ResourceState::UNDEFINED);
    }

    void BloomPass::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        PostProcessing::resize(extent, commandList);
        blurData.update(extent, config.bloomBlurStrength);
        blurPass.resize(extent, commandList);
    }


}