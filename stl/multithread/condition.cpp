//
// Created by lhy on 2021/1/2.
//

#include "condition.h"
#include <condition_variable>
#include <mutex>
#include <future>
#include <thread>
#include <iostream>
#include <queue>


using namespace std;

namespace mthread{

   std::queue<int> queue;                   //共享变量
   std::mutex queueMutex;                   //保护锁
   std::condition_variable queueCondVar;    //同步机制

/*
 * 一个notify_one通知, 随机唤醒一个wait.
 * 一个notify_all通知, 唤醒所有等待的wait.
 * 唤醒的是挂在这个等待唤醒列表中的线程, 所以要想被唤醒，当通知来临时，此线程必须已经挂在该等待列表中。
 */
   void provider(){
       {
           std::lock_guard<std::mutex> lg(queueMutex);
           std::cout << "----first notify----" << std::endl;
       }
       queueCondVar.notify_one();   //在保护区间内最好只做必要的操作, notify通知在外围操作
       this_thread::sleep_for(chrono::seconds (5));

        {
            std::lock_guard<std::mutex> lg(queueMutex);
            std::cout << "----second notify----" << std::endl;
        }
        queueCondVar.notify_one();   //在保护区间内最好只做必要的操作, notify通知在外围操作
   }


/*
 * . wait函数之后的操作在unique_lock的限定的保护区间内进行
 */
    void consumer(){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::unique_lock<std::mutex> ul(queueMutex);
        std::cout << "----start to wait----" << std::endl;
        queueCondVar.wait(ul);  //被唤醒且满足判断条件则跳出该函数, 否则程序被阻塞在这里.
        std::cout << "----wake up----" << std::endl;
   }

   void conVarTest(){
       auto p1 = std::async(std::launch::async, provider);
       auto c1 = std::async(std::launch::async, consumer);
       p1.get();
       c1.get();
   }


   //在进行wait之前首先会检查条件是否满足，如果满足则不会进行wait。
    void conVarTest1(){
        std::mutex mutex_;
        std::condition_variable cv;
        std::unique_lock<std::mutex> ul(mutex_);
        cv.wait(ul, [](){ return true;});
        std::cout << "---no wait---" << std::endl;
    }

}