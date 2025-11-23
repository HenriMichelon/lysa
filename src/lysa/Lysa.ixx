/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa;

export import std;
export import vireo;

export import lysa.context;
export import lysa.event;
export import lysa.exception;
export import lysa.global;
export import lysa.log;
export import lysa.lua;
export import lysa.math;
export import lysa.types;

export import lysa.resources.locator;
export import lysa.resources.manager;
export import lysa.resources.rendering_window;


export namespace  lysa {

    struct LysaConfiguration {
        LoggingConfiguration loggingConfiguration;
    };

    class Lysa {
    public:
        Lysa(const LysaConfiguration& lysaConfiguration);

        /** Run until quit() is requested. */
        void run(
            const std::function<void()>& onInit,
            const std::function<void()>& onProcess,
            const std::function<void()>& onShutdown = {});

        Context& getContext() { return ctx; }

        Lua& getLua() { return lua; }

    private:
        Lua lua;
        Context ctx;

        // Consume platform-specific events
        void processPlatformEvents();
    };

}