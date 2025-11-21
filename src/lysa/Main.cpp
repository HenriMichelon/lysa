/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
module lysa.lysa;

import lysa.event;
import lysa.lua;
import lysa.resources.locator;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& lysaConfiguration) {
        Log::_init(lysaConfiguration.loggingConfiguration);
        Lua::_init();
        EventManager::_init();
        ResourcesLocator::_init();
        Lua::get()["lysa"] = std::ref(*this);
    }

    void Lysa::run(
        const std::function<void()>& onInit,
        const std::function<void()>& onProcess,
        const std::function<void()>& onShutdown) {
        onInit();
        while (!exit) {
            onProcess();
            processPlatformEvents();
        }
        if (onShutdown) onShutdown();
        EventManager::_shutdown();
        Log::_shutdown();

    }

}