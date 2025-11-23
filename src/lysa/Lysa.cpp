/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <sol/sol.hpp>
module lysa;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& lysaConfiguration) {
        Log::_init(lysaConfiguration.loggingConfiguration);
        ctx.eventManager._register(lua);
        ResourcesLocator::_register(lua);
        lua.get().new_usertype<Context>("Context",
           "exit", &Context::exit,
           "event_manager", &Context::eventManager,
           "resources_locator", &Context::resourcesLocator
        );
        lua.get()["ctx"] = std::ref(ctx);
    }

    void Lysa::run(
        const std::function<void()>& onInit,
        const std::function<void()>& onProcess,
        const std::function<void()>& onShutdown) {
        onInit();
        while (!ctx.exit) {
            onProcess();
            processPlatformEvents();
        }
        if (onShutdown) onShutdown();
        Log::_shutdown();

    }

}