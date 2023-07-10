//
// Created by 78472 on 2022/6/15.
//

#ifndef STL_COUNTDOWNLATCH_H
#define STL_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>

using namespace std;

/*
 * 阻塞等待，知道count_减为0
 */
class CountDownLatch {
private:
   mutex mutex_;
   condition_variable condition_;
   int count_;
public:
    explicit CountDownLatch(int count);

    //count_为0时，解除等待
    void wait();

    void countDown();

    int getCount();
};


#endif //STL_COUNTDOWNLATCH_H
