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

    //成员函数
    class MemberFuncObj{
    private:
        string s = "<in class MemberFuncObj>: ";
    public:
        void memberFunc(const string& str){
            this_thread::sleep_for(chrono::seconds (1));
            cout << s + str << endl;
        }
    };


    //函数对象
    class FuncObj{
    private:
        string s = "<in class FuncObj>: ";
    public:
        //单参数函数调用
        void operator() (const string& str){
            cout << s + str << endl;
        }

        //双参数函数调用
        void operator()(const string& str1, const string& str2){
            cout << s + str1 + str2 << endl;
        }
    };

    //普通函数
    int normalFunc(const string& str){
        this_thread::sleep_for(chrono::seconds (1));
        cout << "<in normalFunc>: " << str << endl;
        return 0;
    }


/*
 *  async()指定为立即启动方式，启动callable objet: 普通函数、成员函数、函数对象、lamda
 *  async()指定为立即启动方式，如果不能立即启动，会抛异常
 *  get()返回的结果也有可能抛异常
 */
void async_future(){
    MemberFuncObj mfobj;
    FuncObj funcObj;

    std::future<int>  f1;
    std::future<void> f2;
    std::future<void> f3;
    std::future<void> f4;
    std::future<void> f5;

    //尝试在独立分离的线程内启动一个callable object: 普通函数, lamda, 成员函数, 函数对象
    try{
       f1 = std::async(std::launch::async, normalFunc, "普通函数");                             //普通函数
       f2 = std::async(std::launch::async, &MemberFuncObj::memberFunc, mfobj, "成员函数");      //成员函数
       f3 = std::async(std::launch::async, funcObj, "函数对象单参数");                           //函数对象
       f4 = std::async(std::launch::async, funcObj, "hello", "world");                        //函数对象
       f5 = std::async(std::launch::async,[]{normalFunc("lamda");});                   //lamda

    }catch(const exception& e){
        e.what();
    }

    try{
        f1.get();
        f2.get();
        f3.get();
        f4.get();
        f5.get();
        std::cout << "-----------end------------" << std::endl;

    }catch(const exception& e){
        e.what();
    }
}


/*
 *  线程指定为std::launch::async启动方式，如果线程已启动，则在离开future object作用域之前会阻塞，直到后台任务完成
 */
void future_scope1(){
    std::future<void> f = std::async(std::launch::async, []{
        while(true){
            normalFunc("noraml");
        }
    });
}


/*
 * 线程以std::launch::async方式启动，如果没有将返回值赋值给future object
 * 调用者会在此停滞直到目标函数结束, 相当于一个同步调用.
 * 还是会在一个分离线程内执行，但是会阻塞，直到该线程执行完毕，期间不会切换到其它线程执行
 */
void future_scope2(){
    std::cout << "in main thread: " << this_thread::get_id() << std::endl;
    std::async(std::launch::async, []{
        std::cout << "in subthread: " << this_thread::get_id() << std::endl;
        for(int i = 0; i < 5; ++i){
            normalFunc("normal");
            std::this_thread::yield();
        }
    });

    std::cout << "in main thread: " << this_thread::get_id() << std::endl;
    std::cout << "main thread end...." << std::endl;
}


/*
 * 启动线程后，等待线程结束，或查询线程的执行状态
 */
void future_wait(){
    std::future<void> f = std::async(std::launch::async, []{
       for(int i = 0; i < 5; i++){
           normalFunc("normal");
       }
    });

    std::future_status fs;
    while(true){
        fs = f.wait_for(std::chrono::seconds(1));
        switch(fs){
            case std::future_status::deferred:
                std::cout << "==>defered" << std::endl;
                break;
            case std::future_status::timeout:
                std::cout << "==>timeout" << std::endl;
                break;
            case std::future_status::ready:
                std::cout << "==>ready" << std::endl;
                return;
            default:
                std::cout << "==>unkown" << std::endl;
        }
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
void future_shared(){
    std::shared_future<int> f = std::async(normalFunc, "hhhh");
    std::shared_future<int> f1 = std::async(normalFunc, "llll").share();

    cout << f.get() << endl;
    cout << f.get() << endl;
}


/*
 * thread的基本用法:
 *      调用thread时，如果不能立即启动则会抛出异常
 *      如果在thread执行的例程中抛出异常，但是没有被捕获，则会导致程序结束
 * 使用thread时需要try-catch, 在thread启动的函数内部也需要try-catch.
 */
void thread(){
    try{
        std::thread thread1([]{
            try{
                normalFunc("noraml");
            }catch(const exception& e){
                std::cout << e.what() << std::endl;
            }
        });
        std::cout << "join thread...." << std::endl;
        thread1.join();
    }catch(const exception& e){
        std::cout << e.what() << std::endl;
    }
}


/*
 * 使用thread启动线程的几种方式
 * 想要等待线程结束则调用join(), 将它自母体卸离使它运行于后台而不受任何控制则调用detach(),
 * 必须在thread object寿命结束前这么做, 否则程序会终止并调用std::terminate().
 */
void thread_callableObj(){
    MemberFuncObj mfobj;
    FuncObj fobj;

    try{
        std::thread f1(normalFunc, "普通函数");                            //普通函数
        std::thread f2(&MemberFuncObj::memberFunc, mfobj, "成员函数");    //成员函数
        std::thread f3(fobj, "函数对象单参数");                            //函数对象
        std::thread f4(fobj, "hello", "world");                         //函数对象
        std::thread f5([]{normalFunc("lamda");});                   //lamda

        f1.join();
        f2.join();
        f3.join();
        f4.join();
        f5.join();
        std::cout << "---------end---------" << std::endl;

    }catch(const exception& e){
        cout << e.what() << endl;
    }
}


/*
 * 如果线程的例程在执行过程中抛出异常，但是未被捕获，会导致程序结束
 */
void thread_exception(){
    std::thread thread1([]{
        throw std::runtime_error(string("runtime_error_ssss"));
    });
    while(true){
        normalFunc("noraml");
    }
}

/*
 * 在thread object的寿命结束前不调用join()或detach()的其中一个会导致程序结束
 */
void thread_joinDetach(){
    std::thread thread1([]{
        while(true){
            normalFunc("noraml");
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(6));
    std::cout << "thread_joinDetach end....." << std::endl;
}


/*
 * main函数结束时，所有后台线程（detached）都会被强制结束
 */
void thread_detached(){
        std::thread thread1([]{
            while(true){
                normalFunc("noraml");
            }
        });
        thread1.detach();
        std::this_thread::sleep_for(std::chrono::seconds(6));
        std::cout << "thread_joinDetach end....." << std::endl;
}


void doSomething(std::promise<std::string>& p){
    try{
        std::cout << "read char ('x' for exception): " << std::endl;
        char c = std::cin.get();
        if(c == 'x'){
            throw std::runtime_error(std::string("char ") + c + " read");
        }
        string s = std::string("char ") + c + " processed";
        p.set_value_at_thread_exit(std::move(s));  //线程安全操作
    }catch(...){
        p.set_exception_at_thread_exit(std::current_exception());  //线程安全操作
    }
}


/*
 * class promise用来设置结果值或者异常
 * promise的所有用来设置数值或者异常的成员函数都是线程安全的
 * copying不适用于promise.
 */
void promise(){
    try{
        std::promise<string> p;
        std::future<string> f(p.get_future());
        std::thread t(doSomething, std::ref(p));

        /*
         * get()会导致block直到shared state成为ready，当promise的set_value()或set_exception()执行后便是如此。
         * 但是这并不意味着设定此promise的线程已经结束，该线程可能仍执行其它语句，甚至存储其它结果放进其它promise内。
         * 如果想令shared state在线程确实结束时变为ready,则必须调用
         * set_value_at_thread_exit(),或set_exception_at_tread_exit()
         */
        std::cout << "result: " << f.get() << std::endl;
        t.join();
    }catch(...){
        std::cout << "exception: " << std::endl;
    }
}


}




















