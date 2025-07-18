/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources;

import vireo;
import lysa.global;
import lysa.configuration;
import lysa.memory;
import lysa.samplers;
import lysa.resources.image;
import lysa.resources.mesh;

export namespace lysa {

    class Resources {
    public:
        static constexpr auto MAX_TEXTURES{500};

        static constexpr uint32 SET_RESOURCES{0};
        static constexpr vireo::DescriptorIndex BINDING_MATERIAL{0};
        static constexpr vireo::DescriptorIndex BINDING_SURFACES{1};
        static constexpr vireo::DescriptorIndex BINDING_TEXTURE{2};
        inline static std::shared_ptr<vireo::DescriptorLayout> descriptorLayout{nullptr};

        Resources(
            const vireo::Vireo& vireo,
            ResourcesConfiguration& config,
            const vireo::SubmitQueue& graphicQueue);

        auto& getVertexArray() { return vertexArray; }

        auto& getIndexArray() { return indexArray; }

        auto& getMaterialArray() { return materialArray; }

        auto& getMeshSurfaceArray() { return meshSurfaceArray; }

        auto& getSamplers() { return samplers; }

        uint32 addTexture(const Image& image);

        const auto& getDescriptorSet() const { return descriptorSet; }

        void flush();

        void update();

        void cleanup();

        void setUpdated() { updated = true; }

        bool isUpdated() const { return updated; }

        auto& getMutex() { return mutex; }

        auto getBlankImage() { return blankImage; }

        auto getBlankCubeMap() { return blankCubeMap; }

        Resources(Resources&) = delete;
        Resources& operator = (Resources&) = delete;

    private:
        const ResourcesConfiguration& config;
        DeviceMemoryArray vertexArray;
        DeviceMemoryArray indexArray;
        DeviceMemoryArray materialArray;
        DeviceMemoryArray meshSurfaceArray;
        Samplers samplers;
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        std::vector<std::shared_ptr<vireo::Image>> textures;
        std::shared_ptr<vireo::Image> blankImage;
        std::shared_ptr<vireo::Image> blankCubeMap;
        bool updated{false};
        bool textureUpdated{false};
        std::mutex mutex;

        static std::vector<uint8> createBlankJPEG();

        static void stb_write_func(void *context, void *data, int size);
    };

}