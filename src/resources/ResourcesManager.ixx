/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.manager;

import lysa.exception;
import lysa.log;
import lysa.types;
import lysa.resources;

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
    template<typename CTX, typename T>
    class ResourcesManager {
    public:
        /**
       * @brief Create a new unique resource
       */
        template<typename... Args>
        T& create(Args&&... args) {
            return allocate(std::make_unique<T>(ctx, std::forward<Args>(args)...));
        }

        /**
         * @brief Bracket operator for mutable access without bounds checking.
         * @param id The unique identifier of the resource.
         * @return T& Reference to the resource.
         */
        inline T& operator[](const unique_id id) {
            assert([&]{ return have(id); }, name  + " : invalid id ");
            return *resources[id];
        }

        /**
         * @brief Bracket operator for const access without bounds checking.
         * @param id The unique identifier of the resource.
         * @return const T& Const reference to the resource.
         */
        inline const T& operator[](const unique_id id) const {
            assert([&]{ return have(id); }, name  + " : invalid id ");
            return *resources[id];
        }

        virtual ~ResourcesManager() {
            for (auto& res : resources) {
                if (res) destroy(res->id);
            }
            assert([&]{ return freeList.size() == resources.size(); },
                name  + " : resources still in use");
        }

        ResourcesManager(ResourcesManager&) = delete;
        ResourcesManager& operator=(ResourcesManager&) = delete;

        // Release a resource, returning its slot to the free list.
        virtual bool destroy(const unique_id id) {
            assert([&]{ return have(id); }, name  + " : invalid id ");
            resources[id]->refCounter -= 1;
            if (resources[id]->refCounter == 0) {
                resources[id].reset();
                freeList.push_back(id);
                return true;
            }
            return false;
        }

        virtual bool destroy(const T& res) {
            return destroy(res.id);
        }

        // Increment the reference counter of the resources
        void use(const unique_id id) {
            assert([&]{ return have(id); }, name  + " : invalid id ");
            resources[id]->refCounter += 1;
        }

        bool have(const unique_id id) const { return id < resources.size() && resources.at(id) != nullptr; }

        unique_id getCapacity() const { return resources.size(); }

        constexpr CTX& getContext() const { return ctx; }

    protected:
        friend class Lysa;
        // Reference to the owning application context.
        CTX& ctx;

        // Construct a manager with a fixed number of slots.
        ResourcesManager(CTX& ctx, const size_t capacity, const std::string& name) :
            ctx(ctx),
            resources(capacity),
            name(name) {
            for (auto id = capacity; id > 0; --id) {
                freeList.push_back(id-1);
            }
        }

        // Allocate a new resource
        T& allocate(std::unique_ptr<T> instance) {
            assert([&]{ return !isFull(); }, name  + " : no more free slots");
            auto id = freeList.back();
            freeList.pop_back();
            resources[id] = std::move(instance);
            resources[id]->id = id;
            return *(resources[id]);
        }

        // Contiguous storage for all resources managed by this instance.
        std::vector<std::unique_ptr<T>> resources;

        bool isFull() const {
            return freeList.empty();
        }

    private:
        // Name for debug & error messages
        const std::string name;
        // Stack-like list of free slot IDs available for future creations.
        std::vector<unique_id> freeList{};
    };

}