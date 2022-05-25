//
// Created by lhy on 2020/12/26.
//

#ifndef STL_MMUTEX_H
#define STL_MMUTEX_H

/*
 * mutex
 *      为了获得独占式的资源访问能力， 相应的线程必须锁定mutex, 这样可以防止其它线程也锁定mutex, 直到第一个线程解锁mutex.
 *      这里所谓资源可能是个object, 或多个object的组合.
 *
 *      凡是可能发生并发访问的地方都因该使用同一个mutex, 不论读或写皆如此.
 *
 *      因该确保异常也解除相应的mutex, 否则资源就有可能被永远锁住.
 *
 *      RAII守则：构造函数将获得资源, 而析构函数则负责释放资源。
 *                即使当异常造成生命期结束, 析构函数也总是会被调用.
 *
 *      死锁：2个线程在释放它们自己的锁之前彼此等待对方的lock.
 *
 * lock_guard()
 * unique_lock()
 *      通过以上2个类来使用mutex, 可以保证即使发生异常mutex也可以被正常解锁.
 *
 */

namespace mthread{

    void mutextest();

    void recursive_mutextest();

    void lockGuard();

    void lockGuaed1();

    void lockGuard2();

    void multilock();

    void uniqueLock();


}







#endif //STL_MMUTEX_H
