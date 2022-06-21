//
// Created by 78472 on 2022/6/21.
//

#include "ThreadPool.h"
#include <exception>
#include <cstdio>
#include <utility>
#include <cstdlib>


muduo::ThreadPool::ThreadPool(string threadName)
    :   threadPoolName_(std::move(threadName)),
        maxQueueSize_(0),
        running_(false){}

muduo::ThreadPool::~ThreadPool() {


}

void muduo::ThreadPool::start(int numThreads) {

}

void muduo::ThreadPool::run(muduo::ThreadPool::Task func) {

}

void muduo::ThreadPool::stop() {
    std::lock_guard<std::mutex> lg(mutex_);
    running_ = false;
    taskQueueNotEmpty_.notify_one();
    taskQueueNotFull_.notify_one();
    for(auto& thread : threads_){
        thread->join();
    }
}

size_t muduo::ThreadPool::taskQueueSize() const {
    return 0;
}

bool muduo::ThreadPool::isFull() {
    std::lock_guard<std::mutex> lg(mutex_);
    return maxQueueSize_ > 0 && taskQueue_.size() >= maxQueueSize_;
}

void muduo::ThreadPool::runInThread() {
    try{
        if(threadInitCallback_){
            threadInitCallback_();
        }
        while(running_){
            Task task(take());
            if(task){
                task();
            }
        }
    }catch(const exception& e){
        fprintf(stderr, "exception caught in ThreadPool %s\n", threadPoolName_.c_str());
        fprintf(stderr, "reason: %s\n", e.what());
        abort();

    }catch(...){
        fprintf(stderr, "unknow exception caught in ThreadPool %s\n", threadPoolName_.c_str());
        throw;
    }
}


/*
 * 从任务列表中取出执行机能，如果任务列表为空，则wait()
 */
muduo::ThreadPool::Task muduo::ThreadPool::take() {
    std::unique_lock<std::mutex> ul(mutex_);
    taskQueueNotEmpty_.wait(ul, [this]() -> bool{
        return !taskQueue_.empty() && !running_;
    });

    Task task;
    if(!taskQueue_.empty()){                    //任务列表非空
        task = taskQueue_.front();              //取出执行函数
        taskQueue_.pop_front();                 //弹出占用内存
        if(maxQueueSize_ > 0){
            taskQueueNotFull_.notify_one();     //任务列表非空
        }
    }
    return task;    //task有可能没有任何执行机能
}
