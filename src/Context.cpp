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

    Context::Context(const ContextConfiguration& config):
        config(config),
        vireo(vireo::Vireo::create(config.backend, vireoDebugCallback)),
        fs(config.virtualFsConfiguration, vireo),
        events(config.eventsReserveCapacity),
        defer(config.commandsReserveCapacity),
        samplers(vireo, config.resourcesCapacity.samplers),
        graphicQueue(vireo->createSubmitQueue(vireo::CommandType::GRAPHIC, "Main graphic queue")),
        transferQueue(vireo->createSubmitQueue(vireo::CommandType::TRANSFER, "Main transfer queue")),
        asyncQueue(vireo, transferQueue, graphicQueue) {
    }

}