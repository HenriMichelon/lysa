/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.context;

namespace  lysa {

    Context::Context(
        const vireo::Backend backend,
        const size_t eventsCapacity,
        const size_t commandsCapacity,
        const size_t samplersCapacity,
        const VirtualFSConfiguration& virtualFsConfiguration
#ifdef LUA_BINDING
        ,const LuaConfiguration& luaConfiguration
#endif
        ) :
        vireo(vireo::Vireo::create(backend)),
        fs(virtualFsConfiguration, vireo),
        samplers(vireo, samplersCapacity),
#ifdef LUA_BINDING
        lua(luaConfiguration, fs),
#endif
        events(eventsCapacity),
        defer(commandsCapacity),
        graphicQueue(vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue")),
        transferQueue(vireo->createSubmitQueue(vireo::CommandType::TRANSFER, "Main transfer queue")),
        asyncQueue(vireo, transferQueue, graphicQueue) {
#ifdef LUA_BINDING
        lua.beginNamespace()
            .beginNamespace("ctx")
               .addProperty("exit", &exit)
               .addProperty("vireo", [this] { return &vireo;})
               .addProperty("fs",  [this] { return &fs;})
               .addProperty("events", [this] { return &events;})
#ifdef ECS_SCENES
               .addProperty("world", [this] { return &world;})
#endif
               .addProperty("res", [this] { return &res;})
               .addProperty("graphic_queue", [this] { return &graphicQueue;})
            .endNamespace()
        .endNamespace();
#endif
    }

}