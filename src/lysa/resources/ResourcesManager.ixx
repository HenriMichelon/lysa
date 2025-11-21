/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.manager;

import std;
import lysa.exception;
import lysa.types;

export namespace lysa {

    template<typename T>
    class ResourcesManager {
    public:
        virtual ~ResourcesManager() = default;

        inline T& get(const unique_id id) { return resources.at(id); }

        inline const T& get(const unique_id id) const { return resources.at(id); }

        inline T& operator[](const unique_id id) { return resources[id]; }

        inline const T& operator[](const unique_id id) const { return resources[id]; }

    protected:
        ResourcesManager(const unique_id capacity) : resources(capacity) {
            for (auto i = 0; i < capacity; ++i) {
                freeList.push_back(i);
            }
        }

        virtual T& create() {
            if (freeList.empty()) throw Exception("ResourcesManager : no more free slots");
            auto id = freeList.back();
            freeList.pop_back();
            resources[id] = T{};
            resources[id].id = id;
            return resources[id];
        }

        virtual bool destroy(const unique_id id) {
            if (id >= resources.size()) return false;
            freeList.push_back(id);
            return true;
        }

    private:
        std::vector<T> resources;
        std::vector<unique_id> freeList;
    };

}