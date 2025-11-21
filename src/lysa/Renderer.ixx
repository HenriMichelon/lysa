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
        /** Run until quit() is requested. */
        void run(const std::function<void()>& onUpdate);

        /** Request the renderer to exit its main loop at the next opportunity. */
        bool quit() { return exit = true; }

    private:
        // Flag set to request exit from the main loop.
        bool exit{false};
        // Consume platform specific events
        void processPlaformEvents() const;
    };

}