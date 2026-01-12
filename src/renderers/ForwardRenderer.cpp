/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.forward_renderer;

namespace lysa {

    ForwardRenderer::ForwardRenderer(
        const Context& ctx,
        const RendererConfiguration& config) :
        Renderer(ctx, config, false),
        forwardColorPass(ctx, config) {
    }

    void ForwardRenderer::update(const uint32 frameIndex) {
        Renderer::update(frameIndex);
        forwardColorPass.update(frameIndex);
    }

    void ForwardRenderer::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        Renderer::updatePipelines(pipelineIds);
        forwardColorPass.updatePipelines(pipelineIds);
    }

    void ForwardRenderer::colorPass(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const vireo::Viewport&,
        const vireo::Rect&,
        const bool clearAttachment,
        const uint32 frameIndex) {
        const auto& frame = framesData[frameIndex];
        forwardColorPass.render(
            commandList,
            scene,
            frame.colorAttachment,
            frame.depthAttachment,
            depthPrePass.getMultisampledDepthAttachment(frameIndex),
            clearAttachment,
            frameIndex);
    }

    void ForwardRenderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        Renderer::resize(extent, commandList);
        forwardColorPass.resize(extent, commandList);
    }

}