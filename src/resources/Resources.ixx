/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources;

import lysa.types;

export namespace lysa {

    struct Resource {
        //! Unique ID
        unique_id id{INVALID_ID};

        bool operator==(const Resource&other) const {
            return id == other.id;
        }

        Resource() = default;
        Resource(const unique_id id) : id(id) {}
        virtual ~Resource() = default;
    };

    class UnmanagedResource : public Resource {
    public:
        UnmanagedResource() : Resource(nextId++) {}

    private:
        static inline std::atomic<unique_id> nextId{1};
    };

    class UniqueResource : public UnmanagedResource {
    public:
        UniqueResource() = default;
        UniqueResource(UniqueResource&) = delete;
        UniqueResource& operator = (UniqueResource&) = delete;
    };

    struct ManagedResource : Resource {
        uint32 refCounter{0};

        ManagedResource() = default;
        ManagedResource(UniqueResource&) = delete;
        ManagedResource& operator = (ManagedResource&) = delete;
    };

}