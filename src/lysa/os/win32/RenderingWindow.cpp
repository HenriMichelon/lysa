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
import lysa.log;
import lysa.resources.locator;

namespace lysa {

    struct MonitorEnumData {
        int  enumIndex{0};
        int  monitorIndex{0};
        RECT monitorRect{};
    };

    /** Enum callback used to find monitors/rects for window placement. */
    static BOOL CALLBACK monitorEnumProc(HMONITOR, HDC, LPRECT, LPARAM);

    /** Win32 window procedure to handle OS messages/events. */
    static LRESULT CALLBACK windowProcedure(HWND, UINT, WPARAM, LPARAM);

    void RenderingWindowManager::show(const unique_id id) const {
        const auto& window = get(id);
        ShowWindow(static_cast<HWND>(window.platformHandle), SW_SHOW);
    }

    unique_id RenderingWindowManager::create(const RenderingWindowConfiguration& config) {
        auto& window = ResourcesManager::create();
            Log::trace();

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

        RECT rect;
        rect.left = x;
        rect.top = y;
        rect.right = rect.left + w;
        rect.bottom = rect.top + h;
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

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&window));
        window.x = x;
        window.y = y;
        window.width = w;
        window.height = h;
        window.platformHandle = hwnd;
        window.locator = &ctx.resourcesLocator;
        ctx.eventManager.push({window.id, static_cast<event_type>(RenderingWindowEventType::READY)});
        return window.id;
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
        auto& manager = window->locator->get<RenderingWindowManager>(RENDERING_WINDOW);
        switch (message) {
        case WM_SIZE:
            if (IsIconic(hWnd)) {
                window->stopped = true;
            } else {
                window->stopped = false;
                manager.resized(window->id);
            }
            return 0;
        case WM_CLOSE:
            manager.closing(window->id);
            break;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEMOVE:
            // return Input::windowProcedure(hWnd, message, wParam, lParam);
        default:;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

}