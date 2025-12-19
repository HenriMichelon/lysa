/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#define NOMINMAX
#include <windows.h>
module lysa.directory_watcher;

import lysa.exception;
import lysa.utils;

namespace lysa {

    DirectoryWatcher::DirectoryWatcher(Context& ctx, const std::string& uri, const uint32 debounceTimer) :
        ctx(ctx), debounceTimer(std::chrono::milliseconds(debounceTimer)) {
        directoryName = to_wstring(ctx.fs.getPath(uri));
        directory = CreateFileW(
            directoryName.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr
        );
        if (directory == INVALID_HANDLE_VALUE) {
            throw Exception("FileWatcher CreateFileW(FILE_LIST_DIRECTORY) failed");
        }
        stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
        ioEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        if (!stopEvent || !ioEvent) {
            throw Exception("FileWatcher CreateEvent failed");
        }
        start();
    }

    DirectoryWatcher::~DirectoryWatcher() {
        stop();
    }

    void DirectoryWatcher::start() {
        worker = std::thread([this]{ run(); });
    }

    void DirectoryWatcher::stop() noexcept {
        if (stopped.exchange(true)) return;
        if (stopEvent) SetEvent(stopEvent);
        if (directory != INVALID_HANDLE_VALUE) CancelIoEx(directory, nullptr);
        if (worker.joinable()) worker.join();
        if (ioEvent) {
            CloseHandle(ioEvent);
            ioEvent = nullptr;
        }
        if (stopEvent) {
            CloseHandle(stopEvent);
            stopEvent = nullptr;
        }
        if (directory != INVALID_HANDLE_VALUE) {
            CloseHandle(directory);
            directory = INVALID_HANDLE_VALUE;
        }
    }

    void DirectoryWatcher::run() const {
        BYTE buffer[4096];
        auto ov = OVERLAPPED {
            .hEvent = ioEvent
        };

        auto lastFileName = std::wstring();
        auto lastFileTime = std::chrono::steady_clock::time_point::min();

        while (WaitForSingleObject(stopEvent, 0) != WAIT_OBJECT_0) {
            ResetEvent(ioEvent);
            ov.Internal = ov.InternalHigh = ov.Offset = ov.OffsetHigh = 0;

            if (!ReadDirectoryChangesW(
                    directory,
                    buffer, sizeof(buffer),
                    FALSE,
                    filter,
                    nullptr,
                    &ov,
                    nullptr)) {
                break;
            }

            const HANDLE waits[2] = { stopEvent, ioEvent };
            const auto w = WaitForMultipleObjects(
                2,
                waits,
                FALSE,
                INFINITE);

            if (w == WAIT_OBJECT_0) {
                CancelIoEx(directory, &ov);
                break;
            }
            if (w != WAIT_OBJECT_0 + 1) {
                CancelIoEx(directory, &ov);
                break;
            }

            DWORD bytes = 0;
            if (!GetOverlappedResult(directory, &ov, &bytes, FALSE)) { continue; }

            auto* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
            while (true) {
                std::wstring changed{info->FileName, info->FileNameLength / sizeof(wchar_t)};

                const auto now = std::chrono::steady_clock::now();
                if ((changed != lastFileName) ||
                    ((now - lastFileTime) > debounceTimer)) {
                    ctx.events.fire({
                        INVALID_ID,
                        DirectoryWatcherEvent::FILE_CHANGE,
                        to_string(changed)}
                    );
                    lastFileName = std::move(changed);
                    lastFileTime = now;
                }

                if (info->NextEntryOffset == 0) { break; }
                info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                    reinterpret_cast<BYTE*>(info) + info->NextEntryOffset
                );
            }
        }
    }

}