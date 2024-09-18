
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <unistd.h>

using namespace std;

/*
 * 现代化体系结构通常支持同时执行多个任务和多个线程, 如果采用多处理器内核,那么程序执行时间可在多线程情况下获得大幅改善.
 * 使之受益于并行处理(如果低层平台对此有所支持), 但仍能够在单线程环境中正确运作.
 *
 * 并行带来的挑战:
 *      不再是完成一个语句后执行另一语句,而是多语句同时执行,于是可能导致并发访问同一资源
 *      造成创建,读取,涂写,删除等动作不在预期次序下发生,形成不可测的结果
 *
 * c++11不论语言自身或标准库都得到强化,可支持并发编程.
 *      语言核心定义了一个内存模型，保证当你更改被“2个不同线程使用”的2个object时，它们彼此独立
 *      标准库提供启动多线程，包括：传递实参、返回数值、跨线程边界传递异常、同步化等
 *
 * 组织结构:
 *      多线程启动方法
 *      线程的同步化
 *          Mutex                     独占访问共享资源
 *          condition variable        同步线程次序
 *          Atomic
 *      *多个线程同时运行可能会导致数据的并发访问, 同步化就是解决数据并发访问时可能引发的问题
 */


/*
 * async()辅助函数搭配future<>类，可以启动线程并获取线程例程函数的执行结果
 *      async():
 *          指定启动方式：立即启动、暂缓启动
 *          指定callable object作为线程执行例程
 *          指定callable object需要的参数
 *      future<>:
 *          存储线程例程执行的返回结果或线程
 *          强制线程启动并获取其结果
 *          强制线程启动并等待其结束
 *          查询线程的状态：未启动、已启动未结束、已结束
 *
 * 线程中发生的异常:
 *      异常会终止线程, 因为它未被捕获.
 *      future object会保持这一状态直到get()被调用. 搭配get()后这个异常在main()内被进一步传播
 *      欲处理后台操作所产生的异常, 只需偕同get()作出"以同步方式调用该操作"所作的相同动作即可.
 *
 * async():
 *      让一段机能(a piece of functionality)或说一个callable object若是可能的话在后台运行,成为一个独立线程
 *      尝试将其所获得的函数立刻异步启动于一个分离线程内
 *
 * std::launch::async:  立即启动
 *      有了async发射策略, 就不必非要调用get()了.
 *      当离开返回的future object作用域时,程序会阻塞等待后台任务结束
 *      如果不将async(std::launch::async, ...)的结果赋值出去,调用者会在此停滞直到目标函数结束, 相当于一个同步调用.
 *      如何异步调用在此处无法实现,程序会抛出一个system_error异常.
 *
 * std::launch::deferred: 暂不启动
 *      保证函数绝不会在没有get()或wait()的情况下启动
 *      此策略的用处在于缓式求值
 *
 * 传递实参
 *      如果以by reference方式传递实参, 被传递值甚至可能在后台任务启动前就变得无效.
 *      如果在一个线程中改动c, 在另一个线程中读取c, 这是对同一对象的异步并发处理, 将导致不可预期的行为, 除非使用mutex或atomic保护并发处理动作.
 *      如果使用async(), 就因该尽可能以by value方式传递实参, 使async()只需使用局部拷贝.使用引用时尽可能使用const reference, 且不使用mutable.
 *
 *
 *
 * class future<>: 允许你等待线程结束并取得其结果(一个返回值或是一个异常)
 *
 * future<>::get(): 获取线程结果
 *      如果函数被启动于一个分离线程中并且已结束：   立刻获得其结果
 *      如果函数被启动于分离线程中但是尚未结束：     get()会引发停滞直到函数执行完后获得结果
 *      如果函数尚未启动：                         函数会被强迫启动如同一个同步调用, get()会引发停滞直到产生结果.
 *      只能调用一次, 调用之后future就处于无效状态, 除了valid()和析构外,对它的任何调用都会导致不可预期的行为
 *
 * future<>::wait(): 等待线程执行结束
 *      对某个future调用wait(), 会强制启动该future象征的线程并阻塞等待这一后台操作终止.
 *      等待后台操作完成而不需要处理其结果, 可被调用一次以上.
 *
 * future<>::wait_for(), future<>::wait_until(): 不会强制启动线程，检查线程的运行状态, 返回以下3种状态
 *      std::future_status::deferred: 线程未启动（立即返回）
 *      std::future_status::timeout:  线程启动但是未结束，waiting又已逾期
 *      std::future_status::ready:    线程已结束
 *      轮询时先检查是否启动，再检查是否结束。否则有可能因为线程未启动而陷入死循环。
 * 
 * shared future
 *      函数对应的线程只执行一次, 将结果存储下来, 因此调用get()时, 导致相同的结果, 或导致抛出同一个异常.
 */

//普通函数
int normalFunc(const string& str){
    this_thread::sleep_for(chrono::seconds (1));
    cout << "<in normalFunc>: " << str << endl;
    return 0;
}


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

/*
 *  async()指定为立即启动方式，启动callable objet: 普通函数、成员函数、函数对象、lamda
 *  async()指定为立即启动方式，如果不能立即启动，会抛异常
 *  get()返回的结果也有可能抛异常
 */
void async_future(){
    MemberFuncObj mfobj;
    FuncObj funcObj;

    //当离开返回的future object作用域时,程序会阻塞等待后台任务结束
    std::future<int>  f1;
    std::future<void> f2;
    std::future<void> f3;
    std::future<void> f4;
    std::future<void> f5;

    //尝试在独立分离的线程内运行一个callable object: 普通函数, lamda, 成员函数, 函数对象
    try{
       f1 = std::async(std::launch::async, normalFunc, "普通函数");                              //普通函数
       f2 = std::async(std::launch::async, &MemberFuncObj::memberFunc, mfobj, "成员函数");       //成员函数
       f3 = std::async(std::launch::async, funcObj, "函数对象单参数");                           //函数对象
       f4 = std::async(std::launch::async, funcObj, "hello", "world");                          //函数对象
       f5 = std::async(std::launch::async, [](const string& str){std::cout << str << std::endl;}, "lamda"); //lamda

    }catch(const exception& e){ //不能立即启动则抛出异常
        e.what();
    }

    try{
        /**
         * 顺序阻塞获取其结果，只能获取结果一次
         * 可以传播异常
        */
        f1.get();
        f2.get();
        f3.get();
        f4.get();
        f5.get();
        std::cout << "-----------end------------" << std::endl;
    }catch(const exception& e){ //例程函数执行过程中如果抛出异常，调用get()会重新抛出该异常
        e.what();
    }
}    


/*
 *  线程指定为std::launch::async启动方式，如果线程已启动，则在离开future object作用域之前会阻塞，直到后台任务完成
 */
void future_scope1(){
    std::future<void> f = std::async(std::launch::async, []{
        std::cout << "in subthread: " << std::this_thread::get_id() << std::endl;
        for(int i = 0; i < 5; ++i){
            normalFunc("noraml");
            sleep(1);
        }
    });

    std::cout << "in main thread: " << std::this_thread::get_id() << std::endl;
    //当std::future<>要离开其作用域时，会自动调用get()，阻塞在这里，直到例程函数执行完毕。
}


/*
 * 线程以std::launch::async方式启动，如果没有将返回值赋值给future object
 * 调用者会在此停滞直到目标函数结束, 相当于一个同步调用.
 * 还是会在一个分离线程内执行，但是会阻塞，直到该线程执行完毕，std::async函数返回
 */
void future_scope2(){
    std::cout << "in main thread: " << this_thread::get_id() << std::endl;

    std::future<void> f1 = std::async(std::launch::async, []{
        while(true){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "xxxxxxxxxx" << std::endl;
        }
    });

    //创建独立线程，在线程内阻塞执行，直到线程执行完毕退出
    std::async(std::launch::async, []{
        std::cout << "in subthread: " << this_thread::get_id() << std::endl;
        for(int i = 0; i < 5; ++i){
            std::cout << "------" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::cout << "in main thread: " << this_thread::get_id() << std::endl;
    std::cout << "main thread end...." << std::endl;
}


/*
 * 查询线程状态：可以多次调用
        std::future_status::deferred:   线程没有启动，立即返回
        std::future_status::timeout:    线程已启动，但是在等待时间内依然没有执行完毕
        std::future_status::ready:      线程执行完毕
 */
void future_wait(){
    std::future<void> f = std::async(std::launch::async, []{
       for(int i = 0; i < 5; i++){
            sleep(1);
            std::cout << "----------" << std::endl;
       }
    });

    while(true){
        std::future_status fs = f.wait_for(std::chrono::seconds(1));
        switch(fs){
            case std::future_status::deferred:
                std::cout << "==>defered" << std::endl;
                break;
            case std::future_status::timeout:
                std::cout << "==>timeout" << std::endl;
                break;
            case std::future_status::ready:
                std::cout << "==>ready" << std::endl;
                break;
            default:
                std::cout << "==>unkown" << std::endl;
                break;
        }
        if(fs == std::future_status::ready){
            break;
        }
    }

    std::future_status fs = f.wait_for(std::chrono::milliseconds(100));
    switch(fs){
            case std::future_status::deferred:
                std::cout << "==>defered" << std::endl;
                break;
            case std::future_status::timeout:
                std::cout << "==>timeout" << std::endl;
                break;
            case std::future_status::ready:
                std::cout << "==>ready" << std::endl;
                break;
            default:
                std::cout << "==>unkown" << std::endl;
                break;
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
    std::shared_future<int> f = std::async(std::launch::async, [](){
        for(int i = 0; i < 5; ++i){
            std::cout << "-----------" << std::endl;
            sleep(1);
        }
        return 9999;
    }).share();

    std::cout << "wati for result...." << std::endl;
    cout << f.get() << endl;
    cout << f.get() << endl;
}



int main(int argc, char* argv[]){
    future_shared();

    return 0;
}



