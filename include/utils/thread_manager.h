/*
    This file is part of Reversan Engine.

    Reversan Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Reversan Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Reversan Engine. If not, see <https://www.gnu.org/licenses/>. 
*/

#ifndef THREAD_MANAGER
#define THREAD_MANAGER

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <queue>

/**
 * @brief Class for easy work with threads.
 * 
 * Implements thread pool with job queues for simple yet
 * efficient thread manager.
 * 
 * Accepts jobs in format void fnc(void *args).
 */
class ThreadManager {
    private:
        /// @brief Every job is made from the function and it's args.
        struct Task {
            void (*function)(void*);
            void* args;
        };

        /// @brief Vector holding all threads.
        std::vector<std::thread> thread_pool;

        /// @brief Queue holding all jobs to be executed.
        std::queue<Task> task_queue;

        /// @brief Mutex used for safe queue manipulation
        std::mutex queue_mutex;

        /// @brief Join mutex used for safe joins.
        std::mutex join_mutex;

        std::condition_variable queue_cond;
        std::condition_variable join_cond;

        /// @brief Atomic variable indicated destruction of thread manager.
        std::atomic<bool> stop;

        /// @brief Atomic variable holding the number of tasks in queue.
        std::atomic<int> task_count;
        
        void thread_fnc();

    public:
        /**
         * @brief Initializes thread manager and it's threadpool.
         * 
         * @param thread_count Number of threads in threadpool.
         */
        explicit ThreadManager(size_t thread_count);

        /// @brief Waits until all tasks are finished and destructs the manager.
        ~ThreadManager();
        
        /**
         * @brief Adds single task to the end of queue.
         * 
         * @param function Function to execute.
         * @param args Function's arguments.
         */
        void add_task(void (*function)(void*), void* args);

        /// @brief Block the calling thread until all tasks are finished.
        void join();
};

#endif
