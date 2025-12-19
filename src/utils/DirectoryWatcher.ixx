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
export module lysa.directory_watcher;

import std;
import lysa.context;
import lysa.event;
import lysa.types;

export namespace lysa {

    /**
     * @brief Event triggered when a file in the watched directory is modified.
     */
    struct DirectoryWatcherEvent : Event {
        /**
         * @brief A file last write date or size changed.
         * @details The payload is the file name.
         */
        static inline const event_type FILE_CHANGE{"FILE_CHANGE"};
    };

    /**
     * @brief Monitors a directory for changes and emits events when files are modified.
     * @details This class runs a background thread that uses OS-specific APIs to watch for file changes.
     */
    class DirectoryWatcher {
    public:
        /**
         * @brief Creates a DirectoryWatcher.
         * @param ctx The application context.
         * @param uri The URI of the directory to watch.
         * @param debounceTimer The time to wait (in milliseconds) before emitting an event after a change is detected
         * for the same file as the previous detected change.
         */
        DirectoryWatcher(Context& ctx, const std::string& uri, uint32 debounceTimer = 100);

        ~DirectoryWatcher();

        /**
         * @brief Starts the background thread to watch the directory.
         */
        void start();

        /**
         * @brief Stops the background thread and releases resources.
         */
        void stop() noexcept;

    private:
        /* Reference to the application context. */
        Context& ctx;
        /* Duration to wait before repeating events for the same file. */
        const std::chrono::steady_clock::duration debounceTimer;
        /* The background worker thread. */
        std::thread worker;
        /* Flag indicating if the watcher has been stopped. */
        std::atomic_bool stopped{false};

        /* Internal loop that waits for directory changes. */
        void run() const;

#ifdef _WIN32
        /* Windows specific filter for directory change notifications. */
        static constexpr auto filter{FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE};
        /* The wide-string path of the directory being watched. */
        std::wstring directoryName;
        /* Handle to the directory. */
        HANDLE directory{INVALID_HANDLE_VALUE};
        /* Handle to the event used to stop the watcher. */
        HANDLE stopEvent{nullptr};
        /* Handle to the event used for overlapped I/O. */
        HANDLE ioEvent{nullptr};
#endif
    };

}