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
        static constexpr uint32 SET_GLOBAL{0};
        /** Descriptor binding index for the material buffer. */
        static constexpr vireo::DescriptorIndex BINDING_MATERIAL{0};
        /** Descriptor binding index for the mesh surfaces buffer. */
        static constexpr vireo::DescriptorIndex BINDING_SURFACES{1};
        /** Descriptor binding index for the textures array/sampled images. */
        static constexpr vireo::DescriptorIndex BINDING_TEXTURE{2};

        GlobalDescriptorSet(Context& ctx);

        ~GlobalDescriptorSet();

        /** Returns the descriptor set that exposes resources to shaders. */
        auto getDescriptorSet() const { return descriptorSet; }

        /** Returns the descriptor set layout */
        auto getDescriptorLayout() const { return descriptorLayout; }

        /**
        * Flushes pending uploads to the device. Typically, enqueues copy/transfer
        * commands to make new/updated resources visible to the GPU.
        */
        void flush();

        /** Applies incremental updates to buffers, images, and descriptors if needed. */
        void update();

        /** Marks the container as updated so dependent systems can react. */
        void needUpdates() { updated = true; }

        /** Returns true when an update has been flagged since the last processing. */
        bool isUpdateNeeded() const { return updated; }

    private:
        Context& ctx;
        ImageManager& imageManager;
        // Global descriptor set layout
        std::shared_ptr<vireo::DescriptorLayout> descriptorLayout;
        // Global Descriptor set bound at SET_GLOBAL
        std::shared_ptr<vireo::DescriptorSet> descriptorSet;
        /** Mutex to guard mutations to the descriptor set. */
        std::mutex mutex;
        /** Flag indicating the descriptor set has pending updates. */
        bool updated{false};
    };
}
