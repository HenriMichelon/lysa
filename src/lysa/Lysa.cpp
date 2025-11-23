/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
module lysa;

namespace lysa {

    Lysa::Lysa(const LysaConfiguration& lysaConfiguration) {
        Log::_init(lysaConfiguration.loggingConfiguration);
        EventManager::_register(lua);
        lua.beginNamespace()
            .beginClass<Context>("Context")
                .addProperty("exit", &Context::exit)
                .addProperty("event_manager", &Context::eventManager)
                .addProperty("resources_locator", &Context::resourcesLocator)
            .endClass()
            .addProperty("ctx", &ctx)
        .endNamespace();
        ResourcesLocator::_register(lua);
    }

    void Lysa::run(
        const std::function<void()>& onProcess,
        const std::function<void()>& onQuit) {
        while (!ctx.exit) {
            onProcess();
            processPlatformEvents();
        }
        if (onQuit) onQuit();
        Log::_shutdown();

    }

}