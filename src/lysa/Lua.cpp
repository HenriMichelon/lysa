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

    Lua::Lua(const LuaConfiguration& luaConfiguration) {
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

    luabridge::Namespace Lua::beginNamespace(const std::string& name) const {
        return luabridge::getGlobalNamespace(L).beginNamespace (name.c_str());
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

    void LuaVireo::_register(const Lua& lua) {
        lua.beginNamespace("vireo")
            .beginNamespace("ImageFormat")
                .addProperty("R8_UNORM",               []{ return (int)vireo::ImageFormat::R8_UNORM; })
                .addProperty("R8_SNORM",               []{ return (int)vireo::ImageFormat::R8_SNORM; })
                .addProperty("R8_UINT",                []{ return (int)vireo::ImageFormat::R8_UINT; })
                .addProperty("R8_SINT",                []{ return (int)vireo::ImageFormat::R8_SINT; })

                .addProperty("R8G8_UNORM",             []{ return (int)vireo::ImageFormat::R8G8_UNORM; })
                .addProperty("R8G8_SNORM",             []{ return (int)vireo::ImageFormat::R8G8_SNORM; })
                .addProperty("R8G8_UINT",              []{ return (int)vireo::ImageFormat::R8G8_UINT; })
                .addProperty("R8G8_SINT",              []{ return (int)vireo::ImageFormat::R8G8_SINT; })

                .addProperty("R8G8B8A8_UNORM",         []{ return (int)vireo::ImageFormat::R8G8B8A8_UNORM; })
                .addProperty("R8G8B8A8_SNORM",         []{ return (int)vireo::ImageFormat::R8G8B8A8_SNORM; })
                .addProperty("R8G8B8A8_UINT",          []{ return (int)vireo::ImageFormat::R8G8B8A8_UINT; })
                .addProperty("R8G8B8A8_SINT",          []{ return (int)vireo::ImageFormat::R8G8B8A8_SINT; })
                .addProperty("R8G8B8A8_SRGB",          []{ return (int)vireo::ImageFormat::R8G8B8A8_SRGB; })

                .addProperty("B8G8R8A8_UNORM",         []{ return (int)vireo::ImageFormat::B8G8R8A8_UNORM; })
                .addProperty("B8G8R8A8_SRGB",          []{ return (int)vireo::ImageFormat::B8G8R8A8_SRGB; })
                .addProperty("B8G8R8X8_UNORM",         []{ return (int)vireo::ImageFormat::B8G8R8X8_UNORM; })
                .addProperty("B8G8R8X8_SRGB",          []{ return (int)vireo::ImageFormat::B8G8R8X8_SRGB; })

                .addProperty("A2B10G10R10_UNORM",      []{ return (int)vireo::ImageFormat::A2B10G10R10_UNORM; })
                .addProperty("A2B10G10R10_UINT",       []{ return (int)vireo::ImageFormat::A2B10G10R10_UINT; })

                .addProperty("R16_UNORM",              []{ return (int)vireo::ImageFormat::R16_UNORM; })
                .addProperty("R16_SNORM",              []{ return (int)vireo::ImageFormat::R16_SNORM; })
                .addProperty("R16_UINT",               []{ return (int)vireo::ImageFormat::R16_UINT; })
                .addProperty("R16_SINT",               []{ return (int)vireo::ImageFormat::R16_SINT; })
                .addProperty("R16_SFLOAT",             []{ return (int)vireo::ImageFormat::R16_SFLOAT; })

                .addProperty("R16G16_UNORM",           []{ return (int)vireo::ImageFormat::R16G16_UNORM; })
                .addProperty("R16G16_SNORM",           []{ return (int)vireo::ImageFormat::R16G16_SNORM; })
                .addProperty("R16G16_UINT",            []{ return (int)vireo::ImageFormat::R16G16_UINT; })
                .addProperty("R16G16_SINT",            []{ return (int)vireo::ImageFormat::R16G16_SINT; })
                .addProperty("R16G16_SFLOAT",          []{ return (int)vireo::ImageFormat::R16G16_SFLOAT; })

                .addProperty("R16G16B16A16_UNORM",     []{ return (int)vireo::ImageFormat::R16G16B16A16_UNORM; })
                .addProperty("R16G16B16A16_SNORM",     []{ return (int)vireo::ImageFormat::R16G16B16A16_SNORM; })
                .addProperty("R16G16B16A16_UINT",      []{ return (int)vireo::ImageFormat::R16G16B16A16_UINT; })
                .addProperty("R16G16B16A16_SINT",      []{ return (int)vireo::ImageFormat::R16G16B16A16_SINT; })
                .addProperty("R16G16B16A16_SFLOAT",    []{ return (int)vireo::ImageFormat::R16G16B16A16_SFLOAT; })

                .addProperty("R32_UINT",               []{ return (int)vireo::ImageFormat::R32_UINT; })
                .addProperty("R32_SINT",               []{ return (int)vireo::ImageFormat::R32_SINT; })
                .addProperty("R32_SFLOAT",             []{ return (int)vireo::ImageFormat::R32_SFLOAT; })

                .addProperty("R32G32_UINT",            []{ return (int)vireo::ImageFormat::R32G32_UINT; })
                .addProperty("R32G32_SINT",            []{ return (int)vireo::ImageFormat::R32G32_SINT; })
                .addProperty("R32G32_SFLOAT",          []{ return (int)vireo::ImageFormat::R32G32_SFLOAT; })

                .addProperty("R32G32B32_UINT",         []{ return (int)vireo::ImageFormat::R32G32B32_UINT; })
                .addProperty("R32G32B32_SINT",         []{ return (int)vireo::ImageFormat::R32G32B32_SINT; })
                .addProperty("R32G32B32_SFLOAT",       []{ return (int)vireo::ImageFormat::R32G32B32_SFLOAT; })

                .addProperty("R32G32B32A32_UINT",      []{ return (int)vireo::ImageFormat::R32G32B32A32_UINT; })
                .addProperty("R32G32B32A32_SINT",      []{ return (int)vireo::ImageFormat::R32G32B32A32_SINT; })
                .addProperty("R32G32B32A32_SFLOAT",    []{ return (int)vireo::ImageFormat::R32G32B32A32_SFLOAT; })

                .addProperty("D16_UNORM",              []{ return (int)vireo::ImageFormat::D16_UNORM; })
                .addProperty("D24_UNORM_S8_UINT",      []{ return (int)vireo::ImageFormat::D24_UNORM_S8_UINT; })
                .addProperty("D32_SFLOAT",             []{ return (int)vireo::ImageFormat::D32_SFLOAT; })
                .addProperty("D32_SFLOAT_S8_UINT",     []{ return (int)vireo::ImageFormat::D32_SFLOAT_S8_UINT; })

                .addProperty("BC1_UNORM",              []{ return (int)vireo::ImageFormat::BC1_UNORM; })
                .addProperty("BC1_UNORM_SRGB",         []{ return (int)vireo::ImageFormat::BC1_UNORM_SRGB; })
                .addProperty("BC2_UNORM",              []{ return (int)vireo::ImageFormat::BC2_UNORM; })
                .addProperty("BC2_UNORM_SRGB",         []{ return (int)vireo::ImageFormat::BC2_UNORM_SRGB; })
                .addProperty("BC3_UNORM",              []{ return (int)vireo::ImageFormat::BC3_UNORM; })
                .addProperty("BC3_UNORM_SRGB",         []{ return (int)vireo::ImageFormat::BC3_UNORM_SRGB; })
                .addProperty("BC4_UNORM",              []{ return (int)vireo::ImageFormat::BC4_UNORM; })
                .addProperty("BC4_SNORM",              []{ return (int)vireo::ImageFormat::BC4_SNORM; })
                .addProperty("BC5_UNORM",              []{ return (int)vireo::ImageFormat::BC5_UNORM; })
                .addProperty("BC5_SNORM",              []{ return (int)vireo::ImageFormat::BC5_SNORM; })
                .addProperty("BC6H_UFLOAT",            []{ return (int)vireo::ImageFormat::BC6H_UFLOAT; })
                .addProperty("BC6H_SFLOAT",            []{ return (int)vireo::ImageFormat::BC6H_SFLOAT; })
                .addProperty("BC7_UNORM",              []{ return (int)vireo::ImageFormat::BC7_UNORM; })
                .addProperty("BC7_UNORM_SRGB",         []{ return (int)vireo::ImageFormat::BC7_UNORM_SRGB; })
            .endNamespace()
            .beginNamespace("PresentMode")
                .addProperty("IMMEDIATE", []{ return (int)vireo::PresentMode::IMMEDIATE; })
                .addProperty("VSYNC",     []{ return (int)vireo::PresentMode::VSYNC; })
            .endNamespace()
        .endNamespace();
    }

}