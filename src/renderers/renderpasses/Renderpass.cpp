/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.renderpass;

namespace lysa {

    std::mutex Renderpass::shaderModulesMutex;
    std::unordered_map<std::string, std::shared_ptr<vireo::ShaderModule>> Renderpass::shaderModules;

    Renderpass::Renderpass(
        const Context& ctx,
        const RendererConfiguration& config,
        const std::string& name):
        ctx{ctx},
        name{name},
        config{config} {
    }

    std::shared_ptr<vireo::ShaderModule> Renderpass::loadShader(const std::string& shaderName) const {
        auto lock = std::lock_guard(shaderModulesMutex);
        if (!shaderModules.contains(shaderName)) {
            auto tempBuffer = std::vector<char>{};
            ctx.fs.loadShader(shaderName, tempBuffer);
            shaderModules[shaderName] = ctx.vireo->createShaderModule(tempBuffer, shaderName);
        }
        return shaderModules[shaderName];
    }

}