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
import lysa.manager;
import lysa.types;

export namespace lysa {

    using RenderingWindowId = unique_id;

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
    * Rendering window configuration
    */
    struct RenderingWindowConfiguration {
        //! Window title bar
        std::string title{};
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
    * Rendering window events type
    */
    enum class RenderingWindowEventType : uint32 {
        //! Called once the window is fully created and ready to display.
        READY,
        //! Called when the window is about to close (release resources here).
        CLOSE,
        //! Called after the window/swap chain has been resized.
        RESIZE,
    };

    /**
    * Rendering window events data
    */
    struct RenderingWindowEvent {
        RenderingWindowId id;
        RenderingWindowEventType type;
    };

    class RenderingWindowManager;

    /**
    * Operating system window that serve as rendering surface.
    */
    struct RenderingWindow {
        /*! Unique ID  */
        RenderingWindowId id{INVALID_ID};
        /*! Top-Left corner x position in pixels*/
        int32 x{0};
        /*! Top-Left corner Y position in pixels*/
        int32 y{0};
        /*! Width in pixels */
        uint32 width{0};
        /*! Height in pixels */
        uint32 height{0};
        /*! True once the window has been requested to stop/close. */
        bool stopped{false};
        /*! Window configuration (extent, title, rendering config). */
        RenderingWindowConfiguration configuration;
        /*! Opaque OS window handle used for presentation. */
        void* platformHandle{nullptr};
        /*! Associated manager for global, platform specific, procedures */
        RenderingWindowManager* manager{nullptr};

        std::function<void(const RenderingWindowEvent&)> onEvent{};
    };

    class RenderingWindowManager : public Manager<RenderingWindowId, RenderingWindow> {
    public:
        RenderingWindow& create(const RenderingWindowConfiguration& configuration);

        void close(RenderingWindow& window) const;

        void resize(const RenderingWindow& window) const;

        /** Makes the OS window visible. */
        void show(const RenderingWindow& window) const;
    };

}