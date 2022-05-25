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

   std::queue<int> queue;
   std::mutex queueMutex;
   std::condition_variable queueCondVar;

/*
 * 一个notify_one通知, 随机唤醒一个wait.
 * 一个notify_all通知, 唤醒所有等待的wait.
 * 通知都是只起一次作用, 不会重复起作用.
 */
   void provider(int val){
       for(int i = 0; i < 6; i++){
           {
               std::lock_guard<std::mutex> lg(queueMutex);
               queue.push(val + i);
               if(i < 4)
                   queueCondVar.notify_all();   //在保护区间内最好只做必要的操作, notify通知在外围操作
               cout << "****" << endl;
           }
           this_thread::sleep_for(chrono::milliseconds(val));
       }

       {
           this_thread::sleep_for(chrono::milliseconds(30000));
           std::lock_guard<std::mutex> lg(queueMutex);
           queue.push(1000);
           queueCondVar.notify_one();   //在保护区间内最好只做必要的操作, notify通知在外围操作
           cout << "****" << endl;
       }

   }


/*
 * 1. 即使在wait函数之前, 其它线程已经发送notify通知, 该通知也不会丢失, 当执行到wait函数时依然会接收到该通知.
 * 2. wait函数之后的操作在unique_lock的限定的保护区间内进行
 */
    void consumer(int num){
       while(true){
           int val;
           {
               this_thread::sleep_for(chrono::milliseconds(5000));
               std::unique_lock<std::mutex> ul(queueMutex);
               queueCondVar.wait(ul, []{ return !queue.empty();});  //被唤醒且满足判断条件则跳出该函数, 否则程序被阻塞在这里.
               cout << "$$$$" << endl;
               val = queue.front();
               queue.pop();
               std::cout << "consumer " << num << ": " << val << std::endl;
           }

       }
   }

   void conVarTest(){
       auto p1 = std::async(std::launch::async, provider, 100);
       auto p2 = std::async(std::launch::async, provider, 200);
       auto p3 = std::async(std::launch::async, provider, 300);

       auto c1 = std::async(std::launch::async, consumer, 1);
       auto c2 = std::async(std::launch::async, consumer, 2);

   }











}