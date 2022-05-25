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
 *      数据提供者锁住mutex, 更新条件, 解锁mutex, 然后通知condition variable.
 *      等待者则是以一个unique_lock锁住mutex, 一面检查条件一面等待被通知, 然后释放锁.
 *
 *      其中mutex确保读和写是atomic的, condition variable用来在"有新元素可用"时激发和唤醒另一个线程.
 *      借由notify函数, 唤醒某一个线程, 让其来进行处理. 如果有多个consumer必须处理相同数据, 也可以调用notify_all().
 *
 *  condition variable的消费者总是在"被锁住的mutex"基础上操作, 只有等待函数会执行以下3个atomic步骤暂时解除mutex:
 *      1. unlock mutex然后进入等待状态（waiting state）.
 *      2. 解除因等待而造成的阻塞(unblocking the wait)
 *      3. 再次锁住mutex
 *      4. 如果有predicate, 则判断predicate, 如果条件不满足则重复上述步骤
 *      5. 对数据进行处理
 *
 *      1. lock
 *      wait函数执行过程：
 *          2. 如果有通知到来, 执行步骤5
 *          3. unlock并进入等待状态, 等待通知, 通知到来则执行步骤4
 *          4. 解除等待状态再次lock,
 *          5. 判断条件, 条件不满足执行步骤3
 */

namespace mthread{

    void conVarTest();


}




#endif //STL_CONDITION_H
