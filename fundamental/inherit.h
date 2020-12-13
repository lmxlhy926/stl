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
 *      派生类中从基类继承而来的成员的初始化工作还是由基类的构造函数完成，派生类新增的成员在派生类的构造函数中初始化。
 *      派生类构造函数执行顺序：先执行基类构造函数-->再执行派生类构造函数
 *          1. 调用基类构造函数时，调用顺序按照它们被继承时的声明顺序（从左到右, 多继承情形下）
 *          2. 如果基类中没有默认构造参数(无参)，那么在派生类的构造函数中必须显示调用基类构造函数，以初始化基类成员。
 *          3. 调用内嵌成员对象的构造函数，调用顺序按照它们在类中的声明顺序。
 *
 * 基类成员的引用：
 *      问题：
 *          同名子类成员隐藏同名父类成员
 *          多继承中访问同名父类成员会产生二义性
 *
 *      解决： 作用域
 *          父类成员的引用：携带父类作用域
 *          子类中要访问继承下来的重名成员，则会产生二义性，为了避免冲突，访问时需要还有父类的作用域信息。
 *          多个父类中重名的成员，继承到子类中后，为了避免冲突，携带了各父类的作用域信息，
 *
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
            cout << "==>inherit constructor" << endl;
            this->inherit::a_ = 100;
            this->inherit::b_ = new double();
            *this->inherit::b_ = 200;
        }

    //覆写函数
        void show() override{
            cout << "---inherit show()" << endl;
            cout << "inherit::a_: " << this->inherit::a_ << endl;
            cout << "inherit::b_ " << *this->inherit::b_ << endl;

        }

        ~inherit() override{
            cout << "==>inherit deconstructor" << endl;
            delete(this->inherit::b_);
        }
    };

    void inheritClassTest(){

        inherit p(1, 2, "hello");
        p.show();
        p.model::show();
        p.inherit::show();

    }

}



#endif //STL_INHERIT_H
