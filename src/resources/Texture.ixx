/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.texture;

import vireo;

import lysa.context;
import lysa.math;
import lysa.resources.image;
import lysa.resources.manager;

export namespace lysa {

    /**
     * Base class for textures resources.
     */
    class Texture : public ManagedResource {
    public:
        /**
         * Returns the width in pixels on the texture
         */
        virtual uint32 getWidth() const = 0;

        /**
         * Returns the height in pixels on the texture
         */
        virtual uint32 getHeight() const = 0;

        /**
         * Returns the size in pixels on the texture
         */
        virtual float2 getSize() const { return float2{getWidth(), getHeight()}; }

        /**
         * Returns the texture name
         */
        virtual const std::string& getName() const = 0;

    protected:
        Texture() = default;
    };

    /**
     * Image-based texture stored in GPU memory
     */
    class ImageTexture : public Texture {
    public:

        /**
         * Returns the attached Image
         */
        const auto& getImage() const { return image; }

        /**
         * Returns the index of the attached sampler in the global sampler GPU array
         */
        uint32 getSamplerIndex() const { return samplerIndex; }

        uint32 getWidth() const override { return image.getWidth(); }

        uint32 getHeight() const override { return image.getHeight(); }

        const std::string& getName() const override { return image.getName(); }

        ImageTexture(Context& ctx, const Image& image, const uint32 samplerIndex) :
            ctx(ctx),
            image(image),
            samplerIndex(samplerIndex) {
            ctx.res.get<ImageManager>().use(image.id);
        }

        ~ImageTexture() override {
            ctx.res.get<ImageManager>().destroy(image.id);
        }

    protected:
        Context& ctx;
        const Image& image;
        uint32 samplerIndex{0};
    };

    class ImageTextureManager : public ResourcesManager<Context, ImageTexture> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        ImageTextureManager(Context& ctx, const unique_id capacity) :
            ResourcesManager(ctx, capacity, "ImageTextureManager") {
            ctx.res.enroll(*this);
        }
    };

}

