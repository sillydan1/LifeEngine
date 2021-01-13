/**
 * Copyright (C) 2021 Asger Gitz-Johansen

   This file is part of lifeengine.

    lifeengine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    lifeengine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with lifeengine.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LIFEENGINE_TASKPOOL_H
#define LIFEENGINE_TASKPOOL_H
#include <lifepch.h>

class TaskPool {
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> pool;
    std::mutex task_lock;
    std::condition_variable task_q;
    std::atomic_bool alive;
public:
    void push_task(std::function<void()> t) {
        std::lock_guard<std::mutex> lock{task_lock};
        tasks.push(t);
        task_q.notify_all();
    }

    void push_many(const std::vector<std::function<void()>>& v_tasks) {
        std::lock_guard<std::mutex> lock{task_lock};
        for(auto& t : v_tasks) {
            tasks.push(t);
        }
        task_q.notify_all();
    }

    TaskPool(size_t poolsize)
            : tasks{}, pool{}, task_lock{}, task_q{}, alive{true}
    {
        for (int i = 0; i < poolsize; i++) {
            pool.emplace_back([&](){ while(true) {
                std::function<void()> t;
                {
                    std::unique_lock<std::mutex> a(task_lock);
                    while(tasks.empty() && alive.load()) {
                        task_q.wait(a);
                    }
                    if(tasks.empty() && !alive.load()) return;
                    t = tasks.front();
                    tasks.pop();
                }
                t();
            } });
        }
    }

    ~TaskPool() {
        {
            std::lock_guard<std::mutex> lock{task_lock};
            alive = false;
            task_q.notify_all(); // We are no longer alive, so close the pool
        }
        for(auto& t : pool) t.join();
    }
};

#endif //LIFEENGINE_TASKPOOL_H
