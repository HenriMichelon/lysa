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
import lysa.log;
import lysa.virtual_fs;

namespace lysa {

    Image::Image(const std::shared_ptr<vireo::Image>& image, const uint32 index, const std::string & name):
        image{image},
        index{index},
        name{name} {
    }

    ImageManager::ImageManager(Context& ctx, const unique_id capacity) :
        ResourcesManager(ctx, capacity),
        images(capacity) {
        ctx.resources.enroll(*this);
        auto blankJPEG = createBlankJPEG();
        std::vector<void*> cubeFaces(6);
        for (int i = 0; i < 6; i++) {
            cubeFaces[i]= blankJPEG.data();
        }
        const auto& vireo = *ctx.vireo;
        blankImage = vireo.createImage(
            vireo::ImageFormat::R8G8B8A8_SRGB,
            1, 1,1, 1,
            "Blank Image");
        blankCubeMap = vireo.createImage(
            vireo::ImageFormat::R8G8B8A8_SRGB,
            1, 1,1, 6,
            "Blank CubeMap");
        const auto commandAllocator = vireo.createCommandAllocator(vireo::CommandType::GRAPHIC);
        const auto commandList = commandAllocator->createCommandList();
        commandList->begin();
        commandList->barrier(
            blankImage,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::COPY_DST);
        commandList->upload(blankImage, blankJPEG.data());
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

        for (int i = 0; i < resources.size(); i++) {
            images[i] = blankImage;
        }
    }

    ImageManager::~ImageManager() {
        cleanup();
        images.clear();
    }

    // void ImageManager::update() {
    //     auto lock = std::lock_guard(mutex);
    //     if (textureUpdated) {
    //         ctx.graphicQueue->waitIdle();
    //         //descriptorSet->update(BINDING_TEXTURE, textures);
    //         textureUpdated = false;
    //     }
    // }

    Image& ImageManager::create(
        const void* data,
        const uint32 width, const uint32 height,
        const vireo::ImageFormat imageFormat,
        const std::string& name) {
        if (isFull()) throw Exception("ImageManager : no more free slots");

        auto lock = std::lock_guard(mutex);

        const auto& vireo = *ctx.vireo;
        const auto image = vireo.createImage(imageFormat, width, height, 1, 1, name);

        const auto commandAllocator = vireo.createCommandAllocator(vireo::CommandType::GRAPHIC);
        const auto commandList = commandAllocator->createCommandList();
        commandList->begin();
        commandList->barrier(image, vireo::ResourceState::UNDEFINED, vireo::ResourceState::COPY_DST);
        commandList->upload(image, data);
        commandList->barrier(image, vireo::ResourceState::COPY_DST, vireo::ResourceState::SHADER_READ);
        commandList->end();

        const auto& graphicQueue = ctx.graphicQueue;
        graphicQueue->submit({commandList});
        graphicQueue->waitIdle();

        uint32 index = 0;
        for (; index < images.size(); index++) {
            if (images[index] == blankImage) {
                images[index] = image;
                textureUpdated = true;
            }
        }

        return ResourcesManager::create(image, index, name);
    }

    Image& ImageManager::load(
        const std::string &filepath,
        const vireo::ImageFormat imageFormat) {
        uint32 texWidth, texHeight;
        uint64 imageSize;
        auto *pixels = ctx.fs.loadImage(filepath, texWidth, texHeight, imageSize);
        if (!pixels) { throw Exception("failed to load texture image ", filepath); }
        auto& image = create(pixels, texWidth, texHeight, imageFormat, filepath);
        ctx.fs.destroyImage(pixels);
        return image;
    }

    void ImageManager::save(const unique_id image_id, const std::string& filepath) {
        const auto image = get(image_id).getImage();
        const auto& vireo = *ctx.vireo;
        const auto& graphicQueue = ctx.graphicQueue;
        graphicQueue->waitIdle();

        const auto commandAllocator = vireo.createCommandAllocator(vireo::CommandType::GRAPHIC);
        const auto commandList = commandAllocator->createCommandList();
        commandList->begin();
        const auto buffer = vireo.createBuffer(vireo::BufferType::IMAGE_DOWNLOAD, image->getAlignedImageSize());
        commandList->copy(image, buffer);
        commandList->end();
        graphicQueue->submit({commandList});
        graphicQueue->waitIdle();

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
        }
    }

    void ImageManager::stb_write_func(void *context, void *data, const int size) {
        auto *buffer = static_cast<std::vector<uint8> *>(context);
        auto *ptr    = static_cast<uint8*>(data);
        buffer->insert(buffer->end(), ptr, ptr + size);
    }

    std::vector<uint8> ImageManager::createBlankJPEG() {
        std::vector<uint8> blankJPEG;
        const auto data = new uint8[1 * 1 * 3];
        data[0]   = static_cast<uint8>(0);
        data[1]   = static_cast<uint8>(0);
        data[2]   = static_cast<uint8>(0);
        stbi_write_jpg_to_func(stb_write_func, &blankJPEG, 1, 1, 3, data, 100);
        delete[] data;
        return blankJPEG;
    }

}
