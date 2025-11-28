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
        const VirtualFSConfiguration& virtualFsConfiguration
#ifdef LUA_BINDINGS
        ,const LuaConfiguration& luaConfiguration
#endif
        ) :
        vireo(vireo::Vireo::create(backend)),
        virtualFs(virtualFsConfiguration, vireo),
#ifdef LUA_BINDINGS
        lua(luaConfiguration, virtualFs),
#endif
        graphicQueue(vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue")) {
#ifdef LUA_BINDINGS
        lua.beginNamespace().addVariable("ctx", this).endNamespace();
#endif
    }

}