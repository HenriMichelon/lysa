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
         * Returns the size in number of pixels
         */
        float2 getSize() const { return float2{getWidth(), getHeight()}; }

        /**
         * Returns the GPU image
         */
        auto getImage() const { return image; }

        /**
         * Returns the index of the image in the global GPU memory array of images
         */
        uint32 getIndex() const { return index; }

        /**
         * Returns the name of the image
         */
        const std::string& getName() const { return name; }

        Image(const std::shared_ptr<vireo::Image>& image, const std::string & name);
        ~Image() override = default;

    private:
        // GPU Image
        std::shared_ptr<vireo::Image> image;
        // Index in GPU memory
        uint32 index{0};
        // File or image name
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
        ImageManager(Context& ctx, size_t capacity);

        ~ImageManager() override;

        //void update();

        /**
         * Save an image into a file.<br>
         * Supports PNG and HDR formats.
         * @param image_id Image
         * @param filepath Destination file URI
         */
        void save(unique_id image_id, const std::string& filepath);

        /**
        * Load a bitmap from a file.<br>
        * Supports JPEG and PNG formats
        * @param filepath Source file URI
        * @param imageFormat Image pixel format
        */
        Image& load(
            const std::string &filepath,
            vireo::ImageFormat imageFormat = vireo::ImageFormat::R8G8B8A8_SRGB);

        /**
         * Creates a bitmap from an array in memory
         * @param data Pixels array
         * @param width Width in pixels
         * @param height Height in pixels
         * @param imageFormat Pixel format
         * @param name Optional name
         */
        Image& create(
            const std::shared_ptr<vireo::Image>& image,
            const std::string& name = "Image");
        /**
         * Creates a bitmap from an array in memory
         * @param data Pixels array
         * @param width Width in pixels
         * @param height Height in pixels
         * @param imageFormat Pixel format
         * @param name Optional name
         */
        Image& create(
            const void* data,
            uint32 width, uint32 height,
            vireo::ImageFormat imageFormat = vireo::ImageFormat::R8G8B8A8_SRGB,
            const std::string& name = "Image");


        /** Returns the default 2D blank image used as a safe fallback. */
        auto getBlankImage() const { return blankImage; }

        /** Returns the default cubemap blank image used as a safe fallback. */
        auto getBlankCubeMap() const { return blankCubeMap; }

        /** Returns true if the descriptor sets using the images must be updated */
        bool _isUpdateNeeded() const { return updated; }

        void _resetUpdateFlag() { updated = false; }

        /** Return the global GPU image array */
        auto getImages() const { return images; }

        void destroy(unique_id id) override;

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

