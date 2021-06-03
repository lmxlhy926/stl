//
// Created by lhy on 2021/3/20.
//

#ifndef STL_MMAP_H
#define STL_MMAP_H





/*
 *
 * 构造(空，初值列，迭代器)，拷贝构造，move构造，析构
 *
 * 非更易型操作：
 *      empty：容器是否为空
 *      size：容器目前容纳的元素个数
 *      max_size：元素容器个数之最大可能量
 *
 *      > < >= <= == !=：容器与容器的比较（key, value, 排序准则都必须有相同的类型）
 *
 *      查找特定key值的元素或位置
 *
 * 赋值：
 *      赋值动作的两端容器必须拥有相同的类型，尽管比较准则本身可能不同但是类型必须相同
 *      如果准则不同，准则本身也会随着容器被赋值或交换
 *      c = c2
 *      c = rv
 *      c = initlist
 *      c1.swap(c2)
 *      swap(c1, c2)
 *
 * 元素访问：通过迭代器或者range-based for循环
 *
 *
 * 更易型操作：
 *      插入元素：（一个值(返回位置)，一个区间的值，初值列）
 *      删除元素：（指定key, 指定位置，指定区间）
 *      清空容器
 *      为元素赋新值
 *
 *
 */

#include <map>
#include <iostream>

using namespace std;
namespace container{

    void mmapTest();

    void mmapTest1();




}

#endif //STL_MMAP_H
