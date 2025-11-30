/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.context;

import std;
import vireo;
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
         * @brief Quit flag controlling the main loop termination.
         *
         * When set to true, the main loop (see @ref Lysa::run) will exit
         * at the end of the current iteration.
         */
        bool exit{false};

        /**
         * @brief Backend object owning the device/instance and factory for GPU resources.
         */
        const std::shared_ptr<vireo::Vireo> vireo;

        /**
         * Read and write resources referenced by URI
         */
        const VirtualFS virtualFs;

#ifdef LUA_BINDING
        /**
         * @brief Embedded Lua execution environment.
         */
        const Lua lua;
#endif

        /**
         * @brief Central event dispatcher for the application.
         */
        EventManager eventManager;

        flecs::world world;

        /**
         * @brief Resource resolution and access facility.
         */
        ResourcesLocator resourcesLocator;

        /**
         * @brief Submit queue used for graphics/rendering work.
         */
        const std::shared_ptr<vireo::SubmitQueue> graphicQueue;

        Context(
            vireo::Backend backend,
            const VirtualFSConfiguration& virtualFsConfiguration
#ifdef LUA_BINDING
            ,const LuaConfiguration& luaConfiguration
#endif
            );
    };

}