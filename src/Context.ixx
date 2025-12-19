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
import lysa.resources.locator;
import lysa.resources.samplers;

export namespace  lysa {

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
        CommandBuffer defer;

        /**
         * Deferred commands buffer
         */
        AsyncPool threads;

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

        Context(
            vireo::Backend backend,
            size_t eventsCapacity,
            size_t commandsCapacity,
            size_t samplersCapacity,
            const VirtualFSConfiguration& virtualFsConfiguration
            );
    };

}