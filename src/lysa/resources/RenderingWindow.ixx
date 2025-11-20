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
import lysa.math;
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

    enum class RenderingWindowEventType : uint32 {
        READY,
        CLOSE,
        RESIZE,
    };

    using RenderingWindowId = unique_id;

    struct RenderingWindow {
        /** Unique ID  */
        RenderingWindowId id;
        /** Window configuration (extent, title, rendering config). */
        RenderingWindowConfiguration configuration;
        /** Opaque OS window handle used for presentation. */
        void* windowHandle;
        /** True once the window has been requested to stop/close. */
        bool stopped{false};
#ifdef _WIN32
#endif
    };

    class RenderingWindowManager {
    public:


    private:


    };

}