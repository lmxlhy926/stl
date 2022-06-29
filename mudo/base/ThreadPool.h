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

/*
 * 线程池的实现逻辑：
 *      1. 包含一个任务函数列表，可以对任务函数进行存取
 *      2. 由用户加入特定的任务函数
 *      3. 每个线程例程的功能是相同的：从任务函数列表中取得函数，取到则执行，没有则等待新函数的加入
 *      4. 同时开启固定数量的多个线程，每个线程执行例程。
 */

namespace muduo{
    class ThreadPool {
    public:
        typedef std::function<void ()> Task;
    private:
        std::mutex mutex_;
        condition_variable taskQueueNotEmpty_;      //任务列表非空
        std::deque<Task> taskQueue_;                //任务函数列表

        string threadPoolName_;                     //线程名称
        Task threadInitCallback_;                   //线程初始回调
        bool running_;                              //运行标志
        std::vector<std::unique_ptr<muduo::Thread>> threads_;   //线程池列表

    public:
        explicit ThreadPool(string  threadName = string("ThreadPool"));

        ~ThreadPool();

        /**
         * 创建指定数量的线程并启动。
         * 所有的线程例程都是取得任务，然后执行
         * @param numThreads    指定线程池数量
         * 注：不能重复调用
         */
        void start(int numThreads = 0);

        /**
         * 将任务函数放入任务函数列表中
         * 如果任务函数列表满，则wait()等待
         * @param func  任务函数
         */
        void run(Task func);

        /**
         * 唤醒所有等待线程，空闲线程例程会执行完毕退出。
         * 依次回收所有例程。
         */
        void stop();

        void setThreadInitCallback(Task callback) { threadInitCallback_ = std::move(callback); }

        string name() const { return threadPoolName_; }

        //返回任务列表当前容量
        size_t taskQueueSize();

    private:
        //线程执行例程
        void runInThread();

        //取任务函数，如果任务列表为空，则阻塞等待
        Task take();
    };

}


#endif //STL_THREADPOOL_H
