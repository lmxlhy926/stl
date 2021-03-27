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
        string s = "xx";
    public:
        void func(const string str){
            for(int i = 0; i < 3; i++){
                this_thread::sleep_for(chrono::milliseconds(500));
                cout << s + str << endl;
            }
        }
    };


    class Y{
    private:
        string s = "yy";
    public:
        void operator() (const string str){
            cout << s + str << endl;
        }

        void operator()(const string str1, const string str2){
            cout << s + str1 + str2 << endl;
        }
    };


    int func(const string str){
        for(int i = 0; i < 3; i++){
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << str << endl;
            if(i == 1)
                this_thread::yield();   //放弃cpu使用权
        }

        return 1;
    }


/*
 * 启动线程，传递参数，获取运行结果，检查线程运行状态
 *
 *      callable object作为一个独立线程启动
 *      给函数传递参数
 *      获取函数运行结果(结果或者异常)
 *      线程运行状态的检查: 未启动, 启动尚未结束, 已结束
 */
    void test(){
        X x;
        Y y;
        std::future<int> f;
        std::future<void> f1;
        std::future<void> f2;
        std::future<void> f3;
        std::future<void> f4;

        try{
           f = std::async(std::launch::async, func, "普通函数");                 //普通函数
           f1 = std::async(std::launch::async, []{func("lamda");});      //lamda
           f2 = std::async(std::launch::async, &X::func, x, "成员函数");         //成员函数
           f3 = std::async(std::launch::async, y, "函数对象单参数");              //函数对象
           f4 = std::async(std::launch::async, y, "hello", "world");            //函数对象

        }catch(const exception& e){
            e.what();
        }

       while(!(f.wait_for(chrono::milliseconds(500)) == std::future_status::ready)){    //检查线程状态
           cout << "===>in while" << endl;
       }

       try{
           cout << "f.get()_result: " << f.get() << endl;   //获取线程函数运行结果

       }catch(const exception& e){
           cout << e.what() << endl;
       }


    }


/*
 * shared future
 *      函数对应的线程只执行一次, 将结果存储下来, 因此调用get()时, 导致相同的结果, 或导致抛出同一个异常.
 *
 *  T future<T>::get()                  搬移后之执行结果,或该结果的一份拷贝
 *  const T& shared_future<T>::get()    返回一个reference, 指向存放于"被共享之shared state"的结果值
 *
 */
    void test2(){
        std::shared_future<int> f = std::async(func, "hhhh");
        std::shared_future<int> f1 = std::async(func, "llll").share();

        cout << f.get() << endl;
        cout << f.get() << endl;
    }


/*
 * 使用thread时需要try-catch, 在thread启动的函数内部也需要try-catch.
 */
    void test3(){
        try{
            std::thread t([]{
                cout << "thread id: " << this_thread::get_id() << endl;
                try{
                    int i{};
                    while(i++ < 3){
                        this_thread::sleep_for(chrono::milliseconds(1000));
                        cout << "hhhh" << endl;
                    }
                }catch(const exception& e){
                    cout << e.what() << endl;
                }
            });

            t.detach();
            cout << "end of test3" << endl;
        }catch(const exception& e){
            cout << e.what() << endl;
        }
    }


/*
 * 想要等待线程结束则调用join(), 将它自母体卸离使它运行于后台而不受任何控制则调用detach(), 必须在thread object寿命结束前这么做,
 * 否则程序会终止并调用std::terminate().
 *
 * 使用thread启动线程的几种方式
 */
    void test4(){
        X x;
        Y y;

        try{
            std::thread f(func, "普通函数");                //普通函数
            std::thread f1([]{func("普通函数");});      //lamda
            std::thread f2(&X::func, x, "成员函数");        //成员函数
            std::thread f3(y, "函数对象单参数");            //函数对象
            std::thread f4(y, "hello", "world");          //函数对象

            f.join();
            f1.join();
            f2.join();
            f3.join();
            f4.detach();

        }catch(const exception& e){
            cout << e.what() << endl;
        }
    }




}




















