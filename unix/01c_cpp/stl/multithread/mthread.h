//
// Created by lhy on 2020/12/18.
//

#ifndef STL_MTHREAD_H
#define STL_MTHREAD_H

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
 *      如果函数尚未启动：                      函数会被强迫启动如同一个同步调用, get()会引发停滞直到产生结果.
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
 */



/*
 * 低层接口thread
 *      class thread没有所谓的发射策略.c++标准库永远试着将目标函数启动于一个新线程中,如果无法做到则抛出异常.
 *      没有接口可处理线程结果.
 *      如果发生异常,但未捕捉于线程之内,程序会立刻终止并调用std::terminate().
 *
 *      想要等待线程结束则调用join(), 将它自母体卸离使它运行于后台而不受任何控制则调用detach(), 必须在thread object寿命结束前这么做,
 *      否则程序会终止并调用std::terminate().
 *
 *      如果你让线程运行于后台(detached)而main()结束了, 所有线程会被鲁莽而硬性的终止.
 *
 * detached thread
 *      detached thread很容易形成问题, 如果它们使用nonlocal资源的话.
 *      问题在于丧失了对detached thread的控制,没有办法得知它是否运行,以及运行多久.
 *
 *      寿命问题一样困扰着global和static object, 因为当程序退离(exit)时, detached thread可能还在运行.
 *      意味着它仍有可能访问已被销毁或正在析构的global或static object, 这会导致不可预期的行为.
 *
 *      终止detached thread的唯一安全方法就是搭配"...at_thread_exit()"函数群中的某一个. 这会强制main thread
 *      等待detached thread真正结束
 *
 *  threaID
 *      所谓的threadID隶属于特殊类型std::thread::id，其值独一无二。
 *      唯一对threadID可进行的操作是比较，以及调用output操作符输出至某个stream.
 *      class id有个default构造函数，会产生一个独一无二的ID来表现“no thread”:   std::thread::id();
 */


/*
 * namespace this_thread
 *      针对任何线程包括主线程main thread, <thread>声明一个命名空间std::this_thread用以提供线程专属的global函数
 *      this_thread::get_id()               获取当前线程id
 *      this_thread::sleep_for(dur)         将某个线程阻塞dur时间段
 *      this_thread::sleep_until(tp)        将某个线程阻塞直到时间点tp
 *      this_thread::yield()                放弃当前线程的时间切片，让下一个线程能够执行
 */


namespace mthread{

    void async_future();

    void future_scope1();

    void future_scope2();

    void future_wait();

    void future_shared();

    void thread();

    void thread_callableObj();

    void thread_exception();

    void thread_joinDetach();

    void thread_detached();

    void promise();
}











#endif //STL_MTHREAD_H
