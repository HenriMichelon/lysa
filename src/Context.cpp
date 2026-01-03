/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#ifdef _WIN32
#include <windows.h>
#undef ERROR
#endif
module lysa.context;

import lysa.exception;
import lysa.log;

namespace  lysa {

    void vireoDebugCallback(const vireo::DebugLevel level, const std::string& message) {
        switch (level) {
        case vireo::DebugLevel::VERBOSE:
        case vireo::DebugLevel::INFO:
#ifdef _WIN32
            if (IsDebuggerPresent()) {
                OutputDebugStringA(message.c_str());
                OutputDebugStringA("\n");
            } else {
                Log::info(message);
            }
#else
            Log::info(message);
#endif
            break;
        case vireo::DebugLevel::WARNING:
#ifdef _WIN32
            if (IsDebuggerPresent()) {
                OutputDebugStringA(message.c_str());
                OutputDebugStringA("\n");
            } else {
                Log::warning(message);
            }
#else
            Log::warning(message);
#endif
            break;
        case vireo::DebugLevel::ERROR:
            Log::error(message);
            break;
        }
    }

    Context::Context(
        const vireo::Backend backend,
        const size_t eventsCapacity,
        const size_t commandsCapacity,
        const size_t samplersCapacity,
        const VirtualFSConfiguration& virtualFsConfiguration,
        const uint32 framesInFlight) :
        framesInFlight(framesInFlight),
        vireo(vireo::Vireo::create(backend, vireoDebugCallback)),
        fs(virtualFsConfiguration, vireo),
        events(eventsCapacity),
        defer(commandsCapacity),
        samplers(vireo, samplersCapacity),
        graphicQueue(vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue")),
        transferQueue(vireo->createSubmitQueue(vireo::CommandType::TRANSFER, "Main transfer queue")),
        asyncQueue(vireo, transferQueue, graphicQueue) {
    }

}