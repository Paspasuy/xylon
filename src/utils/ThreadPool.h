#ifndef XYLON_THREADPOOL_H
#define XYLON_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
public:
    void start();
    void queueJob(const std::function<void()>& job, uint priority);
    void stop();
    bool busy();

    ~ThreadPool();

private:
    struct Less {
        bool operator()(const std::pair<uint, std::function<void()>>& p1,
                        const std::pair<uint, std::function<void()>>& p2) {
            return p1.first < p2.first;
        }
    };

    void ThreadLoop();

    bool should_terminate = false;            // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                   // Prevents data races to the job queue
    std::condition_variable mutex_condition;  // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    std::priority_queue<std::pair<uint8_t, std::function<void()>>,
                        std::vector<std::pair<uint8_t, std::function<void()>>>, Less>
        jobs;

};

#endif  // XYLON_THREADPOOL_H
