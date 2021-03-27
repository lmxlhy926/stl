//
// Created by lhy on 2021/3/27.
//

#ifndef STL_THREADPOOL_HPP
#define STL_THREADPOOL_HPP

#include <cassert>
#include <functional>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <map>

using namespace std;

namespace mthread{

    class TaskQueue {
    public:
        TaskQueue() = default;
        virtual ~TaskQueue() = default;

        virtual void enqueue(std::function<void()> fn) = 0;
        virtual void shutdown() = 0;

        virtual void on_idle(){};
    };


    class ThreadPool : public TaskQueue {
    public:
        explicit ThreadPool(size_t n) : shutdown_(false) {
            while (n) {
                threads_.emplace_back(worker(*this));   //启动函数对象于独立线程中
                n--;
            }
        }

        ThreadPool(const ThreadPool &) = delete;
        ~ThreadPool() override = default;

        void enqueue(std::function<void()> fn) override {
            std::unique_lock<std::mutex> lock(mutex_);
            jobs_.push_back(std::move(fn));
            cond_.notify_one();
        }

        void shutdown() override {
            // Stop all worker threads...
            {
                std::unique_lock<std::mutex> lock(mutex_);
                shutdown_ = true;
            }

            cond_.notify_all();

            // Join...
            for (auto &t : threads_) {
                t.join();
            }
        }

    private:
        struct worker {
            explicit worker(ThreadPool &pool) : pool_(pool) {}

            void operator()() {
                for (;;) {
                    std::function<void()> fn;
                    {
                        std::unique_lock<std::mutex> lock(pool_.mutex_);

                        pool_.cond_.wait(
                                lock, [&] { return !pool_.jobs_.empty() || pool_.shutdown_; });

                        if (pool_.shutdown_ && pool_.jobs_.empty()) { break; }

                        fn = pool_.jobs_.front();
                        pool_.jobs_.pop_front();
                    }

                    assert(true == static_cast<bool>(fn));
                    fn();
                }
            }

            ThreadPool &pool_;
        };

        /*
         * 友元类，可以操作私有成员
         */
        friend struct worker;

        /*
         * 如何将函数启动于独立线程
         * 如何回收线程（阻塞回收）
         */
        std::vector<std::thread> threads_;

        /*
         * 函数外覆器
         */
        std::list<std::function<void()>> jobs_;

        bool shutdown_;

        /*
         * 条件变量
         * 越过条件变量的条件：
         *      受到唤醒
         *      获得锁
         *      满足条件
         *
         *      没有收到唤醒则阻塞
         *      受到唤醒后没有获得锁则阻塞
         *      收到唤醒，获得锁，没有满足条件，则放弃锁并且继续阻塞
         *
         * 通知： 通知某一个，通知所有
         */
        std::condition_variable cond_;

        /*
         * 独占性操作： 占有锁，释放锁
         */
        std::mutex mutex_;
    };


    void func(string str){
        for(int i = 0; i <= 3; i++){
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << str + "----" << endl;
        }

    }

    void threadPoolTest(){

    }

    class X{
    public:
        void operator()(string s){
            for(int i = 0; i <= 3; i++){
                this_thread::sleep_for(chrono::milliseconds(1000));
                cout << s + "in operator" << endl;
            }
        }
    };

    void threadPoolTest1(){
        X x;
        std::map<string, std::function<void(string)>> m;
        m.insert(std::pair<string, std::function<void(string)>>("hello", func));
        m.insert(std::pair<string, std::function<void(string)>>("world", x));
        auto fn = m.find("hello")->second;
        auto fn1 = m.find("world")->second;

        ThreadPool pool(10);
        pool.enqueue([&]{ fn("helloworld");});
        pool.enqueue([&]{ fn1("sssss");});
    }




}


























#endif //STL_THREADPOOL_HPP
