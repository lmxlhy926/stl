

#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <future>

using namespace std;

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
 * 使用thread启动线程的几种方式
 * 想要等待线程结束则调用join(), 将它自母体卸离使它运行于后台而不受任何控制则调用detach(),
 * 必须在thread object寿命结束前这么做, 否则程序会终止并调用std::terminate().
 */
void thread_callableObj(){
    MemberFuncObj mfobj;
    FuncObj fobj;

    try{
        std::thread f1(normalFunc, "普通函数");                           //普通函数
        std::thread f2(&MemberFuncObj::memberFunc, mfobj, "成员函数");    //成员函数
        std::thread f3(fobj, "函数对象单参数");                           //函数对象
        std::thread f4(fobj, "hello", "world");                          //函数对象
        std::thread f5([]{normalFunc("lamda");});                        //lamda

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
 * 在thread object的寿命结束前不调用join()或detach()的其中一个会导致进程结束
 */
void thread_joinDetach(){
    std::thread thread1([]{
        while(true){
            normalFunc("noraml");
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "thread_joinDetach end....." << std::endl;

    //离开作用域前没有调用join()或者detach()，会导致进程结束
}


/*
 * 如果线程的例程在执行过程中抛出异常，但是未被捕获，会导致进程结束
 * 
 */
void thread_exception(){
    std::thread thread1([]{
        throw std::runtime_error(string("runtime_error_ssss"));
    });

    while(true){
        std::cout << "-------" << std::endl;
        usleep(100 * 1000);
        
    }
}

/*
 * thread的基本用法:
 *      调用thread时，如果不能立即启动则会抛出异常
 *      如果在thread执行的例程中抛出异常，但是没有被捕获，则会导致进程结束
 *      使用thread时需要try-catch, 在thread启动的函数内部也需要try-catch.
 */
void thread_usage(){
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
 * main函数结束时，所有后台线程（detached）都会被强制结束
 */
void thread_detached(){
    std::thread thread1([]{
        while(true){
            normalFunc("noraml");
        }
    });
    thread1.detach();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "thread_joinDetach end....." << std::endl;

    //函数执行完毕，线程在后台继续运行，直到线程例程执行完毕或者main函数结束。
}



//利用promise来从线程中返回值或者异常
void executeAndReturn(std::promise<std::string>& p){
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
void threadRetValue(){
    try{
        std::promise<string> p;
        std::future<string> f(p.get_future());
        std::thread t(executeAndReturn, std::ref(p));
        sleep(1);

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


int main(int argc, char* argv[]){

    threadRetValue();

    return 0;
}