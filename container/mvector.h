//
// Created by lhy on 2021/1/30.
//

#ifndef STL_MVECTOR_H
#define STL_MVECTOR_H


/*
vector
    功能：将元素置于dynamic array中加以管理的一个抽象概念

    vector支持随机访问，因此只要知道位置，你可以在常量时间内访问任何一个元素。

    Vector的容量：
        1. 一旦内存重新分配，vector元素相关的所有reference, pointer, iterator都会失效
        2. 内存重新分配很耗时间
        3. 可以使用reserve()保留适当容量, 避免重新分配内存

    vector内部维护了一个数组，capacity()可以查询该数组的容量。数组容量不够时会发生重新分配，
    此时会分配一个容量更大的数组，并将之前数组的元素复制到新的数组。
 */

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

namespace container{

    void vector_construct();        //构造函数

    void vector_assign();           //容器赋值，容器交换

    void vector_access();           //容器元素的访问

    void vector_insert_delete();    //插入、删除、清空

    void deque_operate();          //操作deque

}













#endif //STL_MVECTOR_H
