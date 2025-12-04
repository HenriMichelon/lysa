/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.command_buffer;

namespace lysa {

    void CommandBuffer::_process() {
        {
            auto lock = std::scoped_lock (queueMutex);
            processingQueue.swap(queue);
        }
        for (const Command& e : processingQueue) {
            e();
        }
        processingQueue.clear();
    }

    CommandBuffer::CommandBuffer(const size_t reservedCapacity) {
        queue.reserve(reservedCapacity);
        processingQueue.reserve(reservedCapacity);
    }

}