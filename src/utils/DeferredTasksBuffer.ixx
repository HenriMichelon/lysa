/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.command_buffer;

import std;

export namespace lysa {

    /**
     * @brief A buffer for tasks that need to be deferred and executed later.
     * @details Tasks are queued and processed at the start of the main loop.
     */
    class DeferredTasksBuffer {
    public:
        /** @brief Type definition for a task command. */
        using Command = std::function<void()>;

        /**
         * @brief Pushes a task to the deferred buffer.
         * @tparam L The type of the task (usually a lambda).
         * @param lambda The task to be executed.
         */
        template<typename L>
        void push(L&& lambda) {
            queue.emplace_back(std::forward<L>(lambda));
        }

        void _process();

        /**
         * @brief Creates a tasks queue with an initial capacity.
         * @param reservedCapacity The initial number of tasks to reserve space for.
         */
        DeferredTasksBuffer(size_t reservedCapacity);

    private:
        /* The queue of pending tasks. */
        std::vector<Command> queue;
        /* The queue currently being processed. */
        std::vector<Command> processingQueue;
        /* Mutex for protecting access to the task queue. */
        std::mutex queueMutex;
    };

}