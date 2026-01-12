/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.renderpasses.gbuffer_pass;

import vireo;
import lysa.context;
import lysa.renderers.configuration;
import lysa.renderers.scene_frame_data;
import lysa.renderers.renderpasses.renderpass;
import lysa.resources.material;

export namespace lysa {

    /**
     * @brief Render pass for generating G-buffers
     */
    class GBufferPass : public Renderpass {
    public:
        /**
         * @brief Constructs a GBufferPass
         * @param ctx The engine context
         * @param config The renderer configuration
         * @param withStencil Whether to enable stencil testing
         */
        GBufferPass(
            const Context& ctx,
            const RendererConfiguration& config,
            bool withStencil);

        /**
         * @brief Updates the graphics pipelines based on active pipeline IDs
         * @param pipelineIds Map of pipeline IDs to unique object IDs
         */
        void updatePipelines(
            const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds);

        /**
         * @brief Renders the G-buffer pass
         * @param commandList The command list to record rendering commands into
         * @param scene The scene frame data
         * @param colorAttachment The target color attachment
         * @param depthAttachment The target depth attachment
         * @param clearAttachment Whether to clear the color attachment
         * @param frameIndex Index of the current frame
         */
        void render(
            vireo::CommandList& commandList,
            const SceneFrameData& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            bool clearAttachment,
            uint32 frameIndex);

        /**
         * @brief Resizes the render pass resources
         * @param extent The new extent
         * @param commandList Command list for resource transitions if needed
         */
        void resize(const vireo::Extent& extent, const std::shared_ptr<vireo::CommandList>& commandList) override;

        /**
         * @brief Gets the position buffer for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the position render target
         */
        auto getPositionBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].positionBuffer;
        }

        /**
         * @brief Gets the normal buffer for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the normal render target
         */
        auto getNormalBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].normalBuffer;
        }

        /**
         * @brief Gets the albedo buffer for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the albedo render target
         */
        auto getAlbedoBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].albedoBuffer;
        }

        /**
         * @brief Gets the emissive buffer for a specific frame
         * @param frameIndex Index of the current frame
         * @return A shared pointer to the emissive render target
         */
        auto getEmissiveBuffer(const uint32 frameIndex) const {
            return framesData[frameIndex].emissiveBuffer;
        }

    private:
        const std::string VERTEX_SHADER{"default.vert"};
        const std::string FRAGMENT_SHADER{"gbuffers.frag"};

        static constexpr int BUFFER_POSITION{0};
        static constexpr int BUFFER_NORMAL{1};
        static constexpr int BUFFER_ALBEDO{2};
        static constexpr int BUFFER_EMISSIVE{3};

        struct FrameData {
            std::shared_ptr<vireo::RenderTarget>  positionBuffer;
            std::shared_ptr<vireo::RenderTarget>  normalBuffer;
            std::shared_ptr<vireo::RenderTarget>  albedoBuffer;
            std::shared_ptr<vireo::RenderTarget>  emissiveBuffer;
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorRenderFormats  = {
                vireo::ImageFormat::R16G16B16A16_SFLOAT, // RGB: Position, A : View position Z
                vireo::ImageFormat::R8G8B8A8_SNORM, // RGB: Normal, A: roughness
                vireo::ImageFormat::R8G8B8A8_UNORM, // RGB: Albedo, A: metallic
                vireo::ImageFormat::R8G8B8A8_UNORM, // RGB: emissive color
            },
            .colorBlendDesc      = {
                {}, // Position
                {}, // Normal
                {}, // Albedo
                {}  // Emissive
            },
            .cullMode            = vireo::CullMode::BACK,
            .depthTestEnable     = true,
            .depthWriteEnable    = true,
            .frontStencilOpState = {
                .failOp      = vireo::StencilOp::KEEP,
                .passOp      = vireo::StencilOp::REPLACE,
                .depthFailOp = vireo::StencilOp::KEEP,
                .compareOp   = vireo::CompareOp::ALWAYS,
                .compareMask = 0xff,
                .writeMask   = 0xff
            }
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {
                { .clear = true }, // Position
                { .clear = true }, // Normal
                { .clear = true }, // Albedo
                { .clear = true }, // Emissive
            },
            .depthTestEnable    = pipelineConfig.depthTestEnable,
        };

        std::vector<FrameData> framesData;
        const MaterialManager& materialManager;
        std::unordered_map<pipeline_id, std::shared_ptr<vireo::GraphicPipeline>> pipelines;
    };
}