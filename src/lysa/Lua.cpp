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
}
module lysa.lua;

import std;
//import lua_bridge;

import lysa.exception;
import lysa.virtual_fs;

namespace lysa {

    Lua::Lua() {
        L = luaL_newstate();
        luaL_openlibs(L);
        luabridge::enableExceptions(L);
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

    void Lua::execute(const std::string& filename) const{
        std::vector<char> data;
        VirtualFS::loadBinaryData(filename, data);
        const auto script = std::string(data.begin(), data.end());
        if (script.empty()) {
            throw Exception("Lua error: failed to load script '", filename, "'");
        }

        if (luaL_dostring(L, script.c_str()) != LUA_OK) {
            const char* err = lua_tostring(L, -1);
            std::string msg = err ? err : "(unknown error)";
            lua_pop(L, 1);
            throw Exception("Lua error in '", filename, "': ", msg);
        }
    }

}