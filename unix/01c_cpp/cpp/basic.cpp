//
// Created by lhy on 2020/11/14.
//

#include "basic.h"
namespace fundamental{
    //类静态成员变量，在类中声明，在类外定义。本质上是普通全局变量，作用域限定在类中
    //所有的类对象中可引用此变量
    int Basic::intVar = 100;

    //'static' can only be specified inside the class definition
    void Basic::printMessage() {
        std::cout << "in static function printMessage..." << std::endl;
    }

    //在类的友元函数中可以访问类的所有成员
    void friendFunc1(const Basic& a) {
        //类的访问修饰限定符指定的封装性对友元函数不起作用
        std::cout << "friendFunc1: " << a.str << std::endl;
    }

    //在类的友元函数中可以访问类的所有成员
    void friendFunc2() {
        string str = "friendFunc2";
        Basic b(str);
        std::cout << "friendFunc2: " << b.str << std::endl;
    }
}

