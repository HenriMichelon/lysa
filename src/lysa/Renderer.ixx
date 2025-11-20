/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderer;

import std;
import vireo;

export namespace lysa {

    class Renderer {
    public:
        Renderer(const std::function<void()>& onUpdate)  : onUpdate(onUpdate) {}

        /** Run until quit() is requested. */
        void run();

        /** Request the renderer to exit its main loop at the next opportunity. */
        bool quit() { return exit = true; }

    private:
        // Application specific update hook called before updating sub systems and drawing a frame
        std::function<void()> onUpdate{};
        // Flag set to request exit from the main loop.
        bool exit{false};
        // Consume platform specific events
        void processPlaformEvents() const;
    };

}