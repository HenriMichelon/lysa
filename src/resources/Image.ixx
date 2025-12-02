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
        Image(const std::shared_ptr<vireo::Image>& image, const std::string & name);

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

        ~Image() override = default;

    protected:
        std::string name;
        std::shared_ptr<vireo::Image> image;
        uint32 index;
    };

    class ImageManager : public ResourcesManager<Image> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        ImageManager(Context& ctx, unique_id capacity);

        ~ImageManager() override {
            cleanup();
        }

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

    private:


    };

}

