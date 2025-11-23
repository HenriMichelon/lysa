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
     * @brief Wrapper around a single global %Lua state.
     *
     * This utility class exposes a global sol::state used by the engine to load and execute
     * Lua scripts, and to retrieve Lua functions. All members are static and the class is not
     * intended to be instantiated.
     *
     * @note The implementation relies on the Sol2 library (sol/sol.hpp).
     */
    class Lua {
    public:

        luabridge::Namespace beginNamespace() const;

        luabridge::LuaRef getGlobal(const std::string & name) const;

        /**
         * @brief Get the global Lua state.
         * @return Reference to the unique sol::state managed by this class.
         */
        //sol::state& get() { return lua; }

        /**
         * @brief Execute a Lua script file in the current state.
         * Convenience function that loads and immediately runs the specified script. Errors are
         * captured in the returned protected_function_result.
         *
         * @param filename Path to the Lua script file to execute.
         * @return sol::protected_function_result Execution result; check for validity and errors.
         */
        void execute(const std::string& filename) const;

        Lua();

        ~Lua();

    private:
        // The single Lua state instance used by the application.
        lua_State* L;
    };

}