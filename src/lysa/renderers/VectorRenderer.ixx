/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <cstdlib>
export module lysa.renderers.vector;

import std;
import vireo;
import lysa.global;
import lysa.configuration;
import lysa.scene;

export namespace lysa {

    class VectorRenderer {
    public:
        VectorRenderer(
            bool depthTestEnable,
            const RenderingConfiguration& renderingConfiguration,
            const std::wstring& name,
            const std::wstring& shadersName = L"vector",
            const vireo::PushConstantsDesc& pushConstantsDesc = {},
            const void* pushConstants = nullptr);

        void drawLine(const float3& from, const float3& to, const float4& color);

        void drawTriangle(const float3& v1, const float3& v2, const float3& v3, const float4& color);

        void restart();

        void update(
            const vireo::CommandList& commandList,
            uint32 frameIndex);

        void render(
            vireo::CommandList& commandList,
            const Scene& scene,
            const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
            const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
            uint32 frameIndex);

        virtual ~VectorRenderer() = default;
        VectorRenderer(VectorRenderer&) = delete;
        VectorRenderer& operator=(VectorRenderer&) = delete;

    protected:
        struct Vertex {
            alignas(16) float3 position;
            alignas(16) float4 color;
        };

        // Vertex buffer needs to be re-uploaded to GPU
        bool vertexBufferDirty{true};
        // All the vertices for lines
        std::vector<Vertex> linesVertices;
        // All the vertices for triangles
        std::vector<Vertex> triangleVertices;

        const void* pushConstants;
        const vireo::PushConstantsDesc pushConstantsDesc;
        const RenderingConfiguration& config;

    private:
        const std::wstring name;

        struct GlobalUniform {
            float4x4 projection{1.0f};
            float4x4 view{1.0f};
        };

        struct FrameData {
            std::shared_ptr<vireo::Buffer> globalUniform;
            std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        };

        const std::vector<vireo::VertexAttributeDesc> vertexAttributes{
            {"POSITION", vireo::AttributeFormat::R32G32B32_FLOAT, offsetof(Vertex, position)},
            {"COLOR",    vireo::AttributeFormat::R32G32B32A32_FLOAT, offsetof(Vertex, color)}
        };

        vireo::GraphicPipelineConfiguration pipelineConfig {
            .colorBlendDesc = {{ .blendEnable = false }},
            .cullMode = vireo::CullMode::NONE,
        };

        vireo::RenderingConfiguration renderingConfig {
            .colorRenderTargets = {{ }},
        };

        vireo::Extent currentExtent{};
        std::vector<FrameData> framesData;

        // Number of vertices for the currently allocated buffers, used to check if we need to resize the buffers
        uint32 vertexCount{0};
        // Staging vertex buffer used when updating GPU memory
        std::shared_ptr<vireo::Buffer> stagingBuffer;
        // Vertex buffer in GPU memory
        std::shared_ptr<vireo::Buffer> vertexBuffer;
        // Used when we need to postpone the buffers destruction when they are in use by another frame in flight
        std::list<std::shared_ptr<vireo::Buffer>> oldBuffers;

        std::shared_ptr<vireo::GraphicPipeline>  pipelineLines;
        std::shared_ptr<vireo::GraphicPipeline>  pipelineTriangles;
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;

    };
}