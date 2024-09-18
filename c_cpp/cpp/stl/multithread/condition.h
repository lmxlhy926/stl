//
// Created by lhy on 2021/1/2.
//

#ifndef STL_CONDITION_H
#define STL_CONDITION_H


/*
 *  有时候, 不同线程执行的任务必须彼此等待, 一个线程必须等待另一个线程的执行结果。
 *  condition variable 用来同步化线程之间的数据流逻辑依赖关系
 *
 * 擦作流程：
 *      数据提供者：锁住mutex, 更新条件, 解锁mutex, 然后使用condition variable进行通知。
 *      数据等待者：则是以一个unique_lock锁住mutex, 一面检查条件一面等待被通知, 然后释放锁.
 *
 *      其中mutex确保读和写是atomic的, condition variable用来在"有新元素可用"时激发和唤醒另一个线程.
 *      借由notify函数, 唤醒某一个线程, 让其来进行处理. 如果有多个consumer必须处理相同数据, 也可以调用notify_all().
 *
 *  condition variable的消费者总是在"被锁住的mutex"基础上操作, 只有wait()函数会执行以下3个atomic步骤暂时解除mutex:
 *      1. 将mutex传递给unique_lock(), 阻塞进行锁定。
 *      2. 没有条件，或者条件不满足，则unlock mutex然后进入等待状态（waiting state），等待被唤醒。条件满足，则不进行wait()。
 *      3. 被condition variable的notify_one()或者notify_all()唤醒，解除因等待而造成的阻塞(unblocking the wait)
 *      4. 再次阻塞进行锁定
 *      5. 如果有条件，则再次进行条件判断，不满足则从步骤1重复。满足则执行wait()后面的数据处理代码
 *
 */

namespace mthread{

    void conVarTest();

    void conVarTest1();
}




#endif //STL_CONDITION_H
