/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include "lua.h"
export module lysa.lua;

import std;
export import lua_bridge;

export namespace lysa {

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
         * @brief Returns a global Lua object (function, variable, ...)
         */
        luabridge::LuaRef getGlobal(const std::string & name) const;

        /**
         * @brief Execute a Lua script file in the current state.
         *
         * @param filename URI to the Lua script file to execute.
         */
        void execute(const std::string& filename) const;

        Lua();

        ~Lua();

    private:
        // The single Lua state instance used by the application.
        lua_State* L;
    };

}