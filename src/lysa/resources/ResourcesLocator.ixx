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

export namespace lysa {

    /**
     * @brief Top level registry to locate resource managers at runtime.
     *
     */
    class ResourcesLocator {
    public:
        /**
         * @brief Retrieve a previously enrolled resources manager by name.
         *
         * Looks up the manager registered under the given name and returns it as type T.
         *
         * @tparam T The concrete manager type to retrieve (e.g., RenderingWindowManager).
         * @param name The key used during enrollment.
         * @return T& Reference to the located manager.
         * @throws Exception if no manager has been enrolled under the specified name.
         */
        template<typename T>
        T& get(const std::string& name) {
            if (managers.contains(name)) {
                return *(static_cast<T*>(managers[name]));
            }
            throw Exception("ResourcesLocator could not find manager " + name);
        }

        void* _getManager(const std::string& name) {
            if (managers.contains(name)) {
                return managers[name];
            }
            throw Exception("ResourcesLocator could not find manager " + name);
        }

        /**
         * @brief Enroll a resources manager instance under a given name.
         *
         * Registers the address of the provided ResourcesManager<T> so it can later be
         * retrieved via get<T>(name).
         *
         * @tparam T The type of resources handled by the manager.
         * @param name The key used to identify this manager (commonly a string literal).
         * @param manager Reference to the manager instance to register. Ownership is not taken;
         *                the caller is responsible for the manager's lifetime, which must exceed
         *                any subsequent lookups.
         */
        template<typename T>
        void enroll(const std::string& name, Manager<T>& manager) {
            managers[name] = &manager;
        }

    private:
        // Internal registry mapping names to manager instances.
        std::unordered_map<std::string, void*> managers{};
    };

}