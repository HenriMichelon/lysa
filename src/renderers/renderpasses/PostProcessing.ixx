/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.post_processing;

import vireo;
import lysa.context;
import lysa.exception;
import lysa.math;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.renderpass;

export namespace lysa {

    /**
     * @brief Base class for post-processing render passes
     */
    class PostProcessing : public Renderpass {
    public:
        /**
         * @brief Parameters for the post-processing shader
         */
        struct PostProcessingParams {
            float  time;       /**< Current time for time-based effects */
            uint2  imageSize;  /**< Size of the image being processed */
        };

        /** @brief Name of the default vertex shader for post-processing */
        inline static const std::string VERTEX_SHADER{"quad.vert"};

        /** @brief Descriptor binding index for parameters uniform buffer */
        static constexpr vireo::DescriptorIndex BINDING_PARAMS{0};
        /** @brief Descriptor binding index for custom data uniform buffer */
        static constexpr vireo::DescriptorIndex BINDING_DATA{1};
        /** @brief Descriptor binding index for input textures */
        static constexpr vireo::DescriptorIndex BINDING_TEXTURES{2};

        /** @brief Index for the input color buffer texture */
        static constexpr int INPUT_BUFFER{0};
        /** @brief Index for the depth buffer texture */
        static constexpr int DEPTH_BUFFER{1};
        /** @brief Index for the bloom buffer texture */
        static constexpr int BLOOM_BUFFER{2};
        /** @brief Total number of input textures */
        static constexpr int TEXTURES_COUNT{BLOOM_BUFFER+1};

        /**
         * @brief Constructs a PostProcessing render pass
         * @param ctx The engine context
         * @param config The renderer configuration
         * @param fragShaderName Name of the fragment shader to use
         * @param outputFormat Format of the output image
         * @param data Pointer to custom data for the shader
         * @param dataSize Size of the custom data
         * @param name Name of the render pass
         */
        PostProcessing(
            const Context& ctx,
            const RendererConfiguration& config,
            const std::string& fragShaderName,
            vireo::ImageFormat outputFormat = vireo::ImageFormat::UNDEFINED,
            void* data = nullptr,
            uint32 dataSize = 0,
            const std::string& name = "");

        /**
         * @brief Updates the render pass state for the current frame
         * @param frameIndex Index of the current frame
         */
        void update(uint32 frameIndex) override;

        /**
         * @brief Renders the post-processing effect
         * @param frameIndex Index of the current frame
         * @param viewport The viewport to render into
         * @param scissor The scissor rectangle
         * @param colorAttachment The input color attachment
         * @param depthAttachment The input depth attachment
         * @param commandList The command list to record rendering commands into
         */
        virtual void render(
           vireo::CommandList& commandList,
           const vireo::Viewport&viewport,
           const vireo::Rect&scissor,
           const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
           const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
           uint32 frameIndex);

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         */
        virtual void resize(const vireo::Extent& extent);

        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override {
            throw Exception("Not implemented for post-processing passes");
        }

        /**
         * @brief Gets the color attachment for a specific frame
         * @param frameIndex Index of the frame
         * @return A shared pointer to the render target
         */
        virtual std::shared_ptr<vireo::RenderTarget> getColorAttachment(const uint32 frameIndex) {
            return framesData[frameIndex].colorAttachment;
        }

        /**
         * @brief Gets the name of the fragment shader
         * @return The fragment shader name
         */
        const auto& getFragShaderName() const { return fragShaderName; }

    protected:
        struct FrameData {
            PostProcessingParams                  params;
            std::shared_ptr<vireo::Buffer>        paramsUniform;
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
            std::shared_ptr<vireo::RenderTarget>  colorAttachment;
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = {{}}
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{}}
        };

        const std::string fragShaderName;
        uint8 dummyData{0};
        void* data{nullptr};
        std::shared_ptr<vireo::Buffer> dataUniform{nullptr};
        std::vector<FrameData> framesData;
        std::vector<std::shared_ptr<vireo::Image>> textures;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
        std::shared_ptr<vireo::GraphicPipeline> pipeline;

        /**
         * @brief Renders the post-processing effect
         * @param frameIndex Index of the current frame
         * @param viewport The viewport to render into
         * @param scissor The scissor rectangle
         * @param colorAttachment The input color attachment
         * @param depthAttachment The input depth attachment
         * @param bloomColorAttachment The input bloom color attachment
         * @param commandList The command list to record rendering commands into
         */
        void render(
           vireo::CommandList& commandList,
           const vireo::Viewport&viewport,
           const vireo::Rect&scissor,
           const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
           const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
           const std::shared_ptr<vireo::RenderTarget>& bloomColorAttachment,
           uint32 frameIndex);
    };
}