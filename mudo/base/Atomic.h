//
// Created by 78472 on 2022/6/15.
//

#ifndef STL_ATOMIC_H
#define STL_ATOMIC_H

/*
 * gcc从4.1.2提供了__sync_*系列的built-in函数，用于提供加减和逻辑运算的原子操作
 *
    type __sync_fetch_and_add (type  * ptr, type value, ...)
    type __sync_fetch_and_sub (type  * ptr, type value, ...)
    type __sync_fetch_and_or (type  * ptr, type value, ...)
    type __sync_fetch_and_and (type  * ptr, type value, ...)
    type __sync_fetch_and_xor (type  * ptr, type value, ...)
    type __sync_fetch_and_nand (type  * ptr, type value, ...)

    type __sync_add_and_fetch (type  * ptr, type value, ...)
    type __sync_sub_and_fetch (type  * ptr, type value, ...)
    type __sync_or_and_fetch (type  * ptr, type value, ...)
    type __sync_and_and_fetch (type  * ptr, type value, ...)
    type __sync_xor_and_fetch (type  * ptr, type value, ...)
    type __sync_nand_and_fetch (type  * ptr, type value, ...)

    type可以是1,2,4,8字节长度的Int类型

    bool  __sync_bool_compare_and_swap (type  * ptr, type oldval type newval, ...)
    type __sync_val_compare_and_swap (type  * ptr, type oldval type newval, ...)
    这2个函数提供原子的比较和交换，如果*ptr == oldval, 就将newval写入*ptr.
    第一个函数在相等并写入的情况下返回true.
    第二个函数返回操作之前的值。

    type __sync_lock_test_and_set (type *ptr, type value, ...)
    将*ptr设为value并返回*ptr操作之前的值。

    void __sync_lock_release (type *ptr, ...)
    将*ptr置0
 *
 */

#include "noncopyable.h"

namespace muduo{

    template<typename T>
    class AtomicIntegerT : noncopyable{
    private:
        volatile T value_;
    public:
        AtomicIntegerT() : value_(0) {}

        //获取当前值
        T get(){
            return __sync_val_compare_and_swap(&value_, 0, 0);
        }

        //先获取，后增加
        T getAndAdd(T x){
            return __sync_fetch_and_add(&value_, x);
        }

        //先增加，后获取
        T addAndGet(T x){
            return getAndAdd(x) + x;
        }

        //加一，获取
        T incrementAndGet(){
            return addAndGet(1);
        }

        //减一，获取
        T decrementAndGet(){
            return addAndGet(-1);
        }

        //改变值
        void add(T x){
            getAndAdd(x);
        }

        //加一
        void increment(){
            incrementAndGet();
        }

        //减一
        void decrement(){
            decrementAndGet();
        }

        //先获取，后设置
        T getAndSet(T newValue){
            reurn __sync_lock_test_and_set(&value_, newValue);
        }
    };

    typedef AtomicIntegerT<int32_t> AtomicInt32;
    typedef AtomicIntegerT<int64_t> AtomicInt64;
}

#endif //STL_ATOMIC_H
