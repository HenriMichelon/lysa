/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpass;

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
        const auto& ext = ctx.vireo->getShaderFileExtension();
        VirtualFS::loadBinaryData(
            ctx,
            "app://" +
            ctx.shaderDir +
            "/" +
            shaderName + ext,
            tempBuffer);
        return ctx.vireo->createShaderModule(tempBuffer);
    }
}