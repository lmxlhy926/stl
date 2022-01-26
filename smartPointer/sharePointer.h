//
// Created by 78472 on 2022/1/16.
//

#ifndef STL_SHAREPOINTER_H
#define STL_SHAREPOINTER_H

/*
 * 所有smart pointer class都被定义于头文件<memory>内
 * 如何确保“pointer的寿命”和“其所指向的对象的寿命”一致？
 *      shared_ptr实现共享式拥有的概念，多个smart pointer可以指向相同的对象，该对象和其相关资源
 *      会在“最后一个reference被销毁”时释放。
 *
 *      unique_ptr实现独占式拥有或严格拥有的概念，保证同一时间内只有一个smart pointer指向该对象，
 *      可以移交所有权。
 */


void test1();

void test2();




#endif //STL_SHAREPOINTER_H
