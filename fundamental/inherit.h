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
 *          多继承时，多个基类中重名的成员，继承到派生类中后，访问时必须需要基类作用域信息，
*/


namespace fundamental{

    class MemInt{
    private:
        int a_;
    public:
        explicit MemInt(int a) : a_(a){
            cout << "==>MemInt constructor" << endl;
        }

        ~MemInt(){
            cout << "==>MemInt deconstructor" << endl;
        }

        int getValue(){
            return a_;
        }
    };


    class MemStr{
    private:
        string str_;
    public:
        explicit MemStr(string& str) : str_(str){
            cout << "==>MemStr constructor" << endl;
        }

        ~MemStr(){
            cout << "==>MemStr deconstructor" << endl;
        }

        string getValue(){
            return str_;
        }
    };


    class ParInt{
    protected:
        int a_;
    public:
        explicit ParInt(int a) :a_(a){
            cout << "==>ParInt constructor" << endl;
        }
        ~ParInt(){
            cout << "==>ParInt deconstructor" << endl;
        }
    };

    class ParIntAnother{
    protected:
        int a_;
    public:
        explicit ParIntAnother(int a) : a_(a){
            cout << "==>ParIntAnother constructor" << endl;
        }
        ~ParIntAnother(){
            cout << "==>ParIntAnother deconstructor" << endl;
        }
    };

/*
 *
 * 构造函数调用顺序：
 *      按照声明的顺序调用构造函数
 *      先调用基类构造函数，再调用成员对象构造函数
 */
    class Inherit : public ParInt, ParIntAnother{
    private:
        int a_;
        MemInt i;   //必须在构造函数里初始化成员对象
        MemStr s;
    public:
        explicit Inherit(string& str): a_(0), s(str), i(3), ParIntAnother(2),  ParInt(1){
           cout << "==>Inherit constructor" << endl;
        }

        ~Inherit(){
            cout << "==>Inherit deconstructor" << endl;
        }

        void printValue(){
            cout << "Inherit::a_: " << a_ << endl;
            cout << "Inherit::a_: " << Inherit::a_ << endl;

            cout << "ParInt::a_: " << ParInt::a_ << endl;
            cout << "ParIntAnother::a_: " << ParIntAnother::a_ << endl;
            cout << "MemInt::a_: " << i.getValue() << endl;
            cout << "MemStr::str_: " << s.getValue() << endl;
        }
    };

    namespace InheritClassTest{
        void test(){
            string s = "hello";
            Inherit i(s);
            i.printValue();
        }
    }

}


#endif //STL_INHERIT_H
