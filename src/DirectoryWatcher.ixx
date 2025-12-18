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

export namespace lysa {

    struct DirectoryWatcherEvent : Event {
        //! A file last write date or size changed. Payload : file name
        static inline const event_type FILE_CHANGE{"FILE_CHANGE"};
    };

    class DirectoryWatcher {
    public:
        DirectoryWatcher(Context& ctx, const std::string& uri);

        ~DirectoryWatcher();

        void start();

        void stop() noexcept;

    private:
        Context& ctx;
        std::thread worker;
        std::atomic_bool stopped{false};

        void run() const;

#ifdef _WIN32
        std::wstring directoryName;
        HANDLE directory{INVALID_HANDLE_VALUE};
        HANDLE stopEvent{nullptr};
        HANDLE ioEvent{nullptr};
#endif
    };

}