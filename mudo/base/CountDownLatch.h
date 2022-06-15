//
// Created by 78472 on 2022/6/15.
//

#ifndef STL_COUNTDOWNLATCH_H
#define STL_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>

using namespace std;

class CountDownLatch {
private:
   mutex mutex_;
   condition_variable condition_;
   int count_;
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount();
};


#endif //STL_COUNTDOWNLATCH_H
