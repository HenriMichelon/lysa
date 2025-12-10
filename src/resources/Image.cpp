/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <cstring>
#include <stb_image_write.h>
module lysa.resources.image;

import lysa.exception;

namespace lysa {

    Image::Image(const std::shared_ptr<vireo::Image>& image, const std::string & name):
        image{image},
        name{name} {
    }

    ImageManager::ImageManager(Context& ctx, const size_t capacity) :
        ResourcesManager(ctx, capacity),
        blankImage(ctx.vireo->createImage(
            vireo::ImageFormat::R8G8B8A8_SRGB,
            1, 1,1, 1,
            "Blank Image")),
        blankCubeMap(ctx.vireo->createImage(
            vireo::ImageFormat::R8G8B8A8_SRGB,
            1, 1,1, 6,
            "Blank CubeMap")),
        images(capacity, blankImage) {
        ctx.res.enroll(*this);
        auto blank = std::vector<uint8>(4, 0);
        std::vector<void*> cubeFaces(6);
        for (int i = 0; i < 6; i++) {
            cubeFaces[i]= blank.data();
        }
        ctx.graphicQueue->waitIdle();
        const auto commandAllocator = ctx.vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
        const auto commandList = commandAllocator->createCommandList();
        commandList->begin();
        commandList->barrier(
            blankImage,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::COPY_DST);
        commandList->upload(blankImage, blank.data());
        commandList->barrier(
            blankImage,
            vireo::ResourceState::COPY_DST,
            vireo::ResourceState::SHADER_READ);
        commandList->barrier(
            blankCubeMap,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::COPY_DST);
        commandList->uploadArray(blankCubeMap, cubeFaces);
        commandList->barrier(
            blankCubeMap,
            vireo::ResourceState::COPY_DST,
            vireo::ResourceState::SHADER_READ);
        commandList->end();
        ctx.graphicQueue->submit({commandList});
        ctx.graphicQueue->waitIdle();
    }

    ImageManager::~ImageManager() {
        cleanup();
        images.clear();
    }

    Image& ImageManager::create(
        const void* data,
        const uint32 width, const uint32 height,
        const vireo::ImageFormat imageFormat,
        const std::string& name) {
        if (isFull()) throw Exception("ImageManager : no more free slots");

        const auto image = ctx.vireo->createImage(imageFormat, width, height, 1, 1, name);
        {
            auto lock = std::lock_guard(mutex);
            ctx.graphicQueue->waitIdle();
            const auto commandAllocator = ctx.vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
            const auto commandList = commandAllocator->createCommandList();
            commandList->begin();
            commandList->barrier(image, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
            commandList->upload(image, data);
            commandList->barrier(image, vireo::ResourceState::COPY_DST, vireo::ResourceState::SHADER_READ);
            commandList->end();
            ctx.graphicQueue->submit({commandList});
            ctx.graphicQueue->waitIdle();
        }

        auto& result = ResourcesManager::create(image, name);
        result.index = result.id;
        images[result.index] = image;
        updated = true;
        return result;
    }

    Image& ImageManager::load(
        const std::string &filepath,
        const vireo::ImageFormat imageFormat) {
        uint32 texWidth, texHeight;
        uint64 imageSize;
        auto *pixels = ctx.fs.loadImage(filepath, texWidth, texHeight, imageSize);
        if (!pixels) { throw Exception("failed to load image ", filepath); }
        auto& image = create(pixels, texWidth, texHeight, imageFormat, filepath);
        ctx.fs.destroyImage(pixels);
        return image;
    }

    void ImageManager::save(const unique_id image_id, const std::string& filepath) {
        const auto image = get(image_id).getImage();
        const auto buffer = ctx.vireo->createBuffer(vireo::BufferType::IMAGE_DOWNLOAD, image->getAlignedImageSize());
        {
            auto lock = std::lock_guard(mutex);
            ctx.graphicQueue->waitIdle();
            const auto commandAllocator = ctx.vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
            const auto commandList = commandAllocator->createCommandList();
            commandList->begin();
            commandList->copy(image, buffer);
            commandList->end();
            ctx.graphicQueue->submit({commandList});
            ctx.graphicQueue->waitIdle();
        }
        buffer->map();
        const auto rowPitch = image->getRowPitch();
        const auto alignedRowPitch = image->getAlignedRowPitch();
        std::vector<uint8> imageData(image->getImageSize());
        const auto* source = static_cast<uint8*>(buffer->getMappedAddress());
        for (int y = 0; y < image->getHeight(); ++y) {
            memcpy(&imageData[y * rowPitch], &source[y * alignedRowPitch], rowPitch);
        }
        buffer->unmap();

        if (filepath.ends_with(".hdr")) {
            const auto floatImage = reinterpret_cast<const float*>(imageData.data());
            stbi_write_hdr(filepath.c_str(),
                image->getWidth(),
                image->getHeight(),
                1,
                floatImage);
        } else if (filepath.ends_with(".png")) {
            stbi_write_png(filepath.c_str(),
                image->getWidth(),
                image->getHeight(),
                image->getPixelSize(image->getFormat()),
                imageData.data(),
                rowPitch);
        } else {
            throw Exception("Image format not supported");
        }
    }

}
