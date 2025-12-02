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
#ifdef LUA_BINDING
        ,const LuaConfiguration& luaConfiguration
#endif
        ) :
        vireo(vireo::Vireo::create(backend)),
        fs(virtualFsConfiguration, vireo),
#ifdef LUA_BINDING
        lua(luaConfiguration, fs),
#endif
        graphicQueue(vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue")) {
#ifdef LUA_BINDING
        lua.beginNamespace().addVariable("ctx", this).endNamespace();
#endif
    }

}