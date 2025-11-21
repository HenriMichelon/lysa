/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.locator;

import std;
import lysa.exception;
import lysa.resources.manager;
import lysa.resources.rendering_window;

export namespace lysa {

    class ResourcesLocator {
    public:
        template<typename T>
        static T& get(const char* name) {
            if (managers.contains(name)) {
                return *(static_cast<T*>(managers[name]));
            }
            throw Exception("ResourcesLocator could not find manager " + std::string(name));
        }

        template<typename T>
        static void enroll(const char* name, ResourcesManager<T>& manager) {
            managers[name] = &manager;
        }

    private:
        static inline std::unordered_map<const char*, void*> managers{};
    };

}