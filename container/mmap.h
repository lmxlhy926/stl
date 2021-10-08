//
// Created by lhy on 2021/3/20.
//

#ifndef STL_MMAP_H
#define STL_MMAP_H



/*
 *  1. 将key/value pair当做元素进行管理，它们可根据key的排序准则自动为元素排序
 *  2. 不可以直接改变元素的key,因为这会破坏正确次序
 *
 *
 *
 *
 */



#include <map>
#include <iostream>

using namespace std;
namespace container{

    void mmap_insert();

    void mmap_erase();

    void mmap_functor(int a, int b, string& option);


}

#endif //STL_MMAP_H
