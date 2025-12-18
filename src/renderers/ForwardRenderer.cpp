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
        const RendererConfiguration& config,
        const uint32 framesInFlight) :
        Renderer(ctx, config, framesInFlight, false),
        forwardColorPass(ctx, config, framesInFlight) {
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
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const bool clearAttachment,
        const uint32 frameIndex) {
        forwardColorPass.render(commandList, scene, colorAttachment, depthAttachment, clearAttachment, frameIndex);
    }


    void ForwardRenderer::resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) {
        Renderer::resize(extent, commandList);
        forwardColorPass.resize(extent, commandList);
    }

}