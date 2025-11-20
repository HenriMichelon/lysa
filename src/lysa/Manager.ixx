/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.manager;

import std;

export namespace lysa {

    template<typename TypeId, typename ResourceType>
    class Manager {
    protected:
        Manager() = default;

        ResourceType& create();

        bool destroy(const ResourceType& resource);

        ResourceType& get(const TypeId id) { return resources.at(id); }

        const ResourceType& get(const TypeId id) const { return resources.at(id); }

    private:
        std::unordered_map<TypeId, ResourceType> resources;
        TypeId nextId = 1;
    };

    template<typename TypeId, typename ResourceType>
    ResourceType& Manager<TypeId, ResourceType>::create() {
        TypeId id = nextId++;
        auto& resource = resources[id];
        resource.id = id;
        return resource;
    }

    template<typename TypeId, typename ResourceType>
    bool Manager<TypeId, ResourceType>::destroy(const ResourceType& resource) {
        resources.erase(resource.id);
        return resources.empty();
    }

}