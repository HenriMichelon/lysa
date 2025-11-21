/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
module lysa.renderer;

import lysa.lua;
import lysa.resources.locator;

namespace lysa {

    Renderer::Renderer() {
        Lua::_init();
        EventManager::_init();
        ResourcesLocator::_init();

        Lua::get()["renderer"] = std::ref(*this);
    }

    void Renderer::run(const std::function<void()>& onProcess) {
        while (!exit) {
            if (onProcess) onProcess();
            processPlatformEvents();
        }
        EventManager::_shutdown();
    }

}