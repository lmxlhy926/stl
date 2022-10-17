//
// Created by 78472 on 2022/5/29.
//

#ifndef STL_SYMBOL_H
#define STL_SYMBOL_H

#include <string>
#include <iostream>
using namespace std;

/*
 * 常量定义式通常被放在头文件内，以便被不同的源码含入.
 * 优秀的编译器不会为"整数型const对象"设定另外的存储空间(除非创建一个pointer或reference指向该对象)
 *
 */
    const int AspectRatio = 100;    //注意常量符号名称的写法; 大写名称通常用于宏


class GamePlayer3{
public:
    static const string str;
    void show(){
        std::cout << "str: " << str << std::endl;
    }
};



#endif //STL_SYMBOL_H
