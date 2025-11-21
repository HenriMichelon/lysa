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

    class Lua {
    public:
        static sol::state& get() { return lua; }

        static sol::protected_function getFunction(const std::string& name);

        static sol::load_result load(const std::string& filename);

        static sol::protected_function_result execute(const std::string& filename);

        static void _init();

    private:
        static sol::state lua;
        static std::string loadScript(const std::string& path);
    };

}