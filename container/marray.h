//
// Created by 78472 on 2021/10/27.
//

#ifndef STL_MARRAY_H
#define STL_MARRAY_H


/*
 * array的大小是固定的，在创建对象时就已经指定，不会再发生变化
 * 越界访问会导致未定义错误
 */



namespace container{
    //创建容器
    void arrayCreate();

    //非更易型属性：是否为空，大小，
    void arrayNonmodify();

    //容器之间赋值，容器的交换
    void assignSwap();

    //元素访问
    void access();

    //作为c-style array
    void cStyleArray();
}




#endif //STL_MARRAY_H
