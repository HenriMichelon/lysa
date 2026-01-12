/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.global_descriptor_set;

import vireo;

import lysa.context;
import lysa.types;
import lysa.resources.image;

export namespace lysa {

    /**
     * @brief Global descriptor set for GPU-ready shared resources.
     *
     * This class manages a descriptor set that provides access to global resources
     * such as materials, mesh surfaces, and textures, which are shared across multiple pipelines.
     */
    class GlobalDescriptorSet {
    public:
        /** @brief Descriptor set index used by pipelines to bind shared resources. */
        static constexpr uint32 SET{0};
        /** @brief Descriptor binding index for the materials buffer. */
        static constexpr vireo::DescriptorIndex BINDING_MATERIALS{0};
        /** @brief Descriptor binding index for the mesh surfaces buffer. */
        static constexpr vireo::DescriptorIndex BINDING_SURFACES{1};
        /** @brief Descriptor binding index for the textures array/sampled images. */
        static constexpr vireo::DescriptorIndex BINDING_TEXTURES{2};

        /**
         * @brief Constructs a new GlobalDescriptorSet.
         * @param ctx Reference to the rendering context.
         */
        GlobalDescriptorSet(Context& ctx);

        /**
         * @brief Destroys the GlobalDescriptorSet.
         */
        ~GlobalDescriptorSet();

        /**
         * @brief Returns the descriptor set that exposes resources to shaders.
         * @return The global descriptor set.
         */
        auto getDescriptorSet() const { return descriptorSet; }

        /**
         * @brief Returns the descriptor set layout.
         * @return The global descriptor set layout.
         */
        auto getDescriptorLayout() const { return descriptorLayout; }

        /**
         * @brief Updates the descriptor set if needed.
         */
        void update();

    private:
        /* Reference to the engine context. */
        Context& ctx;
        /*Reference to the image manager. */
        ImageManager& imageManager;
        /* Global descriptor set layout. */
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
        /* Global descriptor set bound at SET index. */
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /* Mutex to guard mutations to the descriptor set. */
        std::mutex mutex;
    };
}
