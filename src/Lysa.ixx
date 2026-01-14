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
export import lysa.blur_data;
export import lysa.context;
export import lysa.directory_watcher;
export import lysa.event;
export import lysa.exception;
export import lysa.utils;
export import lysa.input;
export import lysa.input_event;
export import lysa.log;
export import lysa.math;
export import lysa.rect;
export import lysa.types;
export import lysa.virtual_fs;

export import lysa.renderers.configuration;
export import lysa.renderers.deferred_renderer;
export import lysa.renderers.forward_renderer;
export import lysa.renderers.global_descriptor_set;
export import lysa.renderers.graphic_pipeline_data;
export import lysa.renderers.renderer;
export import lysa.renderers.scene_frame_data;
export import lysa.renderers.vector_2d;
export import lysa.renderers.vector_3d;
export import lysa.renderers.pipelines.frustum_culling;
export import lysa.renderers.renderpasses.bloom_pass;
export import lysa.renderers.renderpasses.depth_prepass;
export import lysa.renderers.renderpasses.forward_color_pass;
export import lysa.renderers.renderpasses.fxaa_pass;
export import lysa.renderers.renderpasses.gamma_correction_pass;
export import lysa.renderers.renderpasses.gbuffer_pass;
export import lysa.renderers.renderpasses.post_processing;
export import lysa.renderers.renderpasses.renderpass;
export import lysa.renderers.renderpasses.shader_material_pass;
export import lysa.renderers.renderpasses.shadow_map_pass;
export import lysa.renderers.renderpasses.smaa_pass;
export import lysa.renderers.renderpasses.ssao_pass;
export import lysa.renderers.renderpasses.transparency_pass;

export import lysa.resources.camera;
export import lysa.resources.environment;
export import lysa.resources.font;
export import lysa.resources.image;
export import lysa.resources.light;
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
        //! Global runtime context (events, resources, etc.).
        Context ctx;

        /**
         * @brief Construct the runtime and initialize subsystems.
         * @param config Configuration values used during startup.
         */
        Lysa(const ContextConfiguration& config = {});

        ~Lysa();

        /**
         * @brief Run the main loop until quit is requested.
         */
        void run();

    private:
        // Fixed delta time bookkeeping for the physics update loop
        const double fixedDeltaTime;
        double currentTime{0.0};
        double accumulator{0.0};

        ImageManager imageManager;
        MaterialManager materialManager;
        MeshManager meshManager;
        GlobalDescriptorSet globalDescriptors;

        // Consume platform-specific events.
        void processPlatformEvents();

        void uploadData();
    };

}