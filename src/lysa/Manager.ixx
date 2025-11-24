/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.manager;

import std;

import lysa.exception;
import lysa.types;

export namespace lysa {

    /**
     * @brief Generic object/resources manager using ID-based access.
     *
     * Manages a contiguous pool (vector) of resources of type T addressed by a small integral
     * unique identifier (unique_id). Instances are created into available slots and retrieved
     * via their ID. Destruction returns the slot to a free list for reuse.
     *
     * The manager does not perform dynamic growth; capacity is fixed at construction. All
     * accessors are O(1) by direct indexing. This class is intended to be used as a base for
     * concrete resource managers (e.g., textures, windows).
     *
     * @tparam T Resource type stored by the manager. T is expected to contain a public field
     *           named 'id' of type unique_id that will be assigned upon creation.
     */
    template<typename T>
    class Manager {
    public:
        /**
         * @brief Retrieve a mutable reference to the resource with the given ID.
         * @param id The unique identifier of the resource.
         * @return T& Reference to the resource.
         * @throws std::out_of_range if id is invalid (as thrown by std::vector::at).
         */
        inline T& get(const unique_id id) { return resources.at(id); }

        /**
         * @brief Alias for get(id): retrieve a mutable reference by unique ID.
         * @param id The unique identifier of the resource.
         * @return T& Reference to the resource.
         * @throws std::out_of_range if id is invalid (as thrown by std::vector::at).
         */
        inline T& getById(const unique_id id) { return resources.at(id); }

        /**
         * @brief Retrieve a read-only reference to the resource with the given ID.
         * @param id The unique identifier of the resource.
         * @return const T& Const reference to the resource.
         * @throws std::out_of_range if id is invalid (as thrown by std::vector::at).
         */
        inline const T& get(const unique_id id) const { return resources.at(id); }

        /**
         * @brief Bracket operator for mutable access without bounds checking.
         * @param id The unique identifier of the resource.
         * @return T& Reference to the resource.
         * @warning No bounds checking is performed (uses vector::operator[]). Prefer get() when
         *          you need range checking.
         */
        inline T& operator[](const unique_id id) { return resources[id]; }

        /**
         * @brief Bracket operator for const access without bounds checking.
         * @param id The unique identifier of the resource.
         * @return const T& Const reference to the resource.
         * @warning No bounds checking is performed (uses vector::operator[]). Prefer const get()
         *          when you need range checking.
         */
        inline const T& operator[](const unique_id id) const { return resources[id]; }

        virtual ~Manager() {
            for (T& resource : resources) {
                Manager::destroy(resource.id);
            }
        }

    protected:
        // Construct a manager with a fixed number of slots.
        Manager(const unique_id capacity) : resources(capacity) {
            for (auto i = 0; i < capacity; ++i) {
                freeList.push_back(i);
            }
        }

        //
        virtual T& create() {
            if (freeList.empty()) throw Exception("ResourcesManager : no more free slots");
            auto id = freeList.back();
            freeList.pop_back();
            resources[id] = T{};
            resources[id].id = id;
            return resources[id];
        }

        // Destroy (release) a resource, returning its slot to the free list.
        virtual bool destroy(const unique_id id) {
            if (id >= resources.size()) return false;
            freeList.push_back(id);
            return true;
        }

    private:
        // Contiguous storage for all resources managed by this instance.
        std::vector<T> resources;
        // Stack-like list of free slot IDs available for future creations.
        std::vector<unique_id> freeList;
    };

}