/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.renderpass;

import lysa.virtual_fs;

namespace lysa {

    Renderpass::Renderpass(
        Context& ctx,
        const RendererConfiguration& config,
        const std::string& name):
        ctx{ctx},
        name{name},
        config{config} {
    }

    std::shared_ptr<vireo::ShaderModule> Renderpass::loadShader(const std::string& shaderName) const {
        auto tempBuffer = std::vector<char>{};
        ctx.virtualFs.loadShader(shaderName, tempBuffer);
        return ctx.vireo->createShaderModule(tempBuffer);
    }

    void Renderpass::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginClass<Renderpass>("Renderpass")
            .endClass()
        .endNamespace();
    }
}