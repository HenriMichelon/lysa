/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderer;

namespace lysa {

    void Renderer::run(const std::function<void()>& onUpdate) {
        while (!exit) {
            if (onUpdate) onUpdate();
            processPlaformEvents();
        }

    }


}