/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.async_pool;

import std;

export namespace lysa {

    /**
     * @brief A pool of asynchronous tasks executed in background threads.
     * @details Tasks are pushed to the pool and executed in separate jthreads.
     * The main loop take care of the terminated threads.
     */
    class AsyncTasksPool {
    public:
        /**
         * @brief Pushes a task to the pool.
         * @tparam L The type of the task (usually a lambda).
         * @param lambda The task to be executed.
         */
        template<typename L>
        void push(L&& lambda) {
            auto lock = std::lock_guard(mutex);
            pool.push_back(std::forward<L>(lambda));
        }

        void _process();

        ~AsyncTasksPool();

    private:
        /* Mutex for protecting access to the pool. */
        std::mutex mutex;
        /* List of threads in the pool. */
        std::list<std::jthread> pool;
    };

}