/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.command_buffer;

import lysa.types;

export namespace lysa {

    class CommandBuffer {
    public:
        using Command = std::function<void()>;

        template<typename L>
        void push(L&& lambda) {
            queue.emplace_back(std::forward<L>(lambda));
        }

        void _process();

        CommandBuffer(size_t reservedCapacity);

    private:
        std::vector<Command> queue;
        std::vector<Command> processingQueue;
        std::mutex queueMutex;
    };

}