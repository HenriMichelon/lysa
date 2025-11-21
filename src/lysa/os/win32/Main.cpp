/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <windows.h>
module lysa.lysa;

import lysa.event;

namespace lysa {

    void Lysa::processPlatformEvents() const {
        auto msg = MSG{};
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        EventManager::_process();
    }
}


#ifndef LYSA_CONSOLE

extern int lysaMain();

int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int ) {
    return lysaMain();
}

#endif

