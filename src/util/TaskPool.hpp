#ifndef LIFE_ENGINE_TASKPOOL_H
#define LIFE_ENGINE_TASKPOOL_H
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace life {

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

}


#endif //LIFE_ENGINE_TASKPOOL_H
