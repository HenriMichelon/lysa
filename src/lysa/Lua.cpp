/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
extern "C"
{
    #define LUA_LIB
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    int luaopen_socket_core(lua_State* L);
}
module lysa.lua;

import std;

import lysa.exception;
import lysa.log;
import lysa.virtual_fs;

namespace lysa {

    Lua::Lua(const LuaConfiguration& luaConfiguration, const VirtualFS& virtualFs) : virtualFs(virtualFs) {
        L = luaL_newstate();
        luaL_openlibs(L);
        luaL_requiref(L, "socket", luaopen_socket_core, 1);
        lua_pop(L, 1);
        luaL_requiref(L, "socket.core", luaopen_socket_core, 1);
        lua_pop(L, 1);
        luabridge::enableExceptions(L);

        if (luaConfiguration.startRemoteDebug) {
             std::string mobdebug_chunk = R"(
package.path = package.path .. ";./scripts/?.lua;./?.lua"
local ok, mobdebug = pcall(require, 'mobdebug')
if not ok then
    error('mobdebug not found: ' .. tostring(mobdebug))
end
)";
            mobdebug_chunk +=
                "mobdebug.start([[" + luaConfiguration.remoteDebugHosts + "]], " +
                std::to_string(luaConfiguration.remoteDebugPort) + ")\n";
            if (luaL_dostring(L, mobdebug_chunk.c_str()) != LUA_OK) {
                Log::warning("[Lua/mobdebug] error: ", std::string(lua_tostring(L, -1)));
                lua_pop(L, 1);
            }
        }
    }

    Lua::~Lua() {
        lua_close(L);
    }

    luabridge::Namespace Lua::beginNamespace() const {
        return luabridge::getGlobalNamespace(L).beginNamespace ("lysa");
    }

    luabridge::LuaRef Lua::getGlobal(const std::string & name) const {
        return luabridge::getGlobal(L, name.c_str());
    }

    void Lua::execute(const std::string& scriptName) const{
        std::vector<char> data;
        virtualFs.loadScript(scriptName, data);
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
    }
}