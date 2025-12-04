/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.async_pool;

namespace lysa {

    void AsyncPool::_process() {
        if (!pool.empty()) {
            auto lock = std::lock_guard(mutex);
            for (auto it = pool.begin(); it != pool.end();) {
                if (it->joinable()) {
                    ++it;
                } else {
                    it = pool.erase(it);
                }
            }
        }
    }

    AsyncPool::~AsyncPool() {
        for(auto&t : pool) {
            t.join();
        }
    }

}