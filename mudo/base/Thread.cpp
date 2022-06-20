//
// Created by 78472 on 2022/6/16.
//

#include "Thread.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <utility>

using namespace muduo;

struct ThreadData{
    typedef muduo::Thread::ThreadFunc ThreadFunc;
    ThreadFunc          func_;      //执行函数
    string              name_;      //线程名
    pid_t*              pid_;       //进程号
    CountDownLatch*     latch_;

    ThreadData(ThreadFunc func,
               string  name,
               pid_t* pid,
               CountDownLatch* latch)
        :   func_(std::move(func)),
            name_(std::move(name)),
            pid_(pid),
            latch_(latch){}

    //执行函数
    void runInThread(){
        //减一，标识已经开始执行函数
        latch_->countDown();
        latch_ = nullptr;

        try{
           func_();
        }catch (const std::exception& ex){
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
        }catch(...){
            fprintf(stderr, "unkonwn exception caught in Thread %s\n", name_.c_str());
        }
    }
};

//线程执行函数
void* startThread(void *obj){
    auto* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return nullptr;
}

AtomicInt32  Thread::numCreated_;

muduo::Thread::Thread(muduo::Thread::ThreadFunc func, string name)
    :   started_(false),
        joined_(false),
        pthreadId_(nullptr),
        pid_(0),
        func_(std::move(func)),
        name_(std::move(name)),
        latch_(1)
{
    setDefaultName();
}

muduo::Thread::~Thread() {
    if(started_ && !joined_)
        pthread_detach(pthreadId_);
}

void muduo::Thread::start() {
    if(started_)    return;
    started_ = true;
    auto* data = new ThreadData(func_,name_, &pid_, &latch_);
    if(pthread_create(&pthreadId_, nullptr, startThread, data)){
        started_ = false;
        delete data;
        std::cout << "Failed in pthread_create" << std::endl;
    }else{
        //开始执行函数后返回
        latch_.wait();
    }
}

int muduo::Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, nullptr);
}

void muduo::Thread::setDefaultName() {
    int num = numCreated_.incrementAndGet();
    if(name_.empty()){
        char buf[32];
        int size = snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = string(buf, size);
    }
}
