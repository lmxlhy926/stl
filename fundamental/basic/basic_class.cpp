//
// Created by lhy on 2020/11/14.
//


#include "basic_class.h"

//类静态成员变量，在类中声明，在类外定义
int fundamental::basic::intVar = 100;

//'static' can only be specified inside the class definition
void fundamental::basic::printMessage() {
    std::cout << "in static function printMessage..." << std::endl;
}

//友元函数可以访问对象的私有成员。友元函数在类中声明，在类外定义
void fundamental::changeMember(const basic& a) {
    string private_member_str = a.str;      //类的访问修饰限定符对友元函数不起作用
    std::cout << "private_member_str: " << private_member_str << std::endl;
}


using namespace fundamental;
//1. 成员函数的隐式调用
//2. 成员函数的完整调用
void basicTest::test(){
    //无需创建对象，即可通过类名直接调用静态成员函数，该静态成员函数的访问修饰符为public
    basic::printMessage();

    string str = "hello";
    basic b(str);
    b.showMessage();         //成员函数的隐式调用方式
    b.basic::showMessage();  //成员函数的完整名称
}

//1. 指针指向对象
//2. 通过指针调用对象的成员
void basicTest::test1(){
    string str = "hello";
    basic b(str);
    basic *bp = &b;     //类指针变量的赋值
    bp->showMessage();  //通过类对象指针变量访问对象的成员函数
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