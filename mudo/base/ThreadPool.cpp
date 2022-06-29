//
// Created by 78472 on 2022/6/21.
//

#include "ThreadPool.h"
#include <exception>
#include <cstdio>
#include <utility>
#include <cstdlib>
#include <cassert>


muduo::ThreadPool::ThreadPool(string threadName)
    :   threadPoolName_(std::move(threadName)),
        maxTaskQueueSize_(0),
        running_(false){}

muduo::ThreadPool::~ThreadPool() {


}

void muduo::ThreadPool::start(int numThreads) {
    assert(threads_.empty());       //重复调用start会结束进程
    running_ = true;                //运行标志
    threads_.reserve(numThreads);
    for(int i = 0; i < numThreads; i++){
        char id[32];
        snprintf(id, sizeof id, "%d", i + 1);
        threads_.emplace_back(new muduo::Thread([this](){
            runInThread();
        }, threadPoolName_ + id));
        threads_[i]->start();       //启动线程
    }
    if(numThreads == 0 && threadInitCallback_){
        threadInitCallback_();
    }
}

void muduo::ThreadPool::run(muduo::ThreadPool::Task func) {

}

/**
 * 唤醒所有等待线程，等待线程例程执行完毕，退出。
 * 未结束线程例程继续执行，直到执行完毕。
 *
 * taskQueueNotEmpty_:
 *      notify_one(): 任务列表中加入一个任务函数时触发：run()
 *      wait()：取任务函数时，任务列表里没有函数，则等待: take()
 * taskQueueNotFull_:
 *      notify_one(): 从任务列表中取走一个函数时触发：take()
 *      wait(): 任务列表满，无法将函数放入列表，则等待：run()
 */
void muduo::ThreadPool::stop() {
    std::lock_guard<std::mutex> lg(mutex_);
    running_ = false;
    taskQueueNotEmpty_.notify_one();
    taskQueueNotFull_.notify_one();
    for(auto& thread : threads_){
        thread->join();
    }
}

size_t muduo::ThreadPool::taskQueueSize() {
    std::lock_guard<std::mutex> lg(mutex_);
    return taskQueue_.size();
}

bool muduo::ThreadPool::isFull() {
    std::lock_guard<std::mutex> lg(mutex_);
    return maxTaskQueueSize_ > 0 && taskQueue_.size() >= maxTaskQueueSize_;
}

/**
 * 线程例程：
 * 1. 执行初始化函数
 * 2. 循环：获取任务函数，执行函数。   如果running_为false，则线程例程执行完毕
 */
void muduo::ThreadPool::runInThread() {
    try{
        //执行一次初始化函数
        if(threadInitCallback_){
            threadInitCallback_();
        }
        while(running_){    //running_为false，则线程例程执行完毕
            Task task(take());
            if(task){   //获取到任务，则执行
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
 * 如果是stop()导致的唤醒，则返回的task没有任何执行机能
 */
muduo::ThreadPool::Task muduo::ThreadPool::take() {
    std::unique_lock<std::mutex> ul(mutex_);
    //条件：任务列表非空，或者running_为false则退出等待
    taskQueueNotEmpty_.wait(ul, [this]() -> bool{
        return !taskQueue_.empty() || !running_;
    });
    Task task;
    if(!taskQueue_.empty()){                    //任务列表非空导致的唤醒
        task = taskQueue_.front();              //取出执行函数
        taskQueue_.pop_front();                 //弹出占用内存
        if(maxTaskQueueSize_ > 0){
            taskQueueNotFull_.notify_one();     //任务列表非满
        }
    }
    return task;    //task有可能没有任何执行机能
}


