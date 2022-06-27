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
        condition_variable taskQueueNotEmpty_;      //任务列表非空
        condition_variable taskQueueNotFull_;       //任务列表非满
        string threadPoolName_;                     //线程名称
        Task threadInitCallback_;                   //线程初始回调
        std::vector<std::unique_ptr<muduo::Thread>> threads_;   //线程池列表
        std::deque<Task> taskQueue_;        //任务函数列表
        size_t maxTaskQueueSize_;           //任务列表最大容量
        bool running_;                      //运行标志

    public:
        explicit ThreadPool(string  threadName = string("ThreadPool"));

        ~ThreadPool();

        void start(int numThreads);

        void run(Task func);

        void stop();

        void setMaxQueueSize(int maxSize) { maxTaskQueueSize_ = maxSize; }

        void setThreadInitCallback(Task callback) { threadInitCallback_ = std::move(callback); }

        string name() const { return threadPoolName_; }

        size_t taskQueueSize(){
            std::lock_guard<std::mutex> lg(mutex_);
            return taskQueue_.size();
        }

    private:
        bool isFull();

        void runInThread();

        //取任务函数，如果任务列表为空，则阻塞等待
        Task take();
    };

}


#endif //STL_THREADPOOL_H
