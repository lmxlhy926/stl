

#include <string>
#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

using namespace std;

/*
 *  有时候, 不同线程执行的任务必须彼此等待, 一个线程必须等待另一个线程的执行结果。
 *  condition variable 用来同步化线程之间的数据流逻辑依赖关系
 *
 * 擦作流程：
 *      数据提供者：锁住mutex, 更新条件, 解锁mutex, 然后使用condition variable进行通知。
 *      数据等待者：加锁，判断条件是否满足；满足则执行逻辑；不满足则释放锁并陷入等待；
 * 
 *      其中mutex确保读和写是atomic的, condition variable用来在"有新元素可用"时激发和唤醒另一个线程.
 *      借由notify函数, 唤醒某一个线程, 让其来进行处理. 如果有多个consumer必须处理相同数据, 也可以调用notify_all().
 *
 *  condition variable的消费者总是在"被锁住的mutex"基础上操作, 只有wait()函数会执行以下3个atomic步骤暂时解除mutex:
 *      1. 将mutex传递给unique_lock(), 阻塞进行锁定。
 *      2. 没有条件，或者条件不满足，则解锁mutex然后进入等待状态（waiting state），等待被唤醒；条件满足，则跳过wait，执行下一步。。
 *      3. 被condition variable的notify_one()或者notify_all()唤醒，解除因等待而造成的阻塞(unblocking the wait)
 *      4. 再次阻塞进行锁定
 */


string str{"hello"};                //共享变量
std::mutex mutexLock;               //保护锁
std::condition_variable condVar;    //同步机制

/*
 * 一个notify_one通知, 随机唤醒一个wait.
 * 一个notify_all通知, 唤醒所有等待的wait.
 * 唤醒的是挂在这个等待唤醒列表中的线程, 所以要想被唤醒，当通知来临时，此线程必须已经挂在该等待列表中。
 */
void provider(){
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::lock_guard<std::mutex> lg(mutexLock);
    str = "world";
    condVar.notify_all();   //唤醒挂载在condVar的所有等待，唤醒后，清除等待记录；
    std::cout << "---notify---" << std::endl;
}

/*
 * . wait函数之后的操作在unique_lock的限定的保护区间内进行
 */
void consumer(){
    std::unique_lock<std::mutex> ul(mutexLock);
    std::cout << "----consumer start to wait----" << std::endl;
    condVar.wait(ul);  //解锁，阻塞，等待被唤醒；被唤醒后执行后续逻辑
    std::cout << "consumer str: " << str << std::endl;
}

/**
 * 带条件的wait():
 *      1. 加锁
 *      2. 判断条件是否满足，满足则不进入阻塞等待，直接执行后续逻辑；（此时条件判断，逻辑执行都是处于加锁状态）
 *      3. 条件不满足：释放锁、进入阻塞，等待被唤醒
 *      4. 被唤醒：重复执行上述步骤
*/
void consumer1(){
    std::unique_lock<std::mutex> ul(mutexLock);
    std::cout << "----consumer1 start to wait----" << std::endl;
    condVar.wait(ul, []()->bool{   
        if(str == "hello"){
            return false;
        }else if(str == "world"){
            return true;
        }
        return false;
    });  
    std::cout << "consumer1 str: " << str << std::endl;
}


void conVarTest(){
    auto p1 = std::async(std::launch::async, provider);
    auto c  = std::async(std::launch::async, consumer);
    auto c1 = std::async(std::launch::async, consumer1);
    p1.get();
    c.get();
    c1.get();
}


int main(int argc, char* argv[]){
    conVarTest();


    return 0;
}

