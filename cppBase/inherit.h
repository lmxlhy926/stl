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
 * 派生类构造函数的调用：
 *      **派生类中从基类继承而来的成员的初始化工作还是由基类的构造函数完成，派生类新增的成员在派生类的构造函数中初始化。
 *      派生类构造函数执行顺序：先执行基类构造函数-->再执行派生类构造函数
 *          基类构造函数：
 *              1. 调用基类构造函数时，调用顺序按照它们被继承时的声明顺序（从左到右, 多继承情形下）
 *              2. 如果基类中没有无参构造函数，那么在派生类的构造函数中必须显示调用基类构造函数，以初始化基类成员。
 *          派生类构造函数：
 *              3. 调用内嵌成员对象的构造函数，调用顺序按照它们在类中的声明顺序。
 *              4. 调用自身的构造函数
 *
 * 派生类对象的销毁：
 *      析构函数的作用并不是删除对象，而是在对象销毁前完成一些清理工作。
 *      派生类对象析构顺序： 先执行派生类析构--->再执行基类析构
 *                           析构函数的调用顺序是构造函数的调用顺序的逆序
 *
 *
 * 基类成员的引用：
 *      问题：
 *          同名派生类成员遮掩同名基类成员
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

        int getValue() const{
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

        string getValue() const{
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
 * 构造函数调用顺序：
 *      按照声明的顺序调用构造函数
 *      先调用基类构造函数，再调用成员对象构造函数，再调用自身构造函数
 */
    class Inherit : public ParInt, ParIntAnother{
    private:
        int a_;
        MemInt i;   //必须在构造函数里初始化成员对象
        MemStr s;
    public:
        explicit Inherit(string& str): a_(100), s(str), i(3), ParIntAnother(2),  ParInt(1){
           cout << "==>Inherit constructor" << endl;
        }

        ~Inherit(){
            cout << "==>Inherit deconstructor" << endl;
        }

        void printValue(){  //通过类名唯一标识每一个成员
            cout << "Inherit::a_: " << this->a_ << endl;
            cout << "Inherit::a_: " << this->Inherit::a_ << endl;

            cout << "ParInt::a_: " << this->ParInt::a_ << endl;     //多继承导致的同名基类成员
            cout << "ParIntAnother::a_: " << this->ParIntAnother::a_ << endl;

            cout << "MemInt::a_: " << this->Inherit::i.getValue() << endl;
            cout << "MemStr::str_: " << this->Inherit::s.getValue() << endl;
        }
    };


/*---------名称遮掩---------*/
   class base{
   public:
       void f1(){
           std::cout << "in base f1()" << endl;
       }

       int f1(int i){
           std::cout << "in base f1(int i)" << endl;
       }
   };

   class inherit2Base : public base{
   public:
       using base::f1;  //引入基类名称，使得编译器可以看到，避免名称遮掩
       void f1(){       //名称遮掩是符号遮掩，和符号对应的成员的类型无关
           std::cout << "===> in inherit2Base f1()" << endl;
       }
   };

}


namespace inheritTest{
    void nameCoverageTest();    //派生类作用域遮掩基类作用域
}



#endif //STL_INHERIT_H
