/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
export module lysa.lua;

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
        /**
         * @brief Get the global Lua state.
         * @return Reference to the unique sol::state managed by this class.
         */
        static sol::state& get() { return lua; }

        /**
         * @brief Retrieve a Lua function by its global name.
         * Looks up a global symbol in the Lua state and returns it as a protected function. The
         * caller should check the returned function for validity before invoking it.
         *
         * @param name Global function name to retrieve (e.g., "update").
         * @return sol::protected_function The function handle; may be invalid (nil) if not found.
         */
        static sol::protected_function getFunction(const std::string& name);

        /**
         * @brief Load (but do not execute) a Lua script file.
         * The script is read from disk and compiled by Lua. The returned object can later be
         * executed by the caller.
         *
         * @param filename Path to the Lua script file.
         * @return sol::load_result The load/compile result; check for validity and errors before use.
         */
        static sol::load_result load(const std::string& filename);

        /**
         * @brief Execute a Lua script file in the current state.
         * Convenience function that loads and immediately runs the specified script. Errors are
         * captured in the returned protected_function_result.
         *
         * @param filename Path to the Lua script file to execute.
         * @return sol::protected_function_result Execution result; check for validity and errors.
         */
        static sol::protected_function_result execute(const std::string& filename);

        static void _init();

    private:
        // The single Lua state instance used by the application.
        static sol::state lua;

        // Load a script file content from disk.
        static std::string loadScript(const std::string& path);
    };

}