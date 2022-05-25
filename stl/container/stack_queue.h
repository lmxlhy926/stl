//
// Created by 78472 on 2022/1/15.
//

#ifndef STL_STACK_QUEUE_H
#define STL_STACK_QUEUE_H

//container adapter: 改造标准的STL容器，使之满足特殊需求

/*
 * stack:
 *      class stack<>实现出一个stack,可以使用push()将任意数量的元素放入stack,
 *      也可以使用pop()将元素依其插入的相反次序从容器中移除。
 *
 *      template<typename T,
 *               typename Container = deque<T>>
 *               class stack;
 *      选择deque作为存放元素的实际容器，是因为deque移除元素时会释放内存，并且不必在重分配时复制全部元素。
 *      可以使用任何sequence容器支持stack，只要他们提供以下成员函数：back(), push_back(), pop_back().
 *      三个核心接口：
 *          1. push()：将一个元素放入stack内
 *          2. top()：返回stack内的下一个元素
 *          3. pop()：从stack中移除元素
 *          如果stack内没有元素，调用top()和pop()会导致不明确的行为。可以采用size()和empty()验证容器是否为空。
 */


/*
 * queue:
 *      class queue<>实现出一个queue也称为FIFO，queue是一个典型的数据缓冲构造。
 *      template<typename T,
 *               typename Container = deque<T>>
 *               class queue;
 *      可以使用任何sequence容器支持queue，只要它们支持front(),back(),push_back()和pop_front()等操作
 *      queue的核心接口函数：
 *          push()：将一个元素放入queue的末尾          push_back()
 *          pop()：移除queue中的第一个元素             pop_front()
 *          front()：返回queue内的当前第一个元素
 *          back()：返回queue内的当前最后一个元素
 */


/*
 * priority queue:
 *      class priority_queue<>实现出一个queue，其中的元素依优先级被读取。
 *      top()/pop()将会访问/移除下一个元素，所谓的“下一个元素并非第一个放入的元素，而是“优先级最高”的元素
 *      换句话说，priority queue内的元素已经根据其值进行了排序
 *      template <typename T,
 *                typename Container = vector<T>,
 *                typename Compare = less<typename Container::value_type>>
 *                class priority_queue;
 *      核心接口：
 *          push(): 将一个元素放入priority queue中
 *          top():  返回priority queue内的“下一个元素”
 *          pop():  从priority queue内移除下一个元素
 */

void stacktest();

void queuetest();

void priorityQueuetest();

void stackQueueConstructor();

void priorityQueueConstructor();






















#endif //STL_STACK_QUEUE_H
