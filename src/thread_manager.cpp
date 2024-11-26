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

#include "thread_manager.h"

ThreadManager::ThreadManager(size_t thread_count) : stop(false) {
    for (size_t i = 0; i < thread_count; ++i) {
        thread_pool.emplace_back(&ThreadManager::thread_fnc, this);
    }
}

ThreadManager::~ThreadManager() {
    // wait for all tasks to finish
    join();
    // join all threads in threadpool
    stop = true;
    queue_cond.notify_all();
    for (auto& t : thread_pool) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ThreadManager::thread_fnc() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            // wait until task is avaible
            queue_cond.wait(lock, [this]() { return stop || !task_queue.empty(); });
            
            if (stop && task_queue.empty())
                return;

            // take task from queue
            task = task_queue.front();
            task_queue.pop();
        }

        // execute the task
        task.function(task.args);

        // decrement task count and notify this change to threads waiting on join
        if (task_count.fetch_sub(1, std::memory_order_relaxed) == 1) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            join_cond.notify_all();
        }
    }
}

void ThreadManager::add_task(void (*function)(void*), void* args) {
    // safely add task to the queue
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        task_queue.push(Task{function, args});
    }
    // increment task count
    task_count.fetch_add(1, std::memory_order_relaxed);
    queue_cond.notify_one();
}

void ThreadManager::join() {
    std::unique_lock<std::mutex> lock(join_mutex);
    join_cond.wait(lock, [this]() { return task_count.load(std::memory_order_relaxed) == 0;});
}
