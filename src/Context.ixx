/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.context;

import std;
import vireo;
import lysa.async_queue;
import lysa.async_pool;
import lysa.command_buffer;
import lysa.event;
import lysa.virtual_fs;
import lysa.types;
import lysa.resources.samplers;
import lysa.resources.registry;
import lysa.resources.samplers;

export namespace  lysa {

    struct ResourcesCapacity {
        //! Maximum number of images stored in CPU & GPU memory
        size_t images{500};
        //! Maximum number of GPU image samplers
        size_t samplers{20};
        //! Maximum number of standard & shader materials in CPU & GPU memory
        size_t material{100};
        //! Maximum number of meshes in CPU & GPU memory
        size_t meshes{1000};
        //! Maximum number of meshes surfaces in GPU memory
        size_t surfaces{meshes * 10};
        //! Maximum number of meshes vertices in GPU memory
        size_t vertices{surfaces * 1000};
        //! Maximum number of meshes indices in GPU memory
        size_t indices{vertices * 10};
    };

    /**
     * @brief Configuration object used to initialize a Lysa instance.
     */
    struct ContextConfiguration {
        //! Graphic API used by the graphic backend
        vireo::Backend backend{vireo::Backend::VULKAN};
        //! Fixed delta time for the main loop
        double deltaTime{1.0/60.0};
        //! Number of simultaneous frames during rendering for ALL render targets and scenes
        uint32 framesInFlight{2};
        //! Maximum number of shadow maps per scene
        uint32 maxShadowMapsPerScene{20};
        //! Enable shadowed colors for transparency objects
        // bool shadowTransparencyColorEnabled{true};
        //! Resource capacity configuration
        ResourcesCapacity resourcesCapacity;
        size_t eventsReserveCapacity{100};
        size_t commandsReserveCapacity{1000};
        //! Virtual file system configuration
        VirtualFSConfiguration virtualFsConfiguration;
    };

    /**
     * @brief Lysa instance-wide runtime context.
     */
    struct Context {
        /**
         * Quit flag controlling the main loop termination.
         *
         * When set to true, the main loop (see @ref Lysa::run) will exit
         * at the end of the current iteration.
         */
        bool exit{false};

        const ContextConfiguration config;

        /**
         * Backend object owning the device/instance and factory for GPU resources.
         */
        const std::shared_ptr<vireo::Vireo> vireo;

        /**
         * Read and write resources referenced by URI
         */
        const VirtualFS fs;

        /**
         * Central event dispatcher for the application.
         */
        EventManager events;

        /**
         * Deferred commands buffer
         */
        DeferredTasksBuffer defer;

        /**
         * Deferred commands buffer
         */
        AsyncTasksPool threads;

        /**
         * Resource resolution and access facility.
         */
        ResourcesRegistry res;

        /**
         * Global GPU samplers
         */
        Samplers samplers;

        /**
         * Submit queue used for graphics/rendering work.
         */
        const std::shared_ptr<vireo::SubmitQueue> graphicQueue;

        /**
         * Submit queue used for DMA transfers work.
         */
        const std::shared_ptr<vireo::SubmitQueue> transferQueue;

        /**
         * Asynchronous submissions of submit queues
         */
        AsyncQueue asyncQueue;

        std::shared_ptr<vireo::DescriptorLayout> globalDescriptorLayout;
        std::shared_ptr<vireo::DescriptorSet> globalDescriptorSet;

        Context(const ContextConfiguration& config);
    };

}