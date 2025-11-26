/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#ifdef _WIN32
#include <windows.h>
#endif
export module lysa.resources.rendering_window;

import vireo;
import lua_bridge;

import lysa.context;
import lysa.event;
import lysa.lua;
import lysa.types;
import lysa.resources.manager;

export namespace lysa {

    /**
    * Rendering Window mode
    */
    enum class RenderingWindowMode : uint32 {
        //! A Window with a border and a title that can be minimized
        WINDOWED = 0,
        //! A maximized Window with a border and a title that can be minimized
        WINDOWED_MAXIMIZED = 1,
        //! A maximized Window without a border and without a title
        WINDOWED_FULLSCREEN = 2,
        //! A full-screen Window. The screen resolution will be changed
        FULLSCREEN = 3,
    };

    /**
    * Rendering window events data
    */
    struct RenderingWindowEvent : Event {
        //! The window is ready to be shown
        static constexpr auto READY{"RENDERING_WINDOW_READY"};
        //! The window is about to close
        static constexpr auto CLOSING{"RENDERING_WINDOW_CLOSING"};
        //! The window has been resized
        static constexpr auto RESIZED{"RENDERING_WINDOW_RESIZED"};
    };

    /**
    * Rendering window configuration
    */
    struct RenderingWindowConfiguration {
        //! Window title bar
        std::string title{"Lysa Window"};
        //! State of the display Window
        RenderingWindowMode mode{RenderingWindowMode::WINDOWED};
        //! Start up X position (top-left corner)
        int32 x{-1};
        //! Start up Y position (top-left corner)
        int32 y{-1};
        //! Width in pixels of the display Window
        uint32 width{1280};
        //! Height in pixels of the display Window
        uint32 height{720};
        //! Monitor index to display the Window
        int32 monitor{0};
    };

    /**
    * Operating system window that serve as rendering surface.
    */
    class RenderingWindow {
    public:
        //! Unique ID
        unique_id id{INVALID_ID};

        RenderingWindow(Context& ctx, const RenderingWindowConfiguration& config);

        /**
        * @brief Make a previously created window visible on screen.
        * @param id Unique identifier of the window to show.
        */
        void show() const;

        void close() const;

        int32 getX() const { return x; }

        int32 getY() const { return y; }

        int32 getWidth() const { return width; }

        int32 getHeight() const { return height; }

        bool isStopped() const { return stopped; }

        void* getPlatformHandle() const { return platformHandle; }

        void _closing();

        void _resized() const;

        void _setStopped(const bool state) { stopped = state; }

    private:
        Context& ctx;
        //! Top-Left corner x position in pixels
        int32 x{0};
        //! Top-Left corner Y position in pixels
        int32 y{0};
        //! Width in pixels
        uint32 width{0};
        //! Height in pixels
        uint32 height{0};
        //! True once the window has been requested to pause rendering or close.
        bool stopped{false};
        //! Opaque OS window handle used for presentation.
        void* platformHandle{nullptr};
    };

    /**
     * @brief Manager for creating and controlling @ref RenderingWindow resources.
     */
    class RenderingWindowManager : public ResourcesManager<RenderingWindow> {
    public:
        static constexpr auto ID = "RenderingWindow";

        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Reference to the application @ref Context (used for events and access to the @ref ResourcesLocator).
         * @param capacity Optional initial capacity for window resources.
         */
        RenderingWindowManager(Context& ctx, unique_id capacity = 5);

        /**
         * @brief Create a new rendering window resource.
         * @param configuration Window creation parameters (title, size, mode, position, monitor).
         * @return The unique @ref unique_id of the newly created window.
         */
        RenderingWindow& create(const RenderingWindowConfiguration& configuration);

        static void _register(const Lua& lua);
    };

}

template <>
struct luabridge::Stack<lysa::RenderingWindowMode> : Enum<lysa::RenderingWindowMode> {};
