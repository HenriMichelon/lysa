/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
module lysa.lua;

import std;
import lysa.exception;

namespace lysa {

    sol::state Lua::lua;

    void Lua::_init() {
        lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
    }

    sol::load_result Lua::load(const std::string& filename) {
        sol::load_result res = lua.load(loadScript(filename));
        if (!res.valid()) {
            const sol::error err = res;
            throw Exception("Lua error: ", err.what());
        }
        return res;
    }

    std::string Lua::loadScript(const std::string& path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) return {};

        const std::streamsize size = file.tellg();
        std::string buffer(size, '\0');

        file.seekg(0);
        file.read(buffer.data(), size);
        return buffer;
    }
}