/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#ifndef LYSA_CONSOLE
#include <windows.h>

extern int lysaMain();

int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int ) {
    return lysaMain();
}

#endif
