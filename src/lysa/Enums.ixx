/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.enums;

import std;
import lysa.types;

export namespace lysa {

    /**
     * Rendering Window mode
     */
    enum class WindowMode : uint8 {
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
    * A Material transparency mode
    * Any transparency mode other than Transparency::DISABLED has a greater performance impact compared to opaque rendering.
    */
    enum class Transparency {
        //! The material will not use transparency. This is the fastest to render.
        DISABLED = 0,
        //! The material will use the texture's alpha values for transparency.
        ALPHA = 1,
        //! The material will cut off all values below a threshold, the rest will remain opaque.
        // SCISSOR = 2,
        //! The material will cut off all values below a threshold, the rest will use the texture's alpha values for transparency.
        // SCISSOR_ALPHA = 3,
    };

    /**
     * Nodes state when the scene is paused or running
     */
    enum class ProcessMode {
        //! Inherits mode from the node's parent. This is the default for any newly created node
        INHERIT = 0,
        //! Stops processing when Application::isPaused() is true. This is the inverse of PROCESS_MODE_WHEN_PAUSED
        PAUSABLE = 1,
        //! Process only when Application::isPaused() is true. This is the inverse of PROCESS_MODE_PAUSABLE
        WHEN_PAUSED = 2,
        //! Always process. Keeps processing, ignoring Application::isPaused(). This is the inverse of PROCESS_MODE_DISABLED
        ALWAYS = 3,
        //! Never process. Completely disables processing, ignoring Application::isPaused(). This is the inverse of PROCESS_MODE_ALWAYS
        DISABLED = 4,
    };


    /**
     * Source of an input event
     */
    enum class InputEventType : uint8 {
        //! A key have been pressed or released
        KEY = 0,
        //! The mouse cursor moved
        MOUSE_MOTION = 1,
        //! A mouse button have been pressed or released
        MOUSE_BUTTON = 2,
        //! A gamepad button have been pressed or released
        GAMEPAD_BUTTON = 3,
    };

    /**
     * Keyboard modifier keys
     */
    enum class KeyModifier : uint8 {
        //! Left & right shift keys
        SHIFT = 0x0001,
        //! Left & right control keys
        CONTROL = 0x0002,
        //! Left & right alt keys
        ALT = 0x0004,
    };

    /**
     * Key codes, QWERTY layout to keep the WASD keys
     */
    enum Key : uint32 {
        KEY_NONE = 0,
        //! Space
        KEY_SPACE = 1,
        //! -
        KEY_DASH = 2,
        //! |
        KEY_PIPE = 3,
        //! '
        KEY_APOSTROPHE = 4,
        //! ,
        KEY_COMMA = 5,
        //! $.
        KEY_PERIOD = 6,
        //! ?
        KEY_QUESTIONMARK = 7,
        //! 0
        KEY_0 = 8,
        //! 1
        KEY_1 = 9,
        //! 2
        KEY_2 = 10,
        //! 3
        KEY_3 = 11,
        //! 4
        KEY_4 = 12,
        //! 5
        KEY_5 = 13,
        //! 6
        KEY_6 = 14,
        //! 7
        KEY_7 = 15,
        //! 8
        KEY_8 = 16,
        //! 9
        KEY_9 = 17,
        //! ;
        KEY_SEMICOLON = 18,
        //! =
        KEY_EQUAL = 19,
        //! A
        KEY_A = 20,
        //! B
        KEY_B = 21,
        //! C
        KEY_C = 22,
        //! D
        KEY_D = 23,
        //! E
        KEY_E = 24,
        //! F
        KEY_F = 25,
        //! G
        KEY_G = 26,
        //! H
        KEY_H = 27,
        //! I
        KEY_I = 28,
        //! J
        KEY_J = 29,
        //! K
        KEY_K = 30,
        //! L
        KEY_L = 31,
        //! M
        KEY_M = 32,
        //! N
        KEY_N = 33,
        //! O
        KEY_O = 34,
        //! P
        KEY_P = 35,
        //! Q
        KEY_Q = 36,
        //! R
        KEY_R = 37,
        //! S
        KEY_S = 38,
        //! T
        KEY_T = 39,
        //! U
        KEY_U = 40,
        //! V
        KEY_V = 41,
        //! W
        KEY_W = 42,
        //! X
        KEY_X = 43,
        //! Y
        KEY_Y = 44,
        //! Z
        KEY_Z = 45,
        //! [
        KEY_LEFT_BRACKET = 46,
        //! backslash
        KEY_BACKSLASH      = 47,
        //! ]
        KEY_RIGHT_BRACKET = 48,
        KEY_GRAVE_ACCENT  = 49,
        //! ESC
        KEY_ESCAPE = 50,
        //! ⏎
        KEY_ENTER = 51,
        //! Tabulation
        KEY_TAB = 52,
        //! back space
        KEY_BACKSPACE = 53,
        //! Insert
        KEY_INSERT = 54,
        //! Delete
        KEY_DELETE = 55,
        //! →
        KEY_RIGHT = 56,
        //! ←
        KEY_LEFT = 57,
        //! ↓
        KEY_DOWN = 58,
        //! ↑
        KEY_UP = 59,
        //! Page ↑
        KEY_PAGE_UP = 60,
        //! Page ↓
        KEY_PAGE_DOWN = 61,
        //! Home/Start
        KEY_HOME = 62,
        //! End
        KEY_END = 63,
        //! Left caps lock
        KEY_CAPS_LOCK = 64,
        //! Scroll lock
        KEY_SCROLL_LOCK = 65,
        //! Numeric keypad lock
        KEY_NUM_LOCK = 66,
        //! Print
        KEY_PRINT_SCREEN = 67,
        //! Pause
        KEY_PAUSE = 68,
        //! F1
        KEY_F1 = 69,
        //! F2
        KEY_F2 = 70,
        //! F3
        KEY_F3 = 71,
        //! F4
        KEY_F4 = 72,
        //! F5
        KEY_F5 = 73,
        //! F6
        KEY_F6 = 74,
        //! F7
        KEY_F7 = 75,
        //! F8
        KEY_F8 = 76,
        //! F9
        KEY_F9 = 77,
        //! F10
        KEY_F10 = 78,
        //! F11
        KEY_F11 = 79,
        //! F12
        KEY_F12 = 80,
        //! Keypad 0
        KEY_KP_0 = 81,
        //! Keypad 1
        KEY_KP_1 = 82,
        //! Keypad 2
        KEY_KP_2 = 83,
        //! Keypad 3
        KEY_KP_3 = 84,
        //! Keypad 4
        KEY_KP_4 = 85,
        //! Keypad 5
        KEY_KP_5 = 86,
        //! Keypad 6
        KEY_KP_6 = 87,
        //! Keypad 7
        KEY_KP_7 = 88,
        //! Keypad 8
        KEY_KP_8 = 89,
        //! Keypad 9
        KEY_KP_9 = 90,
        //! Keypad .
        KEY_KP_PERIOD = 91,
        //! Keypad /
        KEY_KP_DIVIDE = 92,
        //! Keypad *
        KEY_KP_MULTIPLY = 93,
        //! Keypad -
        KEY_KP_SUBTRACT = 94,
        //! Keypad +
        KEY_KP_ADD = 95,
        //! Keypad ⏎
        KEY_KP_ENTER = 96,
        //! Keypad =
        KEY_KP_EQUAL = 97,
        //! Left Shift
        KEY_LEFT_SHIFT = 98,
        //! Left Control
        KEY_LEFT_CONTROL = 99,
        //! Left Alt
        KEY_LEFT_ALT = 100,
        //! Left Super/Windows
        KEY_LEFT_SUPER = 101,
        //! Right Shift
        KEY_RIGHT_SHIFT = 102,
        //! Right Control
        KEY_RIGHT_CONTROL = 103,
        //! Right Alt
        KEY_RIGHT_ALT = 104,
        //! Right Super/Windows
        KEY_RIGHT_SUPER = 105,
    };

#ifdef _WIN32
    enum OsKey {
        // https://learn.microsoft.com/fr-fr/windows/win32/inputdev/about-keyboard-input#scan-codes
        OS_KEY_SPACE         = 0x0039,
        OS_KEY_DASH          = 0x000C,
        OS_KEY_PIPE          = 0x002B,
        OS_KEY_APOSTROPHE    = 0x0028,
        OS_KEY_COMMA         = 0x0033,
        OS_KEY_PERIOD        = 0x0034,
        OS_KEY_QUESTIONMARK  = 0x0035,
        OS_KEY_0             = 0x000B,
        OS_KEY_1             = 0x0002,
        OS_KEY_2             = 0x0003,
        OS_KEY_3             = 0x0004,
        OS_KEY_4             = 0x0005,
        OS_KEY_5             = 0x0006,
        OS_KEY_6             = 0x0007,
        OS_KEY_7             = 0x0008,
        OS_KEY_8             = 0x0009,
        OS_KEY_9             = 0x000A,
        OS_KEY_SEMICOLON     = 0x0027,
        OS_KEY_EQUAL         = 0x000D,
        OS_KEY_A             = 0x001E,
        OS_KEY_B             = 0x0030,
        OS_KEY_C             = 0x002E,
        OS_KEY_D             = 0x0020,
        OS_KEY_E             = 0x0012,
        OS_KEY_F             = 0x0021,
        OS_KEY_G             = 0x0022,
        OS_KEY_H             = 0x0023,
        OS_KEY_I             = 0x0017,
        OS_KEY_J             = 0x0024,
        OS_KEY_K             = 0x0025,
        OS_KEY_L             = 0x0026,
        OS_KEY_M             = 0x0032,
        OS_KEY_N             = 0x0031,
        OS_KEY_O             = 0x0018,
        OS_KEY_P             = 0x0019,
        OS_KEY_Q             = 0x0010,
        OS_KEY_R             = 0x0013,
        OS_KEY_S             = 0x001F,
        OS_KEY_T             = 0x0014,
        OS_KEY_U             = 0x0016,
        OS_KEY_V             = 0x002F,
        OS_KEY_W             = 0x0011,
        OS_KEY_X             = 0x002D,
        OS_KEY_Y             = 0x0015,
        OS_KEY_Z             = 0x002C,
        OS_KEY_LEFT_BRACKET  = 0x001A,
        OS_KEY_BACKSLASH     = 0x002B,
        OS_KEY_RIGHT_BRACKET = 0x001B,
        OS_KEY_GRAVE_ACCENT  = 0x0029,
        OS_KEY_ESCAPE        = 0x0001,
        OS_KEY_ENTER         = 0x001C,
        OS_KEY_TAB           = 0x000F,
        OS_KEY_BACKSPACE     = 0x000E,
        OS_KEY_INSERT        = 0x52,
        OS_KEY_DELETE        = 0x53,
        OS_KEY_RIGHT         = 0x4D,
        OS_KEY_LEFT          = 0x4B,
        OS_KEY_DOWN          = 0x50,
        OS_KEY_UP            = 0x48,
        OS_KEY_PAGE_UP       = 0x49,
        OS_KEY_PAGE_DOWN     = 0x51,
        OS_KEY_HOME          = 0x47,
        OS_KEY_END           = 0x4F,
        OS_KEY_CAPS_LOCK     = 0x003A,
        OS_KEY_SCROLL_LOCK   = 0x0046,
        OS_KEY_NUM_LOCK      = 0x0045,
        OS_KEY_PRINT_SCREEN  = 0x54,
        OS_KEY_PAUSE         = 0x46,
        OS_KEY_F1            = 0x003B,
        OS_KEY_F2            = 0x003C,
        OS_KEY_F3            = 0x003D,
        OS_KEY_F4            = 0x003E,
        OS_KEY_F5            = 0x003F,
        OS_KEY_F6            = 0x0040,
        OS_KEY_F7            = 0x0041,
        OS_KEY_F8            = 0x0042,
        OS_KEY_F9            = 0x0043,
        OS_KEY_F10           = 0x0044,
        OS_KEY_F11           = 0x0057,
        OS_KEY_F12           = 0x0058,
        OS_KEY_KP_0          = 0x0052,
        OS_KEY_KP_1          = 0x004F,
        OS_KEY_KP_2          = 0x0050,
        OS_KEY_KP_3          = 0x0051,
        OS_KEY_KP_4          = 0x004B,
        OS_KEY_KP_5          = 0x004C,
        OS_KEY_KP_6          = 0x004D,
        OS_KEY_KP_7          = 0x0047,
        OS_KEY_KP_8          = 0x0048,
        OS_KEY_KP_9          = 0x0049,
        OS_KEY_KP_PERIOD     = 0x0053,
        OS_KEY_KP_DIVIDE     = 0x35,
        OS_KEY_KP_MULTIPLY   = 0x0037,
        OS_KEY_KP_SUBTRACT   = 0x004A,
        OS_KEY_KP_ADD        = 0x004E,
        OS_KEY_KP_ENTER      = 0x1C,
        OS_KEY_KP_EQUAL      = 0x0059,
        OS_KEY_LEFT_SHIFT    = 0x002A,
        OS_KEY_LEFT_CONTROL  = 0x001D,
        OS_KEY_LEFT_ALT      = 0x0038,
        OS_KEY_LEFT_SUPER    = 0x5B,
        OS_KEY_RIGHT_SHIFT   = 0x0036,
        OS_KEY_RIGHT_CONTROL = 0x1D,
        OS_KEY_RIGHT_ALT     = 0x38,
        OS_KEY_RIGHT_SUPER   = 0x5C,
    };
#endif

    /**
     * Mouse buttons
     */
    enum class MouseButton : uint8 {
        NONE = 0b0000,
        //! Left
        LEFT = 0b0001,
        //! Right
        RIGHT = 0b0010,
        //! Middle
        MIDDLE = 0b0100,
        //! Wheel. Pressed==true means rotated backward
        WHEEL = 0b1000,
    };

    /**
     * Gamepas buttons
     */
    enum class GamepadButton : uint8 {
        //! A or X
        A = 0,
        //! A or X
        CROSS = A,
        //! B or ○
        B = 1,
        //! B or ○
        CIRCLE = B,
        //! X or □
        X = 2,
        //! X or □
        SQUARE = X,
        //! Y or △
        Y = 3,
        //! Y or △
        TRIANGLE = Y,
        //! Left shoulder/bumper
        LB = 4,
        //! Left shoulder/bumper
        L1 = LB,
        //! Right shoulder/bumper
        RB = 5,
        //! Right shoulder/bumper
        R1 = RB,
        //! Back/Share
        BACK = 6,
        //! Back/Share
        SHARE = BACK,
        //! Start/Menu
        START = 7,
        //! Start/Menu
        MENU = START,
        //! Left trigger
        LT = 8,
        //! Left trigger
        L2 = LT,
        //! Right trigger
        RT = 9,
        //! Right trigger
        R2 = RT,
        //! Directional hat ↑
        DPAD_UP = 10,
        //! Directional hat →
        DPAD_RIGHT = 11,
        //! Directional hat ↓
        DPAD_DOWN = 12,
        //! Directional hat ←
        DPAD_LEFT = 13,
        LAST      = DPAD_LEFT,
    };

    /**
     * Gamepad thumbs joysticks
     */
    enum class GamepadAxisJoystick : uint8 {
        //! Left stick/joystick
        LEFT = 0,
        //! Right stick/joystick
        RIGHT = 1,
    };

    /**
     * Gamepad axis & triggers
     */
    enum class GamepadAxis : uint8 {
        //! Left stick/joystick X
        LEFT_X = 0,
        //! Left stick/joystick Y
        LEFT_Y = 1,
        //! Right stick/joystick X
        RIGHT_X = 2,
        //! Right stick/joystick Y
        RIGHT_Y       = 3,
        LEFT_TRIGGER  = 4,
        RIGHT_TRIGGER = 5,
        LAST          = RIGHT_TRIGGER,
    };

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
     * Where to log messages
     */
    enum LoggingMode : uint32 {
        //! Disable logging
        LOGGING_MODE_NONE = 0,
        /**
        * Open an external Window (on the first screen if you have multiple screen) to display the log messages.
        * Log message appearance in the Window can be deferred to the next frame if the log message is sent from a thread different from the main thread
        */
        LOGGING_MODE_WINDOW = 0x001,
        /**
         * Log the messages into a file named 'log.txt'
         */
        LOGGING_MODE_FILE = 0x010,
        /**
         * Log the messages to std::cout. WIN32 applications needs to be linked with `-mconsole`
         */
        LOGGING_MODE_STDOUT = 0x100
    };

    /**
     * Log levels
     */
    enum class LogLevel : int32 {
        TRACE       = -1,
        DEBUG       = 0,
        INFO        = 1,
        GAME1       = 2,
        GAME2       = 3,
        GAME3       = 4,
        WARNING     = 5,
        ERROR       = 6,
        CRITICAL    = 7,
        INTERNAL    = 100,
    };


    /**
    * Animation type for a animation track
    */
    enum class AnimationType : uint8 {
        /**
         * The values are the translation along the X, Y, and Z axes.
         */
        TRANSLATION = 1,
        /**
         * The values are a quaternion in the order x, y, z, w where w is the scalar.
         */
        ROTATION = 2,
        /**
         * The values are scaling factors along the X, Y, and Z axes.
         */
        SCALE = 3,
        // Weights = 4,
    };

    /**
     * Interpolation type to apply when caculating animation values
     */
    enum class AnimationInterpolation : uint8 {
        /**
         * The animated values are linearly interpolated between keyframes..
         */
        LINEAR = 0,
        /**
         * The animated values remain constant to the output of the first keyframe, until the next
         * keyframe.
         */
        STEP = 1,
        /**
         * The animation’s interpolation is computed using a cubic spline with specified tangents.
         */
        // CUBIC = 2,
    };

    /**
     * Animation loop mode
     */
    enum class AnimationLoopMode : uint8 {
        //! No loop (default)
        NONE    = 0,
        //! Restart from start of the track
        LINEAR  = 1,
    };

    /**
     * A Tween transition type
     */
    enum class TransitionType : uint8 {
        /** The animation is interpolated linearly */
        LINEAR = 0,
    };

    enum class CombineMode : uint8 {
        AVERAGE = 0,
        MIN     = 1,
        MAX     = 2,
        MULTIPLY= 3,
    };

    enum class RendererType : uint8 {
        FORWARD  = 0,
        DEFERRED = 1,
    };

    enum class ToneMappingType : uint8 {
        NONE     = 0,
        REINHARD = 1,
        ACES     = 2,
    };

    enum class AntiAliasingType : uint8 {
        NONE     = 0,
        FXAA     = 1,
        SMAA     = 2,
    };

}
