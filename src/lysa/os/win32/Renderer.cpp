/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderer;
#include <windows.h>

namespace lysa {

    void Renderer::processPlatformEvents() const {
        auto msg = MSG{};
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        EventManager::_process();
    }


}