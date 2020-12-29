//
// Created by lhy on 2020/12/26.
//
#include <thread>
#include <mutex>
#include <iostream>
#include <future>
#include <chrono>
#include <exception>


using namespace std;

std::mutex printMutex;

namespace mthread{

    class Access{
    private:
        std::recursive_mutex rmutex;

    public:
        void func1(){
            if(!rmutex.try_lock()){
                cout << "try lock false" << endl;
            }else{
                std::lock_guard<std::recursive_mutex> l(rmutex, std::adopt_lock);
                cout << "----------" << endl;
                return;
            }

            std::lock_guard<recursive_mutex> l(rmutex);
            cout << "----------" << endl;
        }

        void func2(){
            std::lock_guard<recursive_mutex> l(rmutex);
            func1();
            this_thread::yield();
            this_thread::sleep_for(chrono::milliseconds(10000));
            cout << "**********" << endl;

        }

    };


    void stringPrint(const string& s){
        std::lock_guard<mutex> l(printMutex);
        for(char c : s){
            this_thread::sleep_for(chrono::milliseconds(10));
            cout << c;
        }
        cout << endl;
    }


/*
 * 锁的一个示例： 顺序打印字符串,不会出现字符顺序混乱
 */
    void mutextest(){

        cout << "-----" << endl;
        future<void> f = async(stringPrint, "this is the first thread");
        future<void> f1 = async(stringPrint, "this is the second thread");

        cout << "the main thread" << endl;
    }


/*
 * 循环锁： 循环锁允许同一线程多次锁定, 并在最后一次unlock()时释放lock.
 */
    void recursive_mutextest(){

       Access ac;

       auto f = std::async(&Access::func2, &ac);
       auto f1 = std::async(&Access::func1, &ac);
    }


/*
 * 保证离开某段作用域后会自动解锁, 不管是因为异常或者生命周期正常结束
 *
 * lock_guard lg(m, std::adopt_lock) : 为已经被锁定的mutex m建立一个lock guard.
 *
 * lock_guard对象生命周期接收时, 即使它拥有的锁已经被提前释放调, 也不会产生异常。
 *
 * 本例中：f线程里首先获得锁, 然后锁被过寄给l, l生命周期结束后释放锁, 然后回到f线程中再次获得锁, g生命周期结束时将锁释放掉
 */
    void lockGuard(){

        std::mutex s;
        string str{"hello"};

        try{
            auto f = std::async(std::launch::async, [&]{
               std::lock_guard<mutex> g(s);
               cout << "in lamda" << endl;
               this_thread::sleep_for(chrono::milliseconds(10000));
               if(s.try_lock()){
                   cout << "lock again" << endl;
               }
               cout << "end lamda" << endl;

            });

           this_thread::sleep_for(chrono::milliseconds(100));
           if(!s.try_lock()){
               cout << "mutex has already locked" << endl;
           }

           {
                std::lock_guard<mutex> l(s, std::adopt_lock);
                cout << "----------" << endl;
           }

           while(1);

        }catch(const exception& e){
            cout << e.what() << endl;
        }

    }


/*
 * 保证即使发生异常, 相应的锁依然会被unlock()
 * std::lock_guard()
 *      即使以异常的方式结束生命周期, 依然会解锁所拥有的锁
 *      原因：{}作用域, 生命周期结束时会调用析构函数, 析构函数会对所拥有的锁进行unlock.
 *
 */
    void lockGuard1(){

        std::mutex s;
        int i = 0;

        try{
            std::lock_guard<mutex> l(s);
            __throw_length_error("sss");
            cout << "----" << endl;
        }catch(const exception& e){
            cout << e.what() << endl;
        }

        if(s.try_lock())
            cout << "****" << endl;
    }


/*
 * 同时锁定多个锁
 * std::lock():
 *      会锁住它收到的所有mutex, 而且阻塞直到所有mutex都被锁定或直到抛出异常。
 *      如果是后者, 已被锁定的mutex都会被解锁.
 *      注： std::lock(a, a)在此次测试中没有抛出异常而是被阻塞。
 *
 * std::try_lock():
 *      尝试取得多个lock, 若并非所有lock可用也不至于造成阻塞
 *      在取得所有lock的情况下返回-1, 否则返回第一个失败的lock索引（从0开始）, 如果失败所有成功的lock会又被解锁
 *
 * 本例中： f线程中将printMutex和writeMutex锁定后, 必须把这两个锁过寄给lock_guard对象, 否则即使线程结束这2个锁依旧是被锁定的。
 *
 */
    void multilock(){

        string str = "sss";
        std::mutex printMutex;
        std::mutex writeMutex;

        try{
            auto f = async(std::launch::async, [&]{
                try{
                    std::lock(printMutex, writeMutex);
                    std::lock_guard<mutex> l(printMutex, std::adopt_lock);
                    std::lock_guard<mutex> g(writeMutex, std::adopt_lock);
                    str = "...inginging";
                    cout << "str: " << str << endl;

                }catch (const exception& e){
                    cout << e.what() << endl;
                }
            });

            this_thread::sleep_for(chrono::milliseconds(1000));

            {
                std::lock_guard<mutex> l(printMutex);
                cout << "__str: " << str << endl;
            }

        }catch(const exception& e){
            cout << e.what() << endl;
        }

    }

}
