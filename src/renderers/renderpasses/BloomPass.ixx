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
import lysa.math;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
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
         * @param commandList The command list to record rendering commands into
         * @param colorAttachment The target depth attachment
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const vireo::Viewport& viewport,
            const vireo::Rect& scissor,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& bloomAttachment,
            uint32 frameIndex);

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

    private:
        const std::string VERTEX_SHADER{"quad.vert"};
        const std::string FRAGMENT_SHADER_BLUR{"bloom_blur.frag"};
        const std::string FRAGMENT_SHADER{"bloom.frag"};

        BlurData blurData;
        PostProcessing blurPass;
    };
}