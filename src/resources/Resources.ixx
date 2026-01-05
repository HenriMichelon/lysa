/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources;

import lysa.types;

export namespace lysa {

    /**
     * @brief Base class for all resources in the engine
     * @details A resource is identified by a unique ID
     */
    struct Resource {
        /**
         * @brief Unique identifier for the resource
         */
        unique_id id{INVALID_ID};

        bool operator==(const Resource&other) const {
            return id == other.id;
        }

        Resource() = default;

        Resource(const unique_id id) : id(id) {}

        virtual ~Resource() = default;
    };

    /**
     * @brief Base class for resources not managed by a resource manager
     * @details Generates a unique ID automatically upon creation
     */
    class UnmanagedResource : public Resource {
    public:
        /**
         * @brief Default constructor, increments the static ID counter
         */
        UnmanagedResource() : Resource(nextId++) {}

        UnmanagedResource(const Resource& other) : Resource(other.id) {}

    private:
        /** @brief Static counter for generating unique IDs */
        static inline std::atomic<unique_id> nextId{1};
    };

    /**
     * @brief A non-managed resource that cannot be copied
     */
    class UniqueResource : public UnmanagedResource {
    public:
        UniqueResource() = default;

        UniqueResource(UniqueResource&) = delete;

        UniqueResource& operator = (UniqueResource&) = delete;
    };

    /**
     * @brief Base class for resources managed with reference counting and manager-assigned ID
     */
    struct ManagedResource : Resource {
        /** @brief Reference counter */
        uint32 refCounter{0};

        ManagedResource() = default;

        ManagedResource(UniqueResource&) = delete;

        ManagedResource& operator = (ManagedResource&) = delete;
    };

}
