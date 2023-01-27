#include "ThreadPool.h"

void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
            if (should_terminate) {
                return;
            }
            job = jobs.top().second;
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::start() {
    const uint32_t num_threads =
        std::thread::hardware_concurrency();  // Max # of threads the system supports
    threads.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++) {
        threads.at(i) = std::thread(&ThreadPool::ThreadLoop, this);
    }
}

void ThreadPool::queueJob(const std::function<void()>& job, uint priority) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.emplace(priority, job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolBusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolBusy = jobs.empty();
    }
    return poolBusy;
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

ThreadPool::~ThreadPool() { stop(); }
