/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.locator;

import lysa.resources.rendering_window;

namespace lysa {

    void ResourcesLocator::_register(Lua& lua) {
        RenderingWindowManager::_register(lua);
    }

}