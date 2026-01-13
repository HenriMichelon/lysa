/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <hb.h>
module lysa.renderers.vector_3d;

import lysa.exception;

namespace lysa {

    Vector3DRenderer::Vector3DRenderer(
        const Context& ctx,
        const RendererConfiguration& config,
        const bool useCamera,
        const bool depthTestEnable,
        const bool filledTriangles,
        const bool enableAlphaBlending,
        const std::string& name,
        const std::string& shadersName,
        const std::string& glyphShadersName) :
        config{config},
        imageManager(ctx.res.get<ImageManager>()),
        useCamera{useCamera},
        name{name},
        ctx(ctx) {

        descriptorLayout = ctx.vireo->createDescriptorLayout(name);
        if (useCamera) {
            globalUniformIndex = 0;
            texturesIndex = 1;
            descriptorLayout->add(globalUniformIndex, vireo::DescriptorType::UNIFORM);
        } else {
            texturesIndex = 0;
        }

        textures.resize(MAX_TEXTURES);
        descriptorLayout->add(texturesIndex, vireo::DescriptorType::SAMPLED_IMAGE, textures.size());
        blankImage = ctx.res.get<ImageManager>().getBlankImage();
        for (int i = 0; i < textures.size(); i++) {
            textures[i] = blankImage;
        }
        descriptorLayout->build();

        fontsParams.resize(MAX_FONTS);
        fontDescriptorLayout = ctx.vireo->createDescriptorLayout(name + " fonts");
        fontDescriptorLayout->add(FONT_PARAMS_BINDING, vireo::DescriptorType::UNIFORM);
        fontDescriptorLayout->build();
        fontDescriptorSet = ctx.vireo->createDescriptorSet(fontDescriptorLayout);
        fontsParamsUniform = ctx.vireo->createBuffer(vireo::BufferType::UNIFORM, sizeof(FontParams) * fontsParams.size(), 1, name + " fonts params");
        fontsParamsUniform->map();
        fontsParamsUniform->write(fontsParams.data());
        fontDescriptorSet->update(FONT_PARAMS_BINDING, fontsParamsUniform);

        framesData.resize(ctx.config.framesInFlight);
        for (auto& frameData : framesData) {
            frameData.descriptorSet = ctx.vireo->createDescriptorSet(descriptorLayout, name);
            if (useCamera) {
                frameData.globalUniform = ctx.vireo->createBuffer(vireo::BufferType::UNIFORM, sizeof(GlobalUniform), 1, name);
                frameData.globalUniform->map();
                frameData.descriptorSet->update(globalUniformIndex, frameData.globalUniform);
            }
            frameData.descriptorSet->update(texturesIndex, textures);
        }
        renderingConfig.depthTestEnable = depthTestEnable;

        pipelineConfig.colorBlendDesc[0].blendEnable = enableAlphaBlending;
        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.depthTestEnable = depthTestEnable;
        pipelineConfig.depthWriteEnable = depthTestEnable;
        pipelineConfig.colorRenderFormats.push_back(config.swapChainFormat);
        pipelineConfig.vertexInputLayout = ctx.vireo->createVertexLayout(sizeof(Vertex), vertexAttributes);
        auto tempBuffer = std::vector<char>{};
        ctx.fs.loadShader(shadersName + ".vert", tempBuffer);
        pipelineConfig.vertexShader = ctx.vireo->createShaderModule(tempBuffer, shadersName + ".vert");
        ctx.fs.loadShader(shadersName + ".frag", tempBuffer);
        pipelineConfig.fragmentShader = ctx.vireo->createShaderModule(tempBuffer, shadersName + ".frag");
        pipelineConfig.resources = ctx.vireo->createPipelineResources(
           {
                descriptorLayout,
                ctx.samplers.getDescriptorLayout(),
                fontDescriptorLayout
           },
           {},
           name);
        pipelineConfig.primitiveTopology = vireo::PrimitiveTopology::LINE_LIST;
        pipelineLines = ctx.vireo->createGraphicPipeline(pipelineConfig, name + " lines");
        pipelineConfig.polygonMode = filledTriangles ?
            vireo::PolygonMode::FILL :
            vireo::PolygonMode::WIREFRAME;
        pipelineConfig.primitiveTopology = vireo::PrimitiveTopology::TRIANGLE_LIST;
        pipelineTriangles = ctx.vireo->createGraphicPipeline(pipelineConfig, name + " triangles");

        pipelineConfig.polygonMode = vireo::PolygonMode::FILL;
        pipelineConfig.depthWriteEnable = false;
        pipelineConfig.colorBlendDesc = glyphPipelineConfig.colorBlendDesc;
        pipelineImages = ctx.vireo->createGraphicPipeline(pipelineConfig, name + " images");

        ctx.fs.loadShader(glyphShadersName + ".vert", tempBuffer);
        pipelineConfig.vertexShader = ctx.vireo->createShaderModule(tempBuffer, glyphShadersName + ".vert");
        ctx.fs.loadShader(glyphShadersName + ".frag", tempBuffer);
        pipelineConfig.fragmentShader = ctx.vireo->createShaderModule(tempBuffer, glyphShadersName + ".frag");
        pipelineGlyphs = ctx.vireo->createGraphicPipeline(pipelineConfig, name + " glyphs");
    }

    void Vector3DRenderer::drawLine(const float3& from, const float3& to, const float4& color) {
        linesVertices.push_back( {from, {}, color});
        linesVertices.push_back( {to, {}, color });
        vertexBufferDirty = true;
    }

    void Vector3DRenderer::drawTriangle(const float3& v1, const float3& v2, const float3& v3, const float4& color) {
        triangleVertices.push_back( {v1, {}, color});
        triangleVertices.push_back( {v2, {}, color});
        triangleVertices.push_back( {v3, {}, color });
        vertexBufferDirty = true;
    }

    void Vector3DRenderer::drawImage(
        unique_id image,
        const float3& position,
        const quaternion& rotation,
        const float2& size,
        const float4& color) {
        /*
        * v1 ---- v3
        * |  \     |
        * |    \   |
        * v0 ---- v2
        */
        const auto rm = float4x4{rotation};
        const float3 v0 = mul({ position.x,  position.y + size.y, position.z, 1.0f }, rm).xyz;
        const float3 v1 = mul({ position.x,  position.y, position.z, 1.0f }, rm).xyz;
        const float3 v2 = mul({ position.x + size.x, position.y + size.y, position.z, 1.0f }, rm).xyz;
        const float3 v3 = mul({ position.x + size.x, position.y, position.z, 1.0f }, rm).xyz;

        auto textureIndex{-1};
        if (image != INVALID_ID) {
            textureIndex = addTexture(imageManager[image]);
        }

        imagesVertices.push_back( {v0, {0.0f, 1.0f}, color, textureIndex });
        imagesVertices.push_back( {v1, {0.0f, 0.0f}, color, textureIndex });
        imagesVertices.push_back( {v2, {1.0f, 1.0f}, color, textureIndex });
        imagesVertices.push_back( {v1, {0.0f, 0.0f}, color, textureIndex });
        imagesVertices.push_back( {v3, {1.0f, 0.0f}, color, textureIndex });
        imagesVertices.push_back( {v2, {1.0f, 1.0f}, color, textureIndex });
        vertexBufferDirty = true;
    }

    void Vector3DRenderer::drawText(
        const std::string& text,
        Font& font,
        const float fontScale,
        const float3& position,
        const quaternion& rotation,
        const float4& innerColor) {
        auto textureIndex = addTexture(font.getAtlas());
        auto fontIndex = addFont(font);
        auto pos = position;

        hb_buffer_t* hb_buffer = hb_buffer_create();
        hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
        hb_buffer_guess_segment_properties(hb_buffer);
        hb_shape(font.getHarfBuzzFont(), hb_buffer, nullptr, 0);
        unsigned int glyph_count;
        hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(hb_buffer, &glyph_count);
        //hb_glyph_position_t* glyph_pos = hb_buffer_get_glyph_positions(hb_buffer, &glyph_count);
        const auto rm = float4x4{rotation};

        for (unsigned int i = 0; i < glyph_count; i++) {
            auto glyphInfo = font.getGlyphInfo(glyph_info[i].codepoint);
            auto plane = Font::GlyphBounds{};
            plane.left = fontScale * (glyphInfo.planeBounds.left);
            plane.right = fontScale * (glyphInfo.planeBounds.right);
            plane.top = fontScale * (glyphInfo.planeBounds.top);
            plane.bottom = fontScale * (glyphInfo.planeBounds.bottom);
            /*
            * v1 ---- v3
            * |  \     |
            * |    \   |
            * v0 ---- v2
            */
            const float3 v0 = mul({ pos.x + plane.left,  pos.y + plane.bottom, pos.z, 1.0f }, rm).xyz;
            const float3 v1 = mul({ pos.x + plane.left,  pos.y + plane.top, pos.z, 1.0f }, rm).xyz;
            const float3 v2 = mul({ pos.x + plane.right, pos.y + plane.bottom, pos.z, 1.0f }, rm).xyz;
            const float3 v3 = mul({ pos.x + plane.right, pos.y + plane.top, pos.z, 1.0f }, rm).xyz;
            glyphVertices.push_back({v0, {glyphInfo.uv0.x, glyphInfo.uv1.y}, innerColor, textureIndex, fontIndex});
            glyphVertices.push_back({v1, {glyphInfo.uv0.x, glyphInfo.uv0.y}, innerColor, textureIndex, fontIndex});
            glyphVertices.push_back({v2, {glyphInfo.uv1.x, glyphInfo.uv1.y}, innerColor, textureIndex, fontIndex});
            glyphVertices.push_back({v1, {glyphInfo.uv0.x, glyphInfo.uv0.y}, innerColor, textureIndex, fontIndex});
            glyphVertices.push_back({v3, {glyphInfo.uv1.x, glyphInfo.uv0.y}, innerColor, textureIndex, fontIndex});
            glyphVertices.push_back({v2, {glyphInfo.uv1.x, glyphInfo.uv1.y}, innerColor, textureIndex, fontIndex});
            pos.x += fontScale * glyphInfo.advance;
        }

        hb_buffer_destroy(hb_buffer);
        vertexBufferDirty = true;
    }

    void Vector3DRenderer::restart() {
        linesVertices.clear();
        triangleVertices.clear();
        imagesVertices.clear();
        glyphVertices.clear();
    }

    void Vector3DRenderer::update(
        const vireo::CommandList& commandList,
        const uint32) {
        // Destroy the previous buffer when we are sure they aren't used by another frame
        oldBuffers.clear();
        if (!linesVertices.empty() || !triangleVertices.empty() || !imagesVertices.empty() || !glyphVertices.empty()) {
            const auto totalVertexCount = linesVertices.size() + triangleVertices.size() + imagesVertices.size() + glyphVertices.size();
            // Resize the buffers only if needed by recreating them
            if ((vertexBuffer == nullptr) ||
                (vertexCount != totalVertexCount)) {
                // Put the current buffers in the recycle bin since they are currently used
                // and can't be destroyed now
                oldBuffers.push_back(stagingBuffer);
                oldBuffers.push_back(vertexBuffer);
                // Allocate new buffers to change size
                vertexCount = totalVertexCount;
                stagingBuffer = ctx.vireo->createBuffer(vireo::BufferType::BUFFER_UPLOAD, sizeof(Vertex), vertexCount, name + " vertices staging");
                stagingBuffer->map();
                vertexBuffer = ctx.vireo->createBuffer(vireo::BufferType::VERTEX, sizeof(Vertex), vertexCount, name + " vertices");
                // commandList.barrier(*vertexBuffer, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
            }
            if (vertexBufferDirty) {
                // Push new vertices data to GPU memory
                if (!linesVertices.empty()) {
                    stagingBuffer->write(linesVertices.data(), linesVertices.size() * sizeof(Vertex));
                }
                if (!triangleVertices.empty()) {
                    stagingBuffer->write(
                        triangleVertices.data(),
                        triangleVertices.size() * sizeof(Vertex),
                        linesVertices.size() * sizeof(Vertex));
                }
                if (!imagesVertices.empty()) {
                    stagingBuffer->write(
                        imagesVertices.data(),
                        imagesVertices.size() * sizeof(Vertex),
                        (linesVertices.size() + triangleVertices.size()) * sizeof(Vertex));
                }
                if (!glyphVertices.empty()) {
                    stagingBuffer->write(
                        glyphVertices.data(),
                        glyphVertices.size() * sizeof(Vertex),
                        (linesVertices.size() + triangleVertices.size() + imagesVertices.size()) * sizeof(Vertex));
                }
                commandList.copy(stagingBuffer, vertexBuffer);
                commandList.barrier(*vertexBuffer, vireo::ResourceState::COPY_DST, vireo::ResourceState::VERTEX_INPUT);
                vertexBufferDirty = false;
            }
        }
    }

    void Vector3DRenderer::render(
        vireo::CommandList& commandList,
        const Camera& camera,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const uint32 frameIndex) {
        if (vertexCount == 0) {
            return;
        }
        const auto& frame = framesData[frameIndex];

        if (useCamera) {
            const auto globalUbo = GlobalUniform {
                .projection = camera.projection,
                .view = inverse(camera.transform),
            };
            frame.globalUniform->write(&globalUbo, sizeof(GlobalUniform));
        }

        renderingConfig.colorRenderTargets[0].renderTarget = colorAttachment;
        renderingConfig.depthStencilRenderTarget = depthAttachment;

        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.bindVertexBuffer(vertexBuffer);
        commandList.beginRendering(renderingConfig);
        if (!linesVertices.empty()) {
            commandList.bindPipeline(pipelineLines);
            commandList.bindDescriptors({frame.descriptorSet, ctx.samplers.getDescriptorSet(), fontDescriptorSet});
            commandList.draw(linesVertices.size(), 1, 0, 0);
        }
        if (!triangleVertices.empty()) {
            commandList.bindPipeline(pipelineTriangles);
            commandList.bindDescriptors({frame.descriptorSet, ctx.samplers.getDescriptorSet(), fontDescriptorSet});
            commandList.draw(triangleVertices.size(), 1, linesVertices.size(), 0);
        }
        if (pipelineImages && !imagesVertices.empty()) {
            commandList.bindPipeline(pipelineImages);
            commandList.bindDescriptors({frame.descriptorSet, ctx.samplers.getDescriptorSet(), fontDescriptorSet});
            commandList.draw(
                imagesVertices.size(),
                1,
                linesVertices.size() + triangleVertices.size(),
                0);
        }
        if (!glyphVertices.empty()) {
            commandList.bindPipeline(pipelineGlyphs);
            commandList.bindDescriptors({frame.descriptorSet, ctx.samplers.getDescriptorSet(), fontDescriptorSet});
            commandList.draw(
                glyphVertices.size(),
                1,
                linesVertices.size() + triangleVertices.size() + imagesVertices.size(),
                0);
        }
        commandList.endRendering();
        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::UNDEFINED);
    }

    int32 Vector3DRenderer::addTexture(const Image &texture) {
        if (texturesIndices.contains(texture.id)) {
            return texturesIndices.at(texture.id);
        }
        for (int index = 0; index < textures.size(); index++) {
            if (textures[index] == blankImage) {
                textures[index] = texture.getImage();
                texturesIndices[texture.id] = index;
                for (const auto& frameData : framesData) {
                    frameData.descriptorSet->update(texturesIndex, textures);
                }
                return index;
            }
        }
        throw Exception("Maximum images count reached for the vector renderer");
    }

    int32 Vector3DRenderer::addFont(const Font &font) {
        if (fontsIndices.contains(font.id)) {
            return fontsIndices.at(font.id);
        }
        for (int index = 0; index < fontsParams.size(); index++) {
            if (all(fontsParams[index].pxRange == FLOAT2ZERO)) {
                fontsParams[index] = font.getFontParams();
                fontsIndices[font.id] = index;
                fontsParamsUniform->write(
                    &fontsParams[index],
                    sizeof(FontParams),
                    sizeof(FontParams) * index);
                return index;
            }
        }
        throw Exception("Maximum font count reached for the vector renderer");
    }

}