/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.locator;

import std;
import lysa.exception;
import lysa.lua;
import lysa.resources.manager;

export namespace lysa {

    /**
     * @brief Global registry to locate resource managers at runtime.
     *
     * This utility provides a very small, static service locator that maps a textual name
     * (C-string) to a concrete ResourcesManager instance. It allows code to retrieve a
     * previously enrolled resource manager by name and type, without carrying explicit
     * references throughout the codebase.
     *
     * The locator stores raw, type-erased pointers internally and performs a static cast when
     * retrieving values, so the caller must request the correct type T that was used at
     * enrollment time.
     *
     * @warning This is a global, mutable registry.
     */
    class ResourcesLocator {
    public:
        /**
         * @brief Retrieve a previously enrolled resources manager by name.
         *
         * Looks up the manager registered under the given name and returns it as type T.
         *
         * @tparam T The concrete manager type to retrieve (e.g., RenderingWindowManager).
         * @param name The C-string key used during enrollment.
         * @return T& Reference to the located manager.
         * @throws Exception if no manager has been enrolled under the specified name.
         *
         * @note The lookup key is compared as a C-string pointer in the underlying container.
         * Use the exact same pointer value passed during enrollment (typically a string literal)
         * to avoid mismatches.
         */
        template<typename T>
        T& get(const char* name) {
            if (managers.contains(name)) {
                return *(static_cast<T*>(managers[name]));
            }
            throw Exception("ResourcesLocator could not find manager " + std::string(name));
        }

        /**
         * @brief Enroll a resources manager instance under a given name.
         *
         * Registers the address of the provided ResourcesManager<T> so it can later be
         * retrieved via get<T>(name).
         *
         * @tparam T The type of resources handled by the manager.
         * @param name The C-string key used to identify this manager (commonly a string literal).
         * @param manager Reference to the manager instance to register. Ownership is not taken;
         *                the caller is responsible for the manager's lifetime, which must exceed
         *                any subsequent lookups.
         */
        template<typename T>
        void enroll(const char* name, ResourcesManager<T>& manager) {
            managers[name] = &manager;
        }

        static void _register(Lua& lua);

    private:
        // Internal registry mapping names to manager instances.
        std::unordered_map<const char*, void*> managers{};
    };

}