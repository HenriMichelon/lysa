/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.manager;

import std;

import lysa.context;
import lysa.exception;
import lysa.manager;
import lysa.types;

export namespace lysa {

    class Resource {
    public:
        //! Unique ID
        unique_id id{INVALID_ID};

    protected:
        Context& ctx;

        Resource(Context& ctx) : ctx{ctx} {}
    };

    /**
     * @brief Generic, fixed-capacity resources manager using ID-based access.
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
    class ResourcesManager : public Manager<T> {
    protected:
        // Reference to the owning application context.
        Context& ctx;

        // Construct a manager with a fixed number of slots.
        ResourcesManager(Context& ctx, const std::string& ID, const unique_id capacity) :
            Manager<T>(capacity),
            ctx{ctx} {
            ctx.resourcesLocator.enroll(ID, *this);
        }

    };

}