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
import lysa.event;
import lysa.flecs;
#ifdef LUA_BINDING
import lysa.lua;
#endif
import lysa.virtual_fs;
import lysa.resources.locator;

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

#ifdef LUA_BINDING
        /**
         * Embedded Lua execution environment.
         */
        const Lua lua;
#endif

        /**
         * Central event dispatcher for the application.
         */
        EventManager events;

        /**
         * ECS world
         */
        flecs::world world;

        /**
         * Resource resolution and access facility.
         */
        ResourcesRegistry resources;

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

        Context(
            vireo::Backend backend,
            const VirtualFSConfiguration& virtualFsConfiguration
#ifdef LUA_BINDING
            ,const LuaConfiguration& luaConfiguration
#endif
            );
    };

}