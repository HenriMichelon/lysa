/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.locator;

namespace lysa {

    void ResourcesLocator::_init() {
        RenderingWindowManager::_init();
    }

    std::unordered_map<const char*, void*> ResourcesLocator::managers{};
}