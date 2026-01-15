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

import lysa.context;
import lysa.event;
import lysa.input_event;
import lysa.math;
import lysa.rect;
import lysa.resources;
import lysa.resources.render_target;

export namespace lysa {

    /**
     * Mouse visibility & capture mode
     */
    enum class MouseMode : uint8 {
        //! Makes the mouse cursor visible
        VISIBLE = 0,
        //! Confines the mouse cursor to the game Window, and make it visible
        VISIBLE_CAPTURED = 1,
        //! Makes the mouse cursor hidden
        HIDDEN = 2,
        //! Confines the mouse cursor to the game Window, and make it hidden.
        HIDDEN_CAPTURED = 3,
    };

    /**
     * Mouse cursors types
     */
    enum class MouseCursor : uint8 {
        //! "Normal" arrow cursor
        ARROW = 0,
        //! Waiting cursor
        WAIT = 1,
        //! Horizontal resize cursor
        RESIZE_H = 2,
        //! Vertical resize cursor
        RESIZE_V = 3,
    };

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
    struct RenderingWindowEvent {
        //! The window is ready to be shown
        static inline const event_type READY{"RENDERING_WINDOW_READY"};
        //! The window is about to close
        static inline const event_type CLOSING{"RENDERING_WINDOW_CLOSING"};
        //! The window has been resized
        static inline const event_type RESIZED{"RENDERING_WINDOW_RESIZED"};
        //! User input
        static inline const event_type INPUT{"RENDERING_WINDOW_INPUT"};
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
        //! Configuration of the window render target
        RenderTargetConfiguration renderTargetConfiguration;
    };

    /**
    * Operating system window that serve as rendering surface.
    */
    class RenderingWindow : public UniqueResource {
    public:
        RenderingWindow(Context& ctx, const RenderingWindowConfiguration& config = {});

        ~RenderingWindow() override;

        /**
        * Make a previously created window visible on screen.
        */
        void show() const;

        void close() const;

        const Rect& getRect() const { return rect; }

        void setTitle(const std::string& title) const;

        RenderingWindowHandle getHandle() const { return handle; }

        /**
          * Sets the mouse visibility and capture mode
          *
          * @param mode MouseMode (visible, hidden, captured, etc.).
          */
        void setMouseMode(MouseMode mode) const;

        /**
         * Sets the mouse cursor
         *
         * @param cursor MouseCursor enum selecting the cursor shape.
         */
        void setMouseCursor(MouseCursor cursor) const;

        /**
         * Sets the mouse position to the center of the window
         */
        void resetMousePosition() const;

        bool isMouseHidden() const;

        /**
         * Returns the mouse position
         *
         * @return Mouse coordinates in pixels relative to the client area.
         */
        float2 getMousePosition() const;

        /**
         * Returns the mouse position
         *
         * @param position Coordinates in pixels relative to the client area.
         */
        void setMousePosition(const float2& position) const;

        RenderTarget& getRenderTarget() { return renderTarget; }

        const RenderTarget& getRenderTarget() const { return renderTarget; }

        void setPause(const bool pause) { renderTarget.setPause(pause); }

        bool isPaused() const { return renderTarget.isPaused(); }

        void _closing();

        void _resized(const Rect& rect);

        void _input(const InputEvent& inputEvent) const;

#ifdef _WIN32
        RECT _rect{};
        /** Internal flag used to suppress synthetic mouse‑move feedback. */
        static bool _resettingMousePosition;
        /** Cached OS cursors per MouseCursor enum value. */
        static std::map<MouseCursor, HCURSOR> _mouseCursors;
#endif

    private:
        // Platform specific handle/ID
        RenderingWindowHandle handle{nullptr};
        //! Associated render target
        RenderTarget renderTarget;
        //! True once the platform window has been requested to close
        bool closed{false};
        Rect rect;

        RenderingWindowHandle openPlatformWindow(const RenderingWindowConfiguration& config);
    };


}