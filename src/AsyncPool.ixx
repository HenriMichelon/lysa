/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.async_pool;

import std;

export namespace lysa {

    class AsyncPool {
    public:
        template<typename L>
        void push(L&& lambda) {
            auto lock = std::lock_guard(mutex);
            pool.push_back(std::jthread(lambda));
        }

        void _process();

        ~AsyncPool();

    private:
        std::mutex mutex;
        std::list<std::jthread> pool;
    };

}