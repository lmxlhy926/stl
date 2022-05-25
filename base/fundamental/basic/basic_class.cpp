//
// Created by lhy on 2020/11/14.
//


#include "basic_class.h"

//类静态成员变量，在类中声明，在类外定义。本质上是普通全局变量。
int fundamental::basic::intVar = 100;

//'static' can only be specified inside the class definition
void fundamental::basic::printMessage() {
    std::cout << "in static function printMessage..." << std::endl;
}

//友元函数可以访问类的所有成员
void fundamental::changeMember(const basic& a) {
    string private_member_str = a.str;      //类的访问修饰限定符指定的封装性对友元函数不起作用
    std::cout << "private_member_str: " << private_member_str << std::endl;
}


using namespace fundamental;
//1. 成员函数的隐式调用
//2. 成员函数的完整调用
void basicTest::basicUsage(){
    //无需创建对象，即可通过类名直接调用静态成员函数，该静态成员函数的访问修饰符为public
    basic::printMessage();

    string str = "hello";
    basic b(str);
    b.showMessage();         //隐式作用域
    b.basic::showMessage();  //显示作用域
}


//1. 指针指向对象
//2. 通过指针访问指针指向的对象的成员
void basicTest::pointerUsage(){
    string str = "hello";
    basic b(str);
    basic *bp = &b;     //类指针变量的赋值
    bp->showMessage();  //通过类对象指针变量访问对象的成员函数
    bp->basic::showMessage();
}


void basicTest::copyConstructInParam(basic in) {
    std::cout << "in copyConstructInParam(basic in) funciton body...." << std::endl;
}

void basicTest::referenceInParam(basic& in) {
    std::cout << "in copyConstructInParam(basic& in) funciton body...." << std::endl;
}

basic basicTest::copyConstructInReturn() {
    std::cout << "in copyConstructInReturn funciton body...." << std::endl;
    string str = "hello";
    basic b(str);
    b.printMessage();
    return b;
}