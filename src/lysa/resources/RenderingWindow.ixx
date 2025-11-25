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
import lysa.resources.locator;
import lysa.resources.manager;
import lysa.types;

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
    struct RenderingWindow {
        //! Unique ID
        unique_id id{INVALID_ID};
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
        //! Resource locator object used by global OS functions
        ResourcesLocator *locator{nullptr};
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
        unique_id create(const RenderingWindowConfiguration& configuration);

        void destroy(RenderingWindow& renderingWindow) override;

        /**
         * @brief Make a previously created window visible on screen.
         * @param id Unique identifier of the window to show.
         */
        void show(unique_id id) const;

        void close(unique_id id) const;

        void _closing(unique_id id);

        void _resized(unique_id id) const;

    private:
        friend class ResourcesLocator;
        static void _register(const Lua& lua);
    };

}

template <>
struct luabridge::Stack<lysa::RenderingWindowMode> : Enum<lysa::RenderingWindowMode> {};
