/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include "lua.h"
export module lysa.lua;

export import lua_bridge; // from Vireo

import lysa.types;
import lysa.virtual_fs;

export namespace lysa {

    /**
     * @brief Configuration options for Lua integration and tooling.
     */
    struct LuaConfiguration {
        /**
         * @brief Whether to start the Lua remote debugger on initialization.
         */
        bool startRemoteDebug{true};

        /**
         * @brief Comma-separated list of hostnames/IPs the debugger may connect to.
         *
         * Multiple hosts can be provided separated by commas to allow fallback, e.g.
         * "127.0.0.1,192.168.1.10".
         */
        std::string remoteDebugHosts{"127.0.0.1"};

        /**
         * @brief TCP port used by the remote debugger server.
         *
         * Defaults to 8172, which is the standard port used by ZeroBrane Studio
         */
        uint32 remoteDebugPort{8172};
    };

    /**
     * @brief Wrapper around a %Lua state.
     *
     * @note The implementation relies on the [LuaBridge3 library](https://kunitoki.github.io/LuaBridge3/Manual)
     */
    class Lua {
    public:
        /**
         * @brief Returns the LuaBridge3 "lysa" namespace
         */
        luabridge::Namespace beginNamespace() const;

        /**
         * @brief Returns a LuaBridge3 namespace
         */
        luabridge::Namespace beginNamespace(const std::string& name) const;

        /**
         * @brief Returns a global Lua object (function, variable, ...)
         */
        luabridge::LuaRef getGlobal(const std::string & name) const;

        /**
         * @brief Execute a Lua script file in the current state.
         *
         * @param scriptName name of the script file to execute, relative to the script directory of the VFS
         */
        void execute(const std::string& scriptName) const;

        Lua(const LuaConfiguration& luaConfiguration, const VirtualFS& virtualFs);

        ~Lua();

        lua_State* get() const { return L; }

    private:
        const VirtualFS &virtualFs;
        lua_State* L;

        void bind();
    };

}
