/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include <windows.h>
module lysa.resources.rendering_window;

import lysa.exception;
import lysa.input;
import lysa.log;

namespace lysa {

    bool RenderingWindow::_resettingMousePosition{false};

    std::map<MouseCursor, HCURSOR> RenderingWindow::_mouseCursors;

    struct MonitorEnumData {
        int  enumIndex{0};
        int  monitorIndex{0};
        RECT monitorRect{};
    };

    /** Enum callback used to find monitors/rects for window placement. */
    static BOOL CALLBACK monitorEnumProc(HMONITOR, HDC, LPRECT, LPARAM);

    /** Win32 window procedure to handle OS messages/events. */
    static LRESULT CALLBACK windowProcedure(HWND, UINT, WPARAM, LPARAM);

    void RenderingWindow::show() const {
        ShowWindow(static_cast<HWND>(platformHandle), SW_SHOW);
    }

    void RenderingWindow::close() const {
        PostMessage(static_cast<HWND>(platformHandle), WM_CLOSE, 0, 0);
    }

    void RenderingWindow::openPlatformWindow(const RenderingWindowConfiguration& config) {
        if (_mouseCursors.empty()) {
            _mouseCursors[MouseCursor::ARROW]    = LoadCursor(nullptr, IDC_ARROW);
            _mouseCursors[MouseCursor::WAIT]     = LoadCursor(nullptr, IDC_WAIT);
            _mouseCursors[MouseCursor::RESIZE_H] = LoadCursor(nullptr, IDC_SIZEWE);
            _mouseCursors[MouseCursor::RESIZE_V] = LoadCursor(nullptr, IDC_SIZENS);
        }

        const auto hInstance = GetModuleHandle(nullptr);

        const auto windowClass = WNDCLASSEX {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = windowProcedure,
            .hInstance = hInstance,
            .hCursor = LoadCursor(nullptr, IDC_ARROW),
            .lpszClassName = L"LysaGameWindowClass",
        };
        RegisterClassEx(&windowClass);

        int x = config.x;
        int y = config.y;
        int w = config.width;
        int h = config.height;
        DWORD style{WS_OVERLAPPEDWINDOW};
        DWORD exStyle{0};
        if (config.mode == RenderingWindowMode::FULLSCREEN) {
            DEVMODE dmScreenSettings = {};
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth = w;
            dmScreenSettings.dmPelsHeight = h;
            dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
                throw Exception( "Display mode change to FULLSCREEN failed");
            }
        }
        if (w == 0 || h == 0 || config.mode != RenderingWindowMode::WINDOWED) {
            if (config.mode == RenderingWindowMode::WINDOWED_FULLSCREEN || config.mode == RenderingWindowMode::FULLSCREEN) {
                style = WS_POPUP;
                exStyle = WS_EX_APPWINDOW;
            } else {
                style |=  WS_MAXIMIZE;
            }
            auto monitorRect = RECT{};
            const auto hPrimary = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
            auto monitorInfo = MONITORINFOEX{};
            monitorInfo.cbSize = sizeof(MONITORINFOEX);
            if (GetMonitorInfo(hPrimary, &monitorInfo)) {
                monitorRect = monitorInfo.rcMonitor;
            } else {
                auto monitorData = MonitorEnumData {};
                EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, reinterpret_cast<LPARAM>(&monitorData));
                monitorRect = monitorData.monitorRect;
            }
            w = monitorRect.right - monitorRect.left;
            h = monitorRect.bottom - monitorRect.top;
            x = monitorRect.left;
            y = monitorRect.top;
        }
        if (config.mode == RenderingWindowMode::WINDOWED || config.mode == RenderingWindowMode::WINDOWED_MAXIMIZED) {
            auto windowRect = RECT{0, 0, static_cast<LONG>(w), static_cast<LONG>(h)};
            AdjustWindowRect(&windowRect, style, FALSE);
            x = config.x == -1 ?
                (GetSystemMetrics(SM_CXSCREEN) - (windowRect.right - windowRect.left)) / 2 :
                config.x;
            y = config.y == -1 ?
            (GetSystemMetrics(SM_CYSCREEN) - (windowRect.bottom - windowRect.top)) / 2 :
                config.y;
        }

        _rect.left = x;
        _rect.top = y;
        _rect.right = _rect.left + w;
        _rect.bottom = _rect.top + h;
        const auto hwnd = CreateWindowEx(
            exStyle,
            windowClass.lpszClassName,
            std::to_wstring(config.title).c_str(),
            style,
            x, y,
            w, h,
            nullptr,
            nullptr,
            hInstance,
            nullptr);
        if (hwnd == nullptr) { throw Exception("Error creating window", std::to_string(GetLastError())); }

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        this->x = x;
        this->y = y;
        width = w;
        height = h;
        platformHandle = hwnd;
    }

    BOOL CALLBACK monitorEnumProc(HMONITOR, HDC , const LPRECT lprcMonitor, const LPARAM dwData) {
        const auto data = reinterpret_cast<MonitorEnumData*>(dwData);
        if (data->enumIndex == data->monitorIndex) {
            data->monitorRect = *lprcMonitor;
            return FALSE;
        }
        data->enumIndex++;
        return TRUE;
    }

    LRESULT CALLBACK windowProcedure(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        auto* window = reinterpret_cast<RenderingWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (window == nullptr) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        switch (message) {
        case WM_SIZE:
            if (IsIconic(hWnd)) {
                window->_pause(true);
            } else {
                window->_pause(false);
                window->_resized();
            }
            return 0;
        case WM_CLOSE:
            window->_closing();
            return 0;
        case WM_KEYDOWN:
        case WM_KEYUP:
            return Input::_windowProcedure(hWnd, message, wParam, lParam);
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEMOVE:
            return Input::_windowProcedure(hWnd, message, wParam, lParam);
        default:;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    float2 RenderingWindow::getMousePosition() const {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(static_cast<HWND>(platformHandle), &point);
        return { point.x, point.y };
    }

    void RenderingWindow::setMousePosition(const float2& position) const {
        POINT point{static_cast<int>(position.x), static_cast<int>(position.y)};
        ClientToScreen(static_cast<HWND>(platformHandle), &point);
        SetCursorPos(point.x, point.y);
    }

    void RenderingWindow::setMouseCursor(const MouseCursor cursor) const {
        SetCursor(_mouseCursors[cursor]);
        PostMessage(static_cast<HWND>(platformHandle), WM_SETCURSOR, 0, 0);
    }

    void RenderingWindow::resetMousePosition() const {
        _resettingMousePosition = true;
        SetCursorPos(_rect.left + (_rect.right-_rect.left) / 2,
                         _rect.top + (_rect.bottom-_rect.top) / 2);
    }

    void RenderingWindow::setMouseMode(const MouseMode mode) const {
        MSG msg;
        while (PeekMessageW(&msg, static_cast<HWND>(platformHandle), 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        switch (mode) {
        case MouseMode::VISIBLE:
            ReleaseCapture();
            ClipCursor(nullptr);
            ShowCursor(TRUE);
            resetMousePosition();
            break;
        case MouseMode::HIDDEN:
            ReleaseCapture();
            ClipCursor(nullptr);
            ShowCursor(FALSE);
            break;
        case MouseMode::VISIBLE_CAPTURED: {
            SetCapture(static_cast<HWND>(platformHandle));
            ClipCursor(&_rect);
            ShowCursor(TRUE);
            resetMousePosition();
            break;
        }
        case MouseMode::HIDDEN_CAPTURED: {
            SetCapture(static_cast<HWND>(platformHandle));
            ClipCursor(&_rect);
            ShowCursor(FALSE);
            resetMousePosition();
            break;
        }
        default:
            throw Exception("Unknown mouse mode");
        }
    }

}