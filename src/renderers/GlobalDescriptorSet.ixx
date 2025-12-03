/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.global_descriptor_set;

import vireo;

import lysa.context;
import lysa.memory;
import lysa.types;
import lysa.resources.image;

export namespace lysa {

    /**
    * Global descriptor set for GPU-ready shared resources.
    **/
    class GlobalDescriptorSet {
    public:
        /** Descriptor set index used by pipelines to bind shared resources. */
        static constexpr uint32 SET{0};
        /** Descriptor binding index for the materials buffer. */
        static constexpr vireo::DescriptorIndex BINDING_MATERIALS{0};
        /** Descriptor binding index for the mesh surfaces buffer. */
        static constexpr vireo::DescriptorIndex BINDING_SURFACES{1};
        /** Descriptor binding index for the textures array/sampled images. */
        static constexpr vireo::DescriptorIndex BINDING_TEXTURES{2};

        GlobalDescriptorSet(Context& ctx);

        ~GlobalDescriptorSet();

        /** Returns the descriptor set that exposes resources to shaders. */
        auto getDescriptorSet() const { return descriptorSet; }

        /** Returns the descriptor set layout */
        auto getDescriptorLayout() const { return descriptorLayout; }

        /** Update descriptor set if needed. */
        void update();

    private:
        Context& ctx;
        ImageManager& imageManager;
        // Global descriptor set layout
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
        // Global Descriptor set bound at SET_GLOBAL
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** Mutex to guard mutations to the descriptor set. */
        std::mutex mutex;
    };
}
