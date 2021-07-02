//
// Created by lhy on 2021/1/30.
//

#ifndef STL_MVECTOR_H
#define STL_MVECTOR_H

/*
 * 操作：
 *      构造（创建对象），复制（对象间赋值）， 析构（销毁对象）
 *
 *      empty, size(), max_size()
 *      大小比较（==, !=）
 *
 *      迭代器（指示元素位置）
 *
 *      索引元素
 *      元素的插入，删除，清空
 *
 */


/*
vector
    功能：将元素置于dynamic array中加以管理的一个抽象概念

    vector支持随机访问，因此只要知道位置，你可以在常量时间内访问任何一个元素。

    Vector的容量：
        1. 一旦内存重新分配，vector元素相关的所有reference, pointer, iterator都会失效
        2. 内存重新分配很耗时间
        3. **可以使用reserve()保留适当容量, 避免重新分配内存

    元素访问：
        对于non-const vector, 这些函数都返回元素的reference， 因此可以使用这些操作函数更改元素内容。

    元素的插入、删除

 */

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

namespace container{

    void vector_construct();

    void vector_assign();

    void vector_access();

    void vector_insert_delete();

}













#endif //STL_MVECTOR_H
