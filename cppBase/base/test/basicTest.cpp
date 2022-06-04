
#include <iostream>
#include "basic.h"

using namespace fundamental;

//成员函数的调用
void basicUsage(){
    //无需创建对象，即可通过类名直接调用静态成员函数，该静态成员函数的访问修饰符为public
    Basic::printMessage();

    string str = "hello";
    Basic b(str);
    b.showMessage();         //成员函数的隐式调用
    b.Basic::showMessage();  //成员函数的完整调用
}

//指针指向类对象
//通过指针访问指针指向的对象的成员
void pointerUsage(){
    string str = "hello";
    Basic b(str);
    Basic *bp = &b;           //类指针变量的赋值
    bp->showMessage();        //通过类对象指针变量访问对象的成员函数
    bp->Basic::showMessage();
}

//形式参数传参时发生拷贝构造，函数作用域结束时形式参数对象会自动销毁
void copyConstructInParam(Basic in){
    std::cout << "in copyConstructInParam(basic in) funciton body...." << std::endl;
}

//使用引用的方式，传参时不进行对象的创建，引用的本质是传指针
void referenceInParam(Basic& in){
    std::cout << "in copyConstructInParam(basic& in) funciton body...." << std::endl;
}

//调用拷贝构造函数返回对象
Basic copyConstructInReturn(){
    string str = "hello";
    Basic b(str);
    return b;
}

//拷贝构造函数在函数传参和返回值中应用测试
void copyCtcTest();


//重载运算符
void operatorTest(){
    string str = "hello";
    Basic b1(str), b2(str), b3(str);
    b1 += b2 += b3;
    std::cout << "b1: " << b1.formatString() << std::endl;
    std::cout << "b2: " << b2.formatString() << std::endl;
    std::cout << "b3: " << b3.formatString() << std::endl;
}

int main(int argc, char* argv[]){
    string str = "Basic";
    Basic basic(str);

    basicUsage();
    pointerUsage();
    copyConstructInParam(basic);
    referenceInParam(basic);
    Basic ret = copyConstructInReturn();

    return 0;
}













