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

    /**
     * @brief Configuration object used to initialize a Lysa instance.
     */
    struct LysaConfiguration {
        /**
         * @brief Logging subsystem configuration applied at startup.
         * When creating multiple Lysa instances, only the first instance configure the Logging subsystem.
         */
        LoggingConfiguration loggingConfiguration;
    };

    /**
     * @brief Main entry class of the Lysa runtime.
     *
     * This class owns the application @ref Context and the embedded @ref Lua
     * scripting environment. It provides the run loop and basic integration
     * points for both C++ and Lua code.
     */
    class Lysa {
    public:
        /**
         * @brief Construct the runtime and initialize subsystems.
         * @param lysaConfiguration Configuration values used during startup.
         */
        Lysa(const LysaConfiguration& lysaConfiguration);

        /**
         * @brief Run the main loop until quit is requested.
         *
         * @param onProcess Callback invoked every iteration (i.e. each frame).
         * @param onQuit Optional callback invoked once after the loop exits, before the instance shutdown
         */
        void run(
            const std::function<void()>& onProcess,
            const std::function<void()>& onQuit = {});
        /**
         * @brief Run the main loop using Lua callbacks.
         *
         * @param onProcess Lua function called every iteration.
         * @param onQuit Optional Lua function called after the loop exits, before the instance shutdown
         */
        void run(
            const luabridge::LuaRef& onProcess,
            const luabridge::LuaRef& onQuit = nullptr) {
            run([&]{onProcess();}, [&]{if (onQuit) onQuit();});
        }

        /**
         * @brief Get the mutable application context.
         * @return Reference to the owned @ref Context.
         */
        Context& getContext() { return ctx; }

        /**
         * @brief Get the Lua interface.
         * @return Reference to the owned @ref Lua instance.
         */
        const Lua& getLua() const { return lua; }

    private:
        // Embedded Lua environment shared across the application.
        Lua lua;
        // Global runtime context (events, resources, etc.).
        Context ctx;

        // Consume platform-specific events.
        void processPlatformEvents();
    };

}