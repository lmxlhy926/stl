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

   /*
    * recursive_mutex的经典应用：
    *       每个成员函数都使用了锁，成员函数之间会互相调用，导致锁的重复调用
    *       支持同一线程内锁的多次锁定
    */
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
            this_thread::sleep_for(chrono::seconds (1));
            cout << "**********" << endl;

        }

        void func3(){
            std::lock_guard<recursive_mutex> l(rmutex);
            cout << "------in func3-------" << endl;
        }
    };


    /*
     * 线程安全函数，打印时不会出现乱序
     * std::lock_guard的典型应用
     */
    void stringPrint(const string& s){
        std::lock_guard<mutex> l(printMutex);
        for(char c : s){
            this_thread::sleep_for(chrono::milliseconds(10));
            cout << c;
        }
        cout << endl;
    }



    /*
     * lock()后调用try_lock()
     * lock()会锁定，锁定后再调用try_lock()返回false.
     */
    void mutex_lock_try_lock(){
        std::mutex mutex_;
        mutex_.lock();
        if(mutex_.try_lock()){
            std::cout << "lock()后调用try_lock()" << std::endl;
        }
    }


    /*
     * try_lock()后再次调用try_lock();
     * 第一次try_lock()锁定后，再次调用try_lock()会返回false.
     */
    void mutex_2tryLock(){
        std::mutex mutex_;
        if(mutex_.try_lock()){
            std::cout << "mutex.try_lock() successfully" << std::endl;
        }
        if(mutex_.try_lock()){
            std::cout << "mutex.try_lock() successfully" << std::endl;
        }
    };


    /*
     * 锁的应用：多线程时, 保证一段时间内的独占式访问
     * 本例：
     *      顺序打印字符串,不会出现字符顺序混乱
     */
    void lockGuard_test(){
        cout << "-----" << endl;
        future<void> f  =  async(stringPrint, "this is the first thread");
        future<void> f1 =  async(stringPrint, "this is the second thread");
        cout << "the main thread" << endl;
    }


    /*
     * 同一把锁可以传递给多个std::lock_guard对象, 自己也可以执行lock, unlock操作
     * 如果析构时mutex仍被锁住，其析构函数会自动调用unlock()。如果当时没有被锁住，则析构函数不做任何事情。
     * lock_guard lg(m, std::adopt_lock) : 为已经被锁定的mutex m建立一个lock guard.
     */
    void lockGuard_test1(){
        std::mutex mutex_;
        std::lock_guard<mutex> lg(mutex_);
        std::lock_guard<mutex> lgadopt(mutex_, std::adopt_lock);
    }

    /*
     * 抛出异常时，离开lock_guard()的作用域，会导致guard对象被析构，从而导致解锁
     */
    void lockGuard_test2(){
        std::mutex s;

        try{
            std::lock_guard<mutex> l(s);
            __throw_length_error("sss");

        }catch(const exception& e){
            cout << e.what() << endl;
        }

        if(s.try_lock())
            cout << "lock again" << endl;
    }


    /*
     * 循环锁： 循环锁允许同一线程多次锁定, 并在最后一次unlock()时释放lock.
     * 本例：
     *      在func2的线程中锁被多次锁定, 只有func2中完全将锁释放出来, func3的线程才能获得到锁.
     */
    void recursive_mutex_test(){
       Access ac;
       auto f1 =  std::async(std::launch::async, &Access::func2, &ac);
       auto f2 =  std::async(std::launch::async, &Access::func3, &ac);
       f1.get();
       f2.get();
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
 *      f线程中将printMutex和writeMutex锁定后, 必须把这两个锁过寄给lock_guard对象, 否则即使线程结束这2个锁依旧是被锁定的,因为没有显示调用unlock()。
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
                        /*
                         * 锁定后，将锁过继给lock_guard对象，保证在离开guard对象作用域后解除锁定
                         * 这里try_lock()锁定2个锁后，如果不过继给lock_guard对象，或者不显示调用unlock(), 则不过自动解锁
                         */
                        std::lock_guard<mutex> l(printMutex, std::adopt_lock);
                        std::lock_guard<mutex> g(writeMutex, std::adopt_lock);

                        str = "...inginging";
                        cout << "str: " << str << endl;
                    }
                }catch (const exception& e){
                    cout << e.what() << endl;
                }
            });

            this_thread::sleep_for(chrono::seconds (1));

            {
                std::lock_guard<mutex> l(printMutex);
                cout << "str: " << str << endl;
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
 */
    void uniqueLock(){

        std::timed_mutex mt;    //带时间属性的锁，额外支持try_lock_for(), try_lock_until()
        std::mutex m1;
        std::mutex m2;

        {
            //尝试锁定
            std::unique_lock<timed_mutex> l(mt, std::try_to_lock);
            //检查是否锁定
            if (l) {
                cout << "--lock--" << endl;
            }
            if (l.owns_lock()) {
                cout << "--lock--" << endl;
            }
        }


        {
            //在指定的时间内尝试锁定
            std::unique_lock<timed_mutex> l1(mt, std::chrono::milliseconds(2000));
            //监测是否锁定
            if(l1){
                cout << "**lock**" << endl;
            }
        }


        {
            //明确指定为不锁定
            std::unique_lock<timed_mutex> l2(mt, std::defer_lock);
            l2.lock();
            if(l2)
                cout << "@@@@" << endl;
            l2.unlock();
            if(!l2)
                cout << "@@@@" << endl;
        }


        {
            std::unique_lock<mutex> lockm1(m1, std::defer_lock);
            std::unique_lock<mutex> lockm2(m2, std::defer_lock);

            if(std::try_lock(m1, m2) == -1){
                std::lock_guard<std::mutex> lg1(m1, adopt_lock);
                std::lock_guard<std::mutex> lg2(m2, adopt_lock);
            }
        }
    }

}



















