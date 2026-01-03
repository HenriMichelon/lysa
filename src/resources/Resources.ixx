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

        Resource(const unique_id id) : id(id) {}
        Resource() = default;
        Resource(Resource&) = delete;
        Resource& operator = (Resource&) = delete;
        virtual ~Resource() = default;
    };

    class UniqueResource : public Resource {
    public:
        UniqueResource() : Resource(nextId++) {}

    private:
        static inline std::atomic<unique_id> nextId{1};
    };

    struct ManagedResource : Resource {
        //! References counter
        uint32 refCounter{0};
    };

}