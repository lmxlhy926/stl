//
// Created by 78472 on 2022/6/15.
//

#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count) : count_(count) {}

void CountDownLatch::wait() {
    std::unique_lock<std::mutex> ul(mutex_);
    while(count_ > 0)
        condition_.wait(ul);
}

void CountDownLatch::countDown() {
    std::lock_guard<std::mutex> lg(mutex_);
    --count_;
    if(count_ == 0)
        condition_.notify_all();
}

int CountDownLatch::getCount(){
    std::lock_guard<std::mutex> lg(mutex_);
    return count_;
}
