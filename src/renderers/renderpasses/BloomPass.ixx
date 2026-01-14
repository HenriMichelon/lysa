/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.bloom_pass;

import vireo;
import lysa.blur_data;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.renderpasses.post_processing;

export namespace lysa {

    /**
     * @brief Render pass for bloom effect
     */
    class BloomPass : public PostProcessing {
    public:
        /**
         * @brief Constructs a BloomPass
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        BloomPass(
            const Context& ctx,
            const RendererConfiguration& config);

        /**
          * @brief Updates the render pass state for the current frame
          * @param frameIndex Index of the current frame
          */
        void update(uint32 frameIndex) override;

        /**
         * @brief Renders the Bloom pass
        * @param frameIndex Index of the current frame
         * @param viewport The viewport to render into
         * @param scissor The scissor rectangle
         * @param colorAttachment The input color attachment
         * @param bloomAttachment The input bloom color attachment
         * @param commandList The command list to record rendering commands into
         */
        void render(
            vireo::CommandList& commandList,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissor,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& bloomAttachment,
            uint32 frameIndex) override;

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

    private:
        BlurData blurData;
        PostProcessing blurPass;
    };
}