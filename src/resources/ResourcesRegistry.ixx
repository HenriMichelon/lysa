/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.locator;

import std;
import lysa.exception;
import lysa.manager;

export namespace lysa {

    /**
     * @brief Top level registry to locate resource managers at runtime.
     *
     */
    class ResourcesRegistry {
    public:
        /**
         * @brief Retrieve a previously enrolled resources manager by type.
         *
         * Looks up the manager registered under the given name and returns it as type T.
         *
         * @tparam T The concrete manager type to retrieve (e.g., RenderingWindowManager).
         * @return T& Reference to the located manager.
         * @throws Exception if no manager has been enrolled under the specified type.
         */
        template<typename T>
        T& get() const {
            using t = std::remove_cv_t<std::remove_reference_t<T>>;
            const auto key = std::type_index(typeid(t));
            if (managers.contains(key)) {
                return *(static_cast<T*>(managers.at(key)));
            }
            throw Exception("ResourcesLocator could not find manager");
        }

        /**
         * @brief Enroll a resources manager instance under a given type.
         *
         * Registers the address of the provided ResourcesManager<T> so it can later be
         * retrieved via get<T>().
         *
         * @tparam T The type of resources handled by the manager.
         * @param manager Reference to the manager instance to register. Ownership is not taken;
         *                the caller is responsible for the manager's lifetime, which must exceed
         *                any subsequent lookups.
         */
        template<typename T>
        void enroll(T& manager) {
            using t = std::remove_cv_t<std::remove_reference_t<T>>;
            const auto key = std::type_index(typeid(t));
            managers[key] = &manager;
        }

    private:
        // Internal registry mapping types to manager instances.
        std::unordered_map<std::type_index, void*> managers{};
    };

}