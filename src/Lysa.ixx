/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa;

export import std;
export import vireo;

export import lysa.aabb;
export import lysa.assets_pack;
export import lysa.async_queue;
export import lysa.context;
export import lysa.directory_watcher;
export import lysa.event;
export import lysa.exception;
export import lysa.utils;
export import lysa.input;
export import lysa.input_event;
export import lysa.log;
export import lysa.math;
export import lysa.types;
export import lysa.virtual_fs;

export import lysa.renderers.configuration;
export import lysa.renderers.forward_renderer;
export import lysa.renderers.global_descriptor_set;
export import lysa.renderers.graphic_pipeline_data;
export import lysa.renderers.renderer;
export import lysa.renderers.scene_frame_data;
export import lysa.renderers.pipelines.frustum_culling;
export import lysa.renderers.renderpasses.depth_prepass;
export import lysa.renderers.renderpasses.forward_color;
export import lysa.renderers.renderpasses.renderpass;

export import lysa.resources.camera;
export import lysa.resources.environment;
export import lysa.resources.image;
export import lysa.resources.manager;
export import lysa.resources.material;
export import lysa.resources.mesh;
export import lysa.resources.mesh_instance;
export import lysa.resources.registry;
export import lysa.resources.render_target;
export import lysa.resources.render_view;
export import lysa.resources.rendering_window;
export import lysa.resources.samplers;
export import lysa.resources.scene;
export import lysa.resources.texture;

#ifdef LUA_BINDING
export import lysa.lua;
#endif

export namespace  lysa {

    struct ResourcesCapacity {
        //! Maximum number of render targets
        size_t renderTarget{1};
        //! Maximum number of viewports
        size_t viewports{5};
        //! Maximum number of render views
        size_t renderViews{viewports};
        //! Maximum number of cameras
        size_t camera{viewports*2};
        //! Maximum number of scenes
        size_t scenes{viewports};
        //! Maximum environments
        size_t environments{scenes};
        //! Maximum number of images stored in GPU memory
        size_t images{500};
        //! Maximum number of GPU image samplers
        size_t samplers{20};
        //! Maximum number of standard & shader materials
        size_t material{100};
        //! Maximum number of meshes
        size_t meshes{1000};
        //! Maximum number of meshes instances
        size_t meshesInstances{meshes * 10};
        //! Maximum number of meshes surfaces in GPU memory
        size_t surfaces{meshes * 10};
        //! Maximum number of meshes vertices in GPU memory
        size_t vertices{surfaces * 10};
        //! Maximum number of meshes indices in GPU memory
        size_t indices{vertices * 2};
        //! Maximum number of shadow maps per scene
        size_t shadowMapsPerScene{20};
        //! Maximum number of lights per scene
        size_t lightsPerScene{100};
        //! Maximum number of mesh instances per frame per scene
        size_t meshInstancesPerScene{meshesInstances};
        //! Maximum number of mesh surfaces instances per pipeline
        size_t meshSurfacePerPipeline{surfaces};
    };

    /**
     * @brief Configuration object used to initialize a Lysa instance.
     */
    struct LysaConfiguration {
        //! Graphic API used by the graphic backend
        vireo::Backend backend{vireo::Backend::VULKAN};
        //! Fixed delta time for the main loop
        double deltaTime{1.0/60.0};
        //! Number of simultaneous frames during rendering
        uint32 framesInFlight{2};
        //! Number of nodes updates per frame for asynchronous scene updates
        uint32 asyncObjectUpdatesPerFrame{50};
        //! Resource capacity configuration
        ResourcesCapacity resourcesCapacity;
        size_t eventsReserveCapacity{100};
        size_t commandsReserveCapacity{1000};
        //! Virtual file system configuration
        VirtualFSConfiguration virtualFsConfiguration;
    };

    /**
    * Global events fired during the main loop
    */
    struct MainLoopEvent : Event {
        //! Fired multiple times per frame with the fixed delta time
        static inline const event_type PHYSICS_PROCESS{"MAIN_LOOP_PHYSICS_PROCESS"};
        //! Fired on time per frame after the physics with the remaining frame time
        static inline const event_type PROCESS{"MAIN_LOOP_PROCESS"};
        //! Fired just after the main loop exit, but before resources destruction's
        static inline const event_type QUIT{"MAIN_LOOP_QUIT"};
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
        /**
         * @brief Construct the runtime and initialize subsystems.
         * @param config Configuration values used during startup.
         */
        Lysa(const LysaConfiguration& config);

        ~Lysa();

        /**
         * @brief Run the main loop until quit is requested.
         */
        void run();

        /**
         * @brief Get the mutable application context.
         * @return Reference to the owned @ref Context.
         */
        Context& getContext() { return ctx; }

    private:
        // Global runtime context (events, resources, etc.).
        Context ctx;
        // Fixed delta time bookkeeping for the physics update loop
        const double fixedDeltaTime;
        double currentTime{0.0};
        double accumulator{0.0};

        CameraManager cameraManager;
        RenderTargetManager renderTargetManager;
        ImageManager imageManager;
        ImageTextureManager imageTextureManager;
        MaterialManager materialManager;
        MeshManager meshManager;
        MeshInstanceManager meshInstanceManager;
        SceneManager sceneManager;
        GlobalDescriptorSet globalDescriptors;
        RenderViewManager renderViewManager;
        EnvironmentManager environmentManager;

        // Consume platform-specific events.
        void processPlatformEvents();

        void uploadData();
    };

}