/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.image;

import vireo;

import lysa.context;
import lysa.math;
import lysa.types;
import lysa.resources.resource_manager;

export namespace lysa {

    /**
     * A bitmap resource, stored in GPU memory.
     */
    class Image : public Resource {
    public:
        /**
         * Returns the width in pixels
         */
        uint32 getWidth() const { return image->getWidth(); }

        /**
         * Returns the height in pixels
         */
        uint32 getHeight() const { return image->getHeight(); }

        /**
         * Returns the size in pixels
         */
        float2 getSize() const { return float2{getWidth(), getHeight()}; }

        auto getImage() const { return image; }

        uint32 getIndex() const { return index; }

        const auto& getName() const { return name; }

        Image(const std::shared_ptr<vireo::Image>& image, const std::string & name);

        ~Image() override = default;

    private:
        std::shared_ptr<vireo::Image> image;
        // Index in GPU memory
        uint32 index{0};
        std::string name;

        friend class ImageManager;
    };

    class ImageManager : public ResourcesManager<Image> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        ImageManager(Context& ctx, unique_id capacity);

        ~ImageManager() override;

        //void update();

        void save(unique_id image_id, const std::string& filepath);

        /**
        * Load a bitmap from a file.<br>
        * Supports JPEG and PNG formats
        */
        Image& load(
            const std::string &filepath,
            vireo::ImageFormat imageFormat = vireo::ImageFormat::R8G8B8A8_SRGB);

        /**
         * Load a bitmap from memory.<br>
         * Supports JPEG & PNG formats.
         */
        Image& create(
            const void* data,
            uint32 width, uint32 height,
            vireo::ImageFormat imageFormat = vireo::ImageFormat::R8G8B8A8_SRGB,
            const std::string& name = "Image");


        /** Returns the default 2D blank image used as a safe fallback. */
        auto getBlankImage() { return blankImage; }

        /** Returns the default cubemap blank image used as a safe fallback. */
        auto getBlankCubeMap() { return blankCubeMap; }

        bool isUpdateNeeded() const { return updated; }

    private:
        /** Flag indicating that one or more textures changed and need syncing. */
        bool updated{false};
        /** Default 2D image used when a texture is missing. */
        std::shared_ptr<vireo::Image> blankImage;
        /** Default cubemap image used when a cubemap is missing. */
        std::shared_ptr<vireo::Image> blankCubeMap;
        /** Mutex to guard mutations to images resources */
        std::mutex mutex;
        /** List of GPU images managed by this container. */
        std::vector<std::shared_ptr<vireo::Image>> images;
    };

}

