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
export import lysa.flecs;
export import lysa.global;
export import lysa.log;
export import lysa.manager;
export import lysa.math;
export import lysa.types;
export import lysa.virtual_fs;

export import lysa.renderers.configuration;
export import lysa.renderers.renderer;
export import lysa.renderers.renderpasses.renderpass;

export import lysa.resources.image;
export import lysa.resources.locator;
export import lysa.resources.render_target;
export import lysa.resources.rendering_window;
export import lysa.resources.resource_manager;
export import lysa.resources.viewport;

#ifdef LUA_BINDING
export import lysa.lua;
#endif

export namespace  lysa {

    struct ResourcesCapacity {
        //! Maximum number of rendering windows
        unique_id renderingWindow{1};
        //! Maximum number of render targets
        unique_id renderTarget{1};
        //! Maximum number of viewports
        unique_id viewports{5};
        //! Maximum images stored in GPU memory
        unique_id images{500};
    };

    /**
     * @brief Configuration object used to initialize a Lysa instance.
     */
    struct LysaConfiguration {
        //! Graphic API used by the graphic backend
        vireo::Backend backend{vireo::Backend::VULKAN};
        //! Resource capacity configuration
        ResourcesCapacity resourcesCapacity;
        //! Virtual file system configuration
        VirtualFSConfiguration virtualFsConfiguration;
#ifdef LUA_BINDING
        //! Configuration for Lua integration and tooling.
        LuaConfiguration luaConfiguration;
#endif
    };

    /**
     * @brief Main entry class of the Lysa runtime.
     *
     * This class owns the application @ref Context and the embedded @ref Lua
     * scripting environment. It provides the run loop and basic integration
     * points for both C++ and %Lua code.
     */
    class Lysa final {
    public:
        /** Fixed time step used by the physics update loop (in seconds). */
        static constexpr float FIXED_DELTA_TIME{1.0f/60.0f};

        /**
         * @brief Construct the runtime and initialize subsystems.
         * @param config Configuration values used during startup.
         */
        Lysa(const LysaConfiguration& config);

        ~Lysa();

        /**
         * @brief Run the main loop until quit is requested.
         *
         * @param onProcess Callback invoked every iteration (i.e. each frame).
         * @param onPhysicsProcess Callback invoked every physics update
         * @param onQuit Optional callback invoked once after the loop exits, before the instance shutdown
         */
        void run(
            const std::function<void(float)>& onProcess,
            const std::function<void(float)>& onPhysicsProcess = {},
            const std::function<void()>& onQuit = {});

#ifdef LUA_BINDING
        /**
         * @brief Run the main loop until quit is requested.
         *
         * @param onProcess Callback invoked every iteration (i.e. each frame).
         * @param onPhysicsProcess Callback invoked every physics update
         * @param onQuit Optional callback invoked once after the loop exits, before the instance shutdown
         */
        void run(
            const luabridge::LuaRef& onProcess,
            const luabridge::LuaRef& onPhysicsProcess = nullptr,
            const luabridge::LuaRef& onQuit = nullptr);
#endif

        /**
         * @brief Get the mutable application context.
         * @return Reference to the owned @ref Context.
         */
        Context& getContext() { return ctx; }

    private:
        // Global runtime context (events, resources, etc.).
        Context ctx;
        // Fixed delta time bookkeeping for the physics update loop
        double currentTime{0.0};
        double accumulator{0.0};

        ViewportManager viewportManager;
        RenderTargetManager renderTargetManager;
        RenderingWindowManager renderingWindowManager;
        ImageManager imageManager;

        // Consume platform-specific events.
        void processPlatformEvents();
    };

}