/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.context;

namespace  lysa {

    Context::Context(
        const std::shared_ptr<vireo::Vireo>& vireo,
        const LuaConfiguration& luaConfiguration,
        const VirtualFSConfiguration& virtualFsConfiguration) :
        vireo(vireo),
        virtualFs(virtualFsConfiguration, *vireo),
        lua(luaConfiguration, virtualFs),
        graphicQueue(vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue")) {
    }

}