/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.display_attachment;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.renderpasses.post_processing;

export namespace lysa {

    class DisplayAttachment : public PostProcessing {
    public:
        DisplayAttachment(
            const Context& ctx,
            const RendererConfiguration& config,
            const std::string& shader = "",
            void* data = nullptr, const uint32 dataSize = 0) :
            PostProcessing(
                ctx,
                config,
                shader.empty() ? "passthrough" : shader,
                data, dataSize,
                config.swapChainFormat,
                "DisplayAttachment") {
        }

        void setAttachment(const std::shared_ptr<vireo::RenderTarget>& attachment) {
            this->attachment = attachment;
        }

    private:
        std::shared_ptr<vireo::RenderTarget> attachment;

        void render(
            vireo::CommandList& commandList,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissor,
            const std::shared_ptr<vireo::RenderTarget>&,
            const std::shared_ptr<vireo::RenderTarget>&,
            const std::shared_ptr<vireo::RenderTarget>&,
            const uint32 frameIndex) override {
            if (attachment) {
                textures[INPUT_BUFFER] = attachment->getImage();
                _render(commandList, viewport, scissor, nullptr, nullptr, frameIndex);
            } else {
                commandList.barrier(
                    framesData[frameIndex].colorAttachment,
                    vireo::ResourceState::UNDEFINED,
                    vireo::ResourceState::RENDER_TARGET_COLOR);
                commandList.barrier(
                    framesData[frameIndex].colorAttachment,
                    vireo::ResourceState::RENDER_TARGET_COLOR,
                    vireo::ResourceState::SHADER_READ);
            }
        }
    };
}