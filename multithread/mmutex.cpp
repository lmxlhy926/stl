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
            std::lock_guard<recursive_mutex> l(rmutex);
            cout << "-----in func1-----" << endl;
        }

        void func2(){
            std::lock_guard<recursive_mutex> l(rmutex);
            cout << "-----in func2-----" << endl;
            func1();
            this_thread::sleep_for(chrono::milliseconds(4000));
            cout << "**********" << endl;

        }

        void func3(){
            std::lock_guard<recursive_mutex> l(rmutex);
            cout << "------in func3-------" << endl;
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
 * 锁的应用：多线程时, 保证一段时间内的独占式访问
 * 本例：
 *      顺序打印字符串,不会出现字符顺序混乱
 */
    void mutextest(){

        cout << "-----" << endl;
        future<void> f = async(stringPrint, "this is the first thread");
        future<void> f1 = async(stringPrint, "this is the second thread");

        cout << "the main thread" << endl;
    }


/*
 * 循环锁： 循环锁允许同一线程多次锁定, 并在最后一次unlock()时释放lock.
 * 本例：
 *      在func2的线程中锁被多次锁定, 只有func2中完全将锁释放出来, func3的线程才能获得到锁.
 */
    void recursive_mutextest(){

       Access ac;

       auto f = std::async(std::launch::async, &Access::func2, &ac);
       auto f1 = std::async(std::launch::async, &Access::func3, &ac);
    }


/*
 * 同一把锁可以传递给多个std::lock_guard对象, 自己也可以执行lock, unlock操作
 *
 * lock_guard lg(m, std::adopt_lock) : 为已经被锁定的mutex m建立一个lock guard.
 * lock_guard对象生命周期结束时, 即使它拥有的锁已经被提前unlock(), 也不会产生异常。
 *
 * 本例中：
 *      f线程里首先获得锁, 然后lock的锁被过寄给l, l生命周期结束后释放锁, 回到f线程中s锁再次锁定, g生命周期结束时将s锁释放掉
 */
    void lockGuard(){

        std::mutex s;
        string str{"hello"};

        try{
            auto f = std::async(std::launch::async, [&]{
               std::lock_guard<mutex> g(s);
               cout << "in lamda" << endl;
               this_thread::sleep_for(chrono::milliseconds(1000));

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

           this_thread::sleep_for(chrono::milliseconds(2000));
           if(s.try_lock())
               cout << "mutex has been unlocked" << endl;

        }catch(const exception& e){
            cout << e.what() << endl;
        }

    }


/*
 * 保证即使发生异常, 相应的锁依然会被unlock()
 * std::lock_guard()
 *      即使以异常的方式结束生命周期, 依然会解锁所拥有的锁
 *      原因：{}作用域, std::lock_guard对象生命周期结束时会调用析构函数, 析构函数会对其所拥有的锁进行unlock.
 *
 * 本例：
 *      创建l对象时会对s进行lock, l的生命周期结束时其析构函数会对l所拥有的锁进行unlock, 因此s.try_lcok()又会将s锁住.
 */
    void lockGuard1(){

        std::mutex s;
        int i = 0;

        try{
            std::lock_guard<mutex> l(s);
            __throw_length_error("sss");

        }catch(const exception& e){
            cout << e.what() << endl;
        }

        if(s.try_lock())
            cout << "****" << endl;
    }

/*
 * std::lock_guard中指定std::adopt_lock后就不会对传入其中的锁进行lock()操作, 主要应用是用来释放被锁定的锁。
 * 不指定std::adopt_lock则在自身构造时对传入的锁执行lock()操作, 在生命周期结束析构时对其执行unlock()操作.
 *
 */
    void lockGuard2(){

        std::mutex s;
        try{
//            std::lock_guard<mutex> g(s);
//            cout << "-----" << endl;
//            std::lock_guard<mutex> l(s, std::adopt_lock);
//            cout << "*****" << endl;
            s.unlock();
            s.unlock();
            cout << "ssss" << endl;

        }catch(const exception& e){
            cout << e.what() << endl;
        }

    }


/*
 * 同时锁定多个锁
 * std::lock():
 *      会锁住它收到的所有mutex, 而且阻塞直到所有mutex都被锁定或直到抛出异常。如果是后者, 已被锁定的mutex都会被解锁.
 *      注： std::lock(a, a)在此次测试中没有抛出异常而是被阻塞。
 *
 * std::try_lock():
 *      尝试取得多个lock, 若并非所有lock可用也不至于造成阻塞.
 *      在取得所有lock的情况下返回-1, 否则返回第一个失败的lock索引（从0开始）, 如果失败,所有成功的lock会又被解锁
 *
 * 本例中：
 *      f线程中将printMutex和writeMutex锁定后, 必须把这两个锁过寄给lock_guard对象, 否则即使线程结束这2个锁依旧是被锁定的。
 *      本例中未将printMutex过寄给std::lock_guard对象, 因此printMutex是一直被锁定的
 *
 */
    void multilock(){

        string str = "sss";
        std::mutex printMutex;
        std::mutex writeMutex;

        try{
            auto f = async(std::launch::async, [&]{
                try{
                    if(std::try_lock(printMutex, writeMutex) == -1){
//                        std::lock_guard<mutex> l(printMutex, std::adopt_lock);
                        std::lock_guard<mutex> g(writeMutex, std::adopt_lock);

                        str = "...inginging";
                        cout << "str: " << str << endl;
                    }

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



/*
 * class unique_lock的主要优点：
 *      如果析构时mutex仍被锁住, 则其析构函数会自动调用unlock(). 如果当时没有锁住mutex, 则析构函数不做任何事
 *      拥有mutex的全功能调用： lock(), try_lock(), try_lock_for(), try_lock_until(), unlock()
 *      可以通过调用owns_lock()或bool()来查询其mutex目前是否被锁住
 *
 * unique_lock对象执行lock操作可能抛出异常
 *      1. 线程特权级不足以执行此操作
 *      2. 如果平台侦测到有个deadlock即将发生
 *      3. mutex已经被锁定但是又无法形成阻塞
 *      如果程序解除一个并非它所拥有的mutex object, 或是销毁一个被任何线程拥有的mutex object,
 *      或是线程拥有mutex object但是却结束了生命, 将导致不明确的行为
 *
 * unique_lock对象调用unlock()可能抛出异常, 如果这个unique lock并未被锁住
 *
 * mutex传递给class unique_lock后, 统一使用unique_lock对象来调用相应的函数
 *
 */

    void uniqueLock(){

        std::timed_mutex mt;
        std::mutex m1;
        std::mutex m2;

        {
            std::unique_lock<timed_mutex> l(mt, std::try_to_lock);
            if (l) {
                cout << "--lock--" << endl;
            }
            if (l.owns_lock()) {
                cout << "--lock--" << endl;
            }
        }


        {
            std::unique_lock<timed_mutex> l1(mt, std::chrono::milliseconds(2000));
            if(l1){
                cout << "**lock**" << endl;
            }
        }


        {
            std::unique_lock<timed_mutex> l2(mt, std::defer_lock);
            l2.lock();
            if(l2)
                cout << "@@@@" << endl;
            l2.unlock();
            if(!l2)
                cout << "@@@@" << endl;
        }

        {
            std::unique_lock<timed_mutex> l3(mt, std::defer_lock);
            if(!l3)
                cout << "$$$$" << endl;
            l3.lock();
            if(l3)
                cout << "$$$$" << endl;
            l3.unlock();
            if(!l3)
                cout << "$$$$" << endl;
        }


        {
            std::unique_lock<mutex> lockm1(m1, std::defer_lock);
            std::unique_lock<mutex> lockm2(m2, std::defer_lock);

            std::lock(m1, m2);
        }

    }







}



















