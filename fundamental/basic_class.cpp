//
// Created by lhy on 2020/11/14.
//


#include "basic_class.h"

int fundamental::basic::intVar = 100;    //static can only be specified inside the class definition

void fundamental::basic::printMessage() {
    std::cout << "in static function printMessage..." << std::endl;
}

void fundamental::changeMember() {      //一切函数都是相应namespace下的函数
    string str = "hello";
    basic b(str);
    string private_member_str = b.str;      //类的访问修饰限定符对友元函数不起作用
    std::cout << "private_member_str: " << private_member_str << std::endl;
}


using namespace fundamental;

void basicTest::test(){
    basic::printMessage();  //通过类名直接调用静态成员函数，该静态成员函数的访问修饰符为public

    string str = "hello";
    basic b(str);
    b.showMessage();
    b.basic::showMessage();
}

void basicTest::test1(){
    basic * bp = nullptr;   //类对象指针变量的定义和赋值
    string str = "hello";
    basic b(str);
    bp = &b;    //类指针变量的赋值
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