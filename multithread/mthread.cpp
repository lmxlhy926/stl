//
// Created by lhy on 2020/12/18.
//

#include "mthread.h"
#include <thread>
#include <future>
#include <exception>

#include <chrono>
#include <iostream>

using namespace std;

namespace mthread{

    class X{
    private:
        string s = "hello";
    public:
        void func(const string& str, int count){
            for(int i = 0; i < count; i++){
                this_thread::sleep_for(chrono::milliseconds(500));
                cout << s + str << endl;
            }
        }
    };


    int func1(int count){
        for(int i = 0; i < count; i++){
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << "c" << endl;
            if(i == 1)
                this_thread::yield();
        }

        return 1;
    }


    void test(){
        X x;
        std::future<int> f;
        std::future<void> f1;

        try{
           f = std::async(std::launch::async, func1, 3);    //普通函数
           f1 = std::async(std::launch::async, &X::func, x, "world", 1);    //成员函数

        }catch(const exception& e){
            e.what();
        }

       while(!(f.wait_for(chrono::milliseconds(500)) == std::future_status::ready)){    //检查线程状态
           cout << "===>in while" << endl;
       }

    }



    void test2(){
        std::shared_future<int> f = std::async(func1, 2);
        cout << f.get() << endl;
        cout << f.get() << endl;

    }



}




















