/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.context;

import lysa.event;
import lysa.resources.locator;

export namespace  lysa {

    struct Context {
        // Flag set to request exit from the main loop.
        bool exit{false};
        EventManager eventManager;
        ResourcesLocator resourcesLocator;
    };

}