//
// Created by lhy on 2020/11/28.
//

#ifndef STL_INHERIT_H
#define STL_INHERIT_H

#include <utility>

#include "basic.h"

/*
 * 继承：新类中包含旧类的内容，从而实现了代码的复用
 * 继承中类的称呼： 基类--派生类   父类--子类
 *
 * 派生类对象的构造：
 *    层级构造：
 *      基类的构造函数完成基类成员的初始化；派生类构造函数完成新增成员变量的初始化；
 *      构造顺序：先构造继承的基类成员-->再构造派生类的新增成员
 *          对象的构造顺序：即成员变量的构造顺序
 *              1. 按照成员变量在类中的声明顺序依次构造（调用初值列指定的构造器-->空参构造器）
 *              2. 执行构造函数
 *          基类对象的构造：
 *              1. 构造基类对象时，构造顺序按照它们被继承时的声明顺序（从左到右, 多继承情形下）
 *              2. 如果基类中没有无参构造函数，那么在派生类的构造函数初值列中必须显示调用基类构造函数。
 *          派生类构造函数：
 *              3. 调用内嵌成员对象的构造函数，调用顺序按照它们在类中的声明顺序。
 *              4. 调用自身的构造函数
 *
 * 派生类对象的销毁：
 *      析构函数的作用并不是删除对象，而是在对象销毁前先完成一些清理工作。
 *      对象的析构：
 *          先调用析构函数，再释放成员变量资源
 *          析构成员变量的顺序是成员变量在类中声明顺序的逆序
 *      派生类对象析构顺序：
 *              1. 调用派生类析构函数
 *              2. 先析构派生类成员对象--->再析构基类对象
 *              3. 析构顺序是构造顺序的逆序
 *
 * 派生类中引用基类成员：
 *      问题：
 *          同名派生类成员遮掩同名基类成员
 *          多继承中访问同名基类成员会产生二义性
 *      解决： 作用域
 *          成员的引用：携带作用域
 *          单继承时，访问成员时如果没有类作用域信息，则按照派生类到基类的顺序查找成员。
 *          多继承时，多个基类中重名的成员，继承到派生类中后，访问时必须需要基类作用域信息。
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


//构造顺序：先构造基类对象，再构造派生类成员变量
    class Inherit : public ParInt, ParIntAnother{
    private:
        int a_;
        MemInt i;   //必须在构造函数里初始化成员对象
        MemStr s;
    public:
        explicit Inherit(string& str) : s(str), i(3), a_(100),
                                        ParIntAnother(2),  ParInt(1){
           cout << "==>Inherit constructor" << endl;
        }

        ~Inherit(){
            cout << "==>Inherit deconstructor" << endl;
        }

        void printValue(){  //类名是一个作用域，通过类名来唯一标识一个成员变量
            cout << "Inherit::a_: " << a_ << endl;  //默认优先在本类中查找,查找不到再到基类中查找
            cout << "Inherit::a_: " << this->a_ << endl;
            cout << "Inherit::a_: " << this->Inherit::a_ << endl;

            cout << "ParInt::a_: " << ParInt::a_ << endl;
            cout << "ParIntAnother::a_: " << this->ParIntAnother::a_ << endl;

            cout << "MemInt::a_: " << i.getValue() << endl;
            cout << "MemStr::str_: " << this->Inherit::s.getValue() << endl;
        }
    };


    //名称遮掩
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
       //引入基类名称，使得编译器可以看到，避免名称遮掩
       //名称遮掩是符号遮掩，和符号对应的成员的类型无关
       using base::f1;
       void f1(){
           std::cout << "===> in inherit2Base f1()" << endl;
       }
    };

}





#endif //STL_INHERIT_H
