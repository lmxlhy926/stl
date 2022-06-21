//
// Created by 78472 on 2022/6/21.
//

#include "ThreadPool.h"
#include <exception>
#include <cstdio>

#include <utility>

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

}

size_t muduo::ThreadPool::taskQueueSize() const {
    return 0;
}

bool muduo::ThreadPool::isFull() const {
    return false;
}

void muduo::ThreadPool::runInThread() {
    try{

    }catch(const exception& e){
        fprintf(stderr, "exception caught in ThreadPool %s\n", threadPoolName_.c_str());
        fprintf(stderr, "reason: %s\n", e.what());
    }


}

muduo::ThreadPool::Task muduo::ThreadPool::take() {
    return muduo::ThreadPool::Task();
}
