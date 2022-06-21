//
// Created by 78472 on 2022/6/21.
//

#ifndef STL_THREADPOOL_H
#define STL_THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <vector>
#include <deque>
#include "Thread.h"

using namespace std;

namespace muduo{
    class ThreadPool {
    public:
        typedef std::function<void ()> Task;
    private:
        mutex mutex_;
        condition_variable taskQueueNotEmpty_;
        condition_variable taskQueueNotFull_;
        string threadPoolName_;
        Task threadInitCallback_;
        std::vector<std::unique_ptr<muduo::Thread>> threads_;
        std::deque<Task> taskQueue_;
        size_t maxQueueSize_;
        bool running_;

    public:
        explicit ThreadPool(string  threadName = string("ThreadPool"));

        ~ThreadPool();

        void start(int numThreads);

        void run(Task func);

        void stop();

        void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }

        void setThreadInitCallback(Task callback) { threadInitCallback_ = std::move(callback); }

        string name() const { return threadPoolName_; }

        size_t taskQueueSize() const;

    private:
        bool isFull();

        void runInThread();

        Task take();
    };

}


#endif //STL_THREADPOOL_H
