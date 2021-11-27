//
// Created by lhy on 2021/3/20.
//

#ifndef STL_MMAP_H
#define STL_MMAP_H



/*
 *  1. 将key/value pair当做元素进行管理，它们可根据key的排序准则自动为元素排序
 *  2. 不可以直接改变元素的key,因为这会破坏正确次序
 */

#include <map>
#include <iostream>

using namespace std;
namespace container{

    //map元素的几种插入方式
    void mmap_insert();

    //删除指定key，指定位置，指定区间
    void mmap_erase();

    //函数表
    void mmap_functor(int a, int b, string& option);

}

#endif //STL_MMAP_H
