//
// Created by lhy on 2020/11/28.
//

#ifndef STL_INHERIT_H
#define STL_INHERIT_H

#include <utility>

#include "basic_class.h"

/*
 * 继承：从已有的类产生新类
 * 继承中类的称呼： 基类/父类   派生类/子类
 *
 * 派生类的初始化：
 *      **派生类中从基类继承而来的成员的初始化工作还是由基类的构造函数完成，派生类新增的成员在派生类的构造函数中初始化。
 *      派生类构造函数执行顺序：先执行基类构造函数-->再执行派生类构造函数
 *          1. 调用基类构造函数时，调用顺序按照它们被继承时的声明顺序（从左到右, 多继承情形下）
 *          2. 如果基类中没有默认构造参数(无参)，那么在派生类的构造函数中必须显示调用基类构造函数，以初始化基类成员。
 *          3. 调用内嵌成员对象的构造函数，调用顺序按照它们在类中的声明顺序。
 *
 * 派生类对象的销毁：
 *      析构函数的作用并不是删除对象，而是在对象销毁前完成一些清理工作。
 *      派生类对象析构顺序： 先执行派生类析构--->再执行基类析构
 *
 * 基类成员的引用：
 *      问题：
 *          同名派生类成员隐藏同名基类成员
 *          多继承中访问同名基类成员会产生二义性
 *
 *      解决： 作用域
 *          成员的引用：携带作用域
 *          单继承时，访问成员时如果没有类作用域信息，则按照派生类到子类的顺序查找成员。
 *          多继承时，多个基类中重名的成员，继承到派生类中后，访问时需要基类作用于信息，
*/


namespace fundamental{

    class inherit : public model
    {
    private:
        int a_;
        double *b_;

    public:
    //构造函数
        inherit(int a, float b, string cmstr) : model(a, b, std::move(cmstr)){
            cout << "==>inherit constructor(int a, float b, string cmstr)" << endl;
            this->inherit::a_ = 100;
            this->inherit::b_ = new double();
            *this->inherit::b_ = 200;
        }

    //覆写函数
        void show() override{
            cout << "---inherit show()" << endl;
            cout << "inherit::a_: " << this->inherit::a_ << endl;
            cout << "inherit::b_ " << *this->inherit::b_ << endl;
            cout << "inherit::model::a_: " << this->inherit::model::a_ << endl;
            cout << "inherit::model::b_: " << *this->inherit::model::b_ << endl;
        }

        ~inherit() override{
            cout << "==>inherit deconstructor" << endl;
            delete(this->inherit::b_);
        }
    };


    class A{
    protected:
        int a_;
    public:
        A():a_(2){}
    };

    class B{
    protected:
        int a_;
    public:
        B():a_(3){}
    };

    class C : A, B{
    public:
    //多继承中访问同名基类成员会产生二异性，必须携带类作用域信息
        void show(){
            std::cout << "A::a_: " << this->A::a_ << std::endl;
            std::cout << "B::a_: " << this->B::a_ << std::endl;
        }
    };


//******************************功能测试*************************************************
    void inheritClassTest(){

        inherit p(1, 2, "hello");
        p.show();
        p.model::show();
        p.inherit::show();
    }

}



#endif //STL_INHERIT_H
