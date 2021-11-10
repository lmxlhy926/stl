//
// Created by 78472 on 2021/10/27.
//

#include "marray.h"
#include <array>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;


/*
 * array的构造器；
 *  1. 空构造器
 *  2. 拷贝构造器
 *  3. 初值列
 *  array的大小是在template实参里确定的，和后期是否插入元素无关
 */
void container::arrayCreate(){
    std::array<int, 4> a;      //元素值不确定
    for(auto elem : a)
        std::cout << "a: " << elem << std::endl;
    std::cout << "a.size(): " << a.size() << endl;

    std::array<int, 4> b {};    //所有元素都由元素类型的default构造函数初始化
    for(auto elem : b)
        std::cout << "b: " << elem << std::endl;

    //初值列没有足够的元素，剩余的元素会由元素类型的default构造函数初始化
    std::array<int, 4> c = {1, 2, 3};
    for(auto elem : c)
        std::cout << "c: " << elem << std::endl;

    std::array<int, 4> d(c);    //拷贝构造
    for(auto &elem : d)
        std::cout << "d: " << elem << std::endl;

}


/*
 *  array的大小是固定的
 *  array的size() == max_size()
 *  empty() 等价于 size() == 0
 *  只有实参里表示大小的参数为0，array的大小才为0
 */
void container::arrayNonmodify(){
    std::array<string, 4> a = {"a", "b", "c"};
    std::cout << "a.size(): " << a.size() << std::endl;
    std::cout << "a.max_size(): " << a.max_size() << std::endl;


    std::array<int, 3> b;
    std::cout << "b.size(): " << b.size() << std::endl;
    std::cout << "b.max_size(): " << b.max_size() << std::endl;
}


/*
 * array容器之间赋值或交换：2个容器的大小和元素类型要一致
 */
void container::assignSwap(){
    std::array<string, 3> a = {"hello", "world", "!"};
    std::array<string, 3> b = {"ni", "hao", "ne"};
    std::array<string, 4> c = {"c", "c", "c"};

    b = a;                  //容器的赋值（元素的类型和数组的大小必须一致）
    for(auto &elem : b)
        std::cout << "b: " << elem << std::endl;

    b.fill("hahaha");   //赋指定值给array的每一个元素
    for(auto &elem : b)
        std::cout << "bfill(): " << elem << std::endl;

    b.swap(a);         //交换容器内容
    for(auto &elem : b)
        std::cout << "bswap: " << elem << std::endl;
}


/*
 * 元素访问：
 *      front(), back(): 确保容器不为空
 *      访问容器元素时注意越界问题
 */
void container::access(){
    std::array<string, 3> a = {"hello", "world", "!"};

    std::cout << "a.front(): " << a.front() << std::endl;
    std::cout << "a.back(): " << a.back() << std::endl;
    std::cout << "a[0]: " << a[0] << std::endl;
    std::cout << "a[1]" << a.at(1) << std::endl;
}


/*
 * array中的所有元素一定位于连续且相邻的内存空间内
 * array首元素的地址：a.data()
 *                 不要使用a.begin()作为首元素的地址，迭代器类型取决于其实现
 */
void container::cStyleArray(){
    std::array<char, 40> a;
    strcpy(a.data(), "hello world");
    printf("===>%s\n", a.data());
}










