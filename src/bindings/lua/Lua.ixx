/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}
export module lysa.lua;

export import lua_bridge; // from Vireo

import lysa.context;
import lysa.exception;
import lysa.types;

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
         * @param ctx
         * @param scriptName name of the script file to execute, relative to the script directory of the VFS
         */
        template <typename... Args>
        luabridge::LuaRef execute(const std::string& scriptName, Args&&... args) const {
            std::vector<char> data;
            ctx.fs.loadScript(scriptName, data);
            const auto script = std::string(data.begin(), data.end());
            if (script.empty()) {
                throw Exception("Lua error: failed to load script");
            }
            if (luaL_dostring(L, script.c_str()) != LUA_OK) {
                const char* err = lua_tostring(L, -1);
                std::string msg = err ? err : "(unknown error)";
                lua_pop(L, 1);
                throw Exception("Lua error : ", msg);
            }
            const auto factory = luabridge::LuaRef::fromStack(L, -1);
            const auto result = factory(ctx, std::forward<Args>(args)...);
            if (result.wasOk() && result[0].isTable()) {
                return result[0];
            }
            throw Exception("Error executing the Lua script " + scriptName + " or incorrect return type");
        }

        Lua(Context& ctx, const LuaConfiguration& luaConfiguration);

        ~Lua();

        lua_State* get() const { return L; }

    private:
        Context& ctx;
        lua_State* L;

        void bind();
    };

}
