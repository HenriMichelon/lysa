/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
module lysa.virtual_fs;

import lysa.exception;

namespace lysa {

    std::ifstream VirtualFS::openReadStream(const std::string &filepath) const {
        std::ifstream file(getPath(filepath), std::ios::ate | std::ios::binary);
        if (!file.is_open()) { throw Exception("Error: Could not open file ",  filepath); }
        return file;
    }

    std::ofstream VirtualFS::openWriteStream(const std::string &filepath) const {
        std::ofstream file(getPath(filepath), std::ios::binary);
        if (!file.is_open()) { throw Exception("Error: Could not open file ",  filepath); }
        return file;
    }

    bool VirtualFS::fileExists(const std::string &filepath) const {
        return std::filesystem::exists(getPath(filepath));
    }

    std::string VirtualFS::parentPath(const std::string& filepath) const {
        const auto lastSlash = filepath.find_last_of("/");
        if (lastSlash == std::string::npos) return "";
        return filepath.substr(0, lastSlash+1);
    }

    std::string VirtualFS::getPath(const std::string& filepath) const {
        std::string filename;
        if (filepath.starts_with(APP_URI)) {
            filename = config.appDirectory.string();
        } else {
            throw Exception("Unknown URI ", filepath);
        }
        const auto filePart = filepath.substr(std::string{APP_URI}.size());
        return (filename + "/" + filePart);
    }

    void VirtualFS::loadBinaryData(const std::string &filepath, std::vector<char>& out) const {
        std::ifstream file(getPath(filepath), std::ios::ate | std::ios::binary);
        if (!file.is_open()) { throw Exception("failed to open file : ", filepath); }
        loadBinaryData(file, out);
    }

    void VirtualFS::loadBinaryData(std::ifstream& input, std::vector<char>& out) const {
        const size_t fileSize = input.tellg();
        out.clear();
        out.resize(fileSize);
        input.seekg(0);
        input.read(out.data(), fileSize);
        input.close();
    }

    struct StreamWrapper {
        std::ifstream* stream;
    };

    int readCallback(void* user, char* data, const int size) {
        const auto* wrapper = static_cast<StreamWrapper*>(user);
        wrapper->stream->read(data, size);
        return static_cast<int>(wrapper->stream->gcount());
    }

    void skipCallback(void* user, const int n) {
        const auto* wrapper = static_cast<StreamWrapper*>(user);
        wrapper->stream->seekg(n, std::ios::cur);
    }

    int eofCallback(void* user) {
        const auto* wrapper = static_cast<StreamWrapper*>(user);
        return wrapper->stream->eof() ? 1 : 0;
    }

    constexpr auto callbacks = stbi_io_callbacks{
        .read = readCallback,
        .skip = skipCallback,
        .eof  = eofCallback,
    };

    std::byte* VirtualFS::loadImage(
        const std::string& filepath,
        uint32& width, uint32& height, uint64& size) const {
        std::ifstream file(getPath(filepath), std::ios::binary);
        if (!file.is_open()) {
            throw Exception ("Error: Could not open file ", filepath);
        }

        StreamWrapper wrapper{&file};
        int texWidth, texHeight, texChannels;
        unsigned char* imageData = stbi_load_from_callbacks(
            &callbacks, &wrapper,
            &texWidth, &texHeight,
            &texChannels, STBI_rgb_alpha);
        if (!imageData) {
            throw Exception("Error loading image : ", std::string{stbi_failure_reason()});
        }
        width = static_cast<uint32>(texWidth);
        height = static_cast<uint32>(texHeight);
        size = width * height * STBI_rgb_alpha;
        return reinterpret_cast<std::byte*>(imageData);
    }

    void VirtualFS::destroyImage(std::byte* image) const {
        stbi_image_free(image);
    }

    void VirtualFS::loadScript(const std::string& scriptName, std::vector<char>& out) const {
        std::ifstream file(
            getPath(APP_URI + config.scriptsDir + "/" + scriptName + ".lua"),
            std::ios::ate | std::ios::binary);
        if (!file.is_open()) { throw Exception("failed to open Lua script '", scriptName, "'"); }
        loadBinaryData(file, out);
    }

    void VirtualFS::loadShader(const std::string& shaderName, std::vector<char>& out) const {
        std::ifstream file(
            getPath(APP_URI + config.shadersDir + "/" + shaderName + vireo->getShaderFileExtension()),
            std::ios::ate | std::ios::binary);
        if (!file.is_open()) { throw Exception("failed to open compiled shader'", shaderName, "'"); }
        loadBinaryData(file, out);
    }

}
