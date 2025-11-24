/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.resources.locator;

import lysa.resources.rendering_window;
import lysa.resources.render_target;

namespace lysa {

    void ResourcesLocator::_register(const Lua& lua) {
        lua.beginNamespace()
            .beginClass<ResourcesLocator>("ResourcesLocator")
                .addFunction("get", &ResourcesLocator::getManager)
                .addFunction("getRenderTargetManager", +[](ResourcesLocator* rl) {
                        return rl->get<RenderTargetManager>(RenderTargetManager::ID);
                    })
                .addFunction("getRenderingWindowManager", +[](ResourcesLocator* rl) {
                    return rl->get<RenderingWindowManager>(RenderingWindowManager::ID);
                })
            .endClass()
        .endNamespace();
        RenderTargetManager::_register(lua);
        RenderingWindowManager::_register(lua);
    }

}