
#include <mutex>
#include <chrono>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <future>

using namespace std;

/*
 * mutex
 *      为了获得独占式的资源访问能力， 相应的线程必须锁定mutex, 这样可以防止其它线程也锁定mutex, 直到第一个线程解锁mutex.
 *      这里所谓资源可能是个object, 或多个object的组合.
 *
 *      凡是可能发生并发访问的地方都因该使用同一个mutex
 *      因该确保异常也解除相应的mutex, 否则资源就有可能被永远锁住.
 *
 *      RAII守则：构造函数将获得资源, 而析构函数则负责释放资源。即使当异常造成生命期结束, 析构函数也总是会被调用.
 *
 *      死锁：2个线程在释放它们自己的锁之前彼此等待对方的lock.
 *
 * 锁的分类：
 *      mutex:                    只能锁定一次，多次锁定会导致阻塞
 *      recursive_mutex:          同一线程内可被多次锁定
 *      timed_mutex：             mutex增加try_lock_for(), try_lock_until()
 *      recursive_timed_mutex     recursive_mutex增加try_lock_for(), try_lock_until()
 *
 * 
 * lock_guard()
 *      lock_guard lg(m) :              为mutex m建立一个lock guard并锁定之
 *      lock_guard lg(m, adopt_lock):   为已经被锁定的mutex m建立一个lock guard
 *
 * 
 * unique_lock()
 *      unique_lock l:                  建立一个lock但不关联任何mutex
 *      unique_lock l(m):               建立一个lock_guard并锁定它
 *      unique_lock l(m, adopt_lock):   为已锁定的mutex m建立一个lock guard
 *      unique_lock l(m, defer_lock):   建立一个lock_guard但是不锁定它
 * 
 *      unique_lock l(m, try_lock):     建立一个lock_guard并试图锁定它
 *      unique_lock l(m, dur):          建立一个lock guard并试图在时间段dur内锁定它
 *      unique_lock l(m, tp):           建立一个lock_guard并试图在时间点tp之前锁定它
 *
 *      l.owns_lock():  如果关联的mutex被锁定则返回true
 *      if(l):          检查被关联的mutex是否被锁定
 *
 * 通过以上2个类来使用mutex, 可以保证即使发生异常mutex也可以被正常解锁.
 *
 */


/**
 * mutex的基本用法：lock(), try_lock(), unlock()
 * lock()锁定后调用try_lock()，由于已经被锁定，所以try_lock()返回失败
*/
void cppMutex(){
    std::mutex mutex_;
    mutex_.lock();  
    if(mutex_.try_lock()){  //lock()锁定后调用try_lock()会返回失败;
        std::cout << "try lock successfully...." << std::endl;
    }else{
        std::cout << "try lock failed..." << std::endl;
    }

    mutex_.unlock();
    mutex_.unlock();    //多次调用unlock()不会抛异常
    if(mutex_.try_lock()){
        std::cout << "try lock successfully...." << std::endl;
    }else{
        std::cout << "try lock failed..." << std::endl;
    }
}


/**
 * timed_mutex基本用法：lock(), try_lock(), try_lock_for(), try_lock_until(), unlock()
*/
void cppTimedMutex(){
    std::timed_mutex tMutex;
    tMutex.lock();
    if(tMutex.try_lock()){
        std::cout << "try_lock successfully...." << std::endl;
    }else{
        std::cout << "try_lock failed..." << std::endl;
    }
    if(tMutex.try_lock_for(std::chrono::seconds(1))){
        std::cout << "try_lock_for successfully...." << std::endl;
    }else{
        std::cout << "try_lock_for failed..." << std::endl;
    }
    tMutex.unlock();
}


/*
* 同一把锁可以传递给多个std::lock_guard对象, 自己也可以执行lock, unlock操作
* 如果析构时mutex仍被锁住，其析构函数会自动调用unlock()。如果当时没有被锁住，则析构函数不做任何事情。
*      默认：创建时阻塞锁定
*      lock_guard lg(m, std::adopt_lock) : 为已经被锁定的mutex m建立一个lock guard.
*/
void lockGuard(){
    std::mutex mutex_;
    std::lock_guard<std::mutex> lg(mutex_);     //阻塞锁定
    std::lock_guard<std::mutex> lga(mutex_, std::adopt_lock);   //接受一个已锁定的mutex
    printf("---------\n");
    std::lock_guard<std::mutex> lg2(mutex_);    //前面已经获取锁，此处会阻塞
    printf("---------\n");
}



/**
 * 按照默认规范来调用接口，‘std::adopt_lock’指明传递的锁已经上锁
 * 当将锁交给unique_lock对象来控制，后续就应该全程使用unique_lock对象来控制
*/
void uniqueLock_adopt(){
    std::mutex mutex_;
    std::unique_lock<std::mutex> ula(mutex_, std::adopt_lock);  //接受一个已加锁的mutex, 即使没加锁也认为加锁
    // ula.lock();             //此时加锁，会抛异常，ula认为自己接受的锁已经加锁
    mutex_.unlock();        //即使这里解锁，下面的判断依然认为是加锁状态，因为没有使用unique_lock对象来操作 
    if(ula.owns_lock()){    //即使传递的锁没有上锁，ula认为其接受的是一个已经上锁的锁，此处会认为已上锁
        std::cout << "---locked---" << std::endl;
    }else{
        std::cout << "---unlocked---" << std::endl;
    }
}


/**
 * unique_lock: 
 *          接收锁和指定方式，可以调用锁的相应函数，可以判断锁是否被锁定
 *          使用unique_lock时，如果重复锁定、重复释放、没有锁定就释放等操作会抛出异常。
*/
void uniqueLock_param(){
    std::mutex mutex_;
    std::unique_lock<std::mutex> uld(mutex_, std::defer_lock);  //关联但是不锁定
    std::unique_lock<std::mutex> ul(mutex_);   //阻塞锁定
    std::unique_lock<std::mutex> ultry(mutex_, std::try_to_lock); //尝试去锁定
    if(ul.owns_lock()){ //阻塞锁定成功
        std::cout << "---ul locked---" << std::endl;
    }else{
        std::cout << "---ul unlocked---" << std::endl;
    }

    if(ultry.owns_lock()){ //由于前面的阻塞锁定成功，后面的try_lock()锁定失败，此处显示失败
        std::cout << "---ultry locked---" << std::endl;
    }else{
        std::cout << "---ultry unlocked---" << std::endl;
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
 * unique_lock对象调用unlock()可能抛出异常, 如果这个unique_lock并未被锁住
 * mutex传递给class unique_lock后, 因统一使用unique_lock对象来调用相应的函数
 */
void uniqueLockUsage(){
    std::timed_mutex mt;    //带时间属性的锁，额外支持try_lock_for(), try_lock_until()
    {
        //尝试锁定
        std::unique_lock<timed_mutex> l(mt, std::try_to_lock);
        //检查是否锁定
        if (l) {
            cout << "--locked--" << endl;
        }
        if (l.owns_lock()) {
            cout << "--locked--" << endl;
        }
    }
    std::cout << "------------" << std::endl;

    {
        //在指定的时间内尝试锁定
        std::unique_lock<timed_mutex> l1(mt, std::chrono::milliseconds(2000));
        //监测是否锁定
        if(l1){
            cout << "**lock**" << endl;
        }
        if (l1.owns_lock()) {
            cout << "--lock--" << endl;
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
}



std::mutex mutex_;
void printStr1(){
    while(true){
        {
            std::lock_guard<mutex> lg(mutex_);
            std::cout << "1------" << std::endl;
            sleep(5);
            std::cout << "2------" << std::endl;
        }
        sleep(1);   //线程睡眠时，锁是被释放的状态
    }
}

void printStr2(){
    while(true){
        {
            std::lock_guard<mutex> lg(mutex_);
            std::cout << "******" << std::endl;
        }
        usleep(100 * 1000);     //线程睡眠时，锁是被释放的状态，否则其它线程无法获得锁
    } 
}

void printStr3(){
    //抛出异常时，离开lock_guard()的作用域，会导致guard对象被析构，从而导致解锁
    try{
        std::unique_lock<mutex> l(mutex_);
        std::cout << "-----printStr3-----" << std::endl;
        std::__throw_length_error("exception.....");
    }catch(const exception& e){
        cout << e.what() << endl;
    }
    //锁被释放掉
}

void printAsyncTest(){
    std::future<void> f1 = async(std::launch::async, printStr1);
    std::future<void> f2 = async(std::launch::async, printStr2);
    std::future<void> f3 = async(std::launch::async, printStr3);
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
    std::mutex printMutex, writeMutex;
    try{
        auto f = async(std::launch::async, [&]{
            try{
                if(std::try_lock(printMutex, writeMutex) == -1){    //2锁同时锁定成功
                    /*
                     * 锁定后，将锁过继给lock_guard对象，保证在离开guard对象作用域后解除锁定
                     * 这里try_lock()锁定2个锁后，如果不过继给lock_guard对象，或者不显式调用unlock(), 则不会自动解锁
                     */
                    std::lock_guard<mutex> l(printMutex, std::adopt_lock);
                    std::lock_guard<mutex> g(writeMutex, std::adopt_lock);
                    std::cout << "-------" << std::endl;
                }
            }catch (const exception& e){
                cout << e.what() << endl;
            }
        });

        this_thread::sleep_for(chrono::seconds (1));
        {
            std::lock_guard<mutex> l(printMutex);
            std::cout << "*******" << std::endl;
        }

    }catch(const exception& e){
        cout << e.what() << endl;
    }
}

 /*
  * recursive_mutex的经典应用：
  *       每个成员函数都使用了锁，成员函数之间会互相调用，导致锁的重复调用
  *       支持同一线程内锁的多次锁定
  */
class Access{
private:
    std::recursive_mutex rMutex;
public:
    void func1(){
        std::lock_guard<recursive_mutex> l(rMutex);
        cout << "-----in func1-----" << endl;
    }

    void func2(){
        std::lock_guard<recursive_mutex> l(rMutex);
        cout << "-----in func2-----" << endl;
        func1();
        cout << "**********" << endl;
    }

    void func3(){
        std::lock_guard<recursive_mutex> l(rMutex);
        cout << "------in func3-------" << endl;
    }
};



int main(int argc, char* argv[]){
    printAsyncTest();

    return 0;
}
















