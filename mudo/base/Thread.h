//
// Created by 78472 on 2022/6/16.
//

#ifndef STL_THREAD_H
#define STL_THREAD_H

#include "CountDownLatch.h"
#include "Atomic.h"

#include <memory>
#include <pthread.h>
#include <functional>

namespace muduo{
    class Thread {
    public:
        typedef std::function<void()> ThreadFunc;
    private:
        bool                started_;       //线程启动标识
        bool                joined_;        //线程回收标识
        pthread_t           pthreadId_;     //线程表示ID
        pid_t               pid_;           //进程ID
        ThreadFunc          func_;          //执行函数
        string              name_;          //线程名字
        CountDownLatch      latch_;
        static AtomicInt32  numCreated_;    //线程创建总数
    public:
        explicit Thread(ThreadFunc func, string name = string());

        ~Thread();

        //启动线程，开始执行后返回
        void start();

        //阻塞回收线程
        int join();

        //线程是否启动
        bool started() const { return started_; }

        //进程ID
        pid_t tid() const { return pid_; }

        //线程名字
        string name() const { return name_; }

        //已经创建的线程的个数
        static int numCreated() { return numCreated_.get(); }
    private:
        //如果没有给线程分配名字，则自动生成名字
        void setDefaultName();
    };
}


#endif //STL_THREAD_H
