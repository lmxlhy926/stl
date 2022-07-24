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
 * unique_lock()
 *      unique_lock l:                  建立一个lock但不关联任何mutex
 *      unique_lock l(m):               建立一个lock_guard并锁定它
 *      unique_lock l(m, adopt_lock):   为已锁定的mutex m建立一个lock guard
 *      unique_lock l(m, defer_lock):   建立一个lock_guard但是不锁定它
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

namespace mthread{

    void mutex_lock_try_lock();

    void mutex_2tryLock();

    void lockGuard_test();

    void recursive_mutex_test();

    void lockGuard_test();

    void lockGuard();

    void lockGuaed1();

    void lockGuard2();

    void multilock();

    void uniqueLock();


}







#endif //STL_MMUTEX_H
