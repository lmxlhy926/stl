//
// Created by lhy on 2020/11/30.
//

#ifndef STL_POLY_H
#define STL_POLY_H

/*
 * 赋值兼容规则： 在需要基类对象的任何地方都可以使用公有派生类的对象来替代
 *      1.派生类的对象可以赋值给基类对象
 *      2.派生类的对象可以初始化基类的引用
 *      3.派生类对象的地址可以赋值给指向基类的指针
 */


/*
 * 多态形成的条件：
 *      1. 基类中有虚函数
 *      2. 派生类覆写基类中的虚函数
 *      3. 通过已被派生类对象赋值的基类的指针或引用, 调用公用接口。   **指针或引用
 */


/*
 * 虚函数：
 *      1. 在基类中用virtual声明成员函数为虚函数。类外实现虚函数时，不必再加virtual。
 *      2. 在派生类中重新定义此函数称为覆写，要求函数名，返回值类型，函数参数个数及类型全部匹配。并根据派生类的需要重新定义函数体。
 *      3. 当一个成员函数被声明为虚函数后，派生类中对其覆写的函数也为虚函数。可以添加virtual明示。
 *      4. 定义一个指向基类对象的指针，并使其指向其派生类的对象，通过该指针调用虚函数，此时调用的就是该指针变量指向对象的同名函数
 *      5. 派生类中覆写的函数，可以为任意访问类型，依派生类需求决定。
 *
 *      虚函数仅适用于有继承关系的类对象, 普通函数不能声明为虚函数
 *      构造函数不能是虚函数。调用构造函数后，对象的创建才算真正的完成
 *      析构函数可以是虚函数且通常声明为虚函数
 */


/*
 * 抽象类/接口
 *      含有纯虚函数的类，称为抽象基类，不可实例化。即不能创建对象，存在的意义就是被继承，提供族类的公共接口，java中称为interface。
 *      纯虚函数只有声明，没有实现，被初始化为0.
 *       一个类中声明了纯虚函数，而在派生类中没有对该函数定义，则该函数在派生类中仍然为纯虚函数，派生类仍然为纯虚基类
 */


/*
 * 虚析构函数：
 *      c++中基类采用virtual析构函数是为了防止内存泄漏。
 *      如果派生类中申请了内存空间，并在其析构函数中对这些内存空间进行释放
 *      假设基类中采用的是非虚析构函数，当删除基类指针指向的派生类对象时就不会触发动态绑定，
 *      因而只会调用基类的析构函数，而不会调用派生类的析构函数
 *      那么在这种情况下，派生类中申请的空间就得不到释放从而产生内存泄漏。
 */



#include "basic_class.h"

namespace fundamental{

    class poly : public model{
    private:
        int a_;

    public:
    //构造函数
        explicit poly(int a) : model(1, 2, "hello"){
            cout << "==>poly constructor" << endl;
            this->poly::a_ = a;
        }

    public:
    //成员函数
        void show() override{
            cout << "poly::a_: " << this->poly::a_ << endl;
        }

        void interfacefunc() override{
            cout << "in poly polyfunc" << endl;
            cout << "poly::a_: " << this->poly::a_ << endl;
        }

        ~poly() override{
            cout << "==>poly deconstructor" << endl;
        }
    };



/*
 *  派生类对象赋值给基类对象
 *  派生类对象初始化基类对象的引用
 *  派生类对象的地址赋值给指向基类对象的指针
 */
    void test1(){
        model d(100, 200, "world");
        poly p(1000);
        d = p;  //扩展类对象赋值给基类，丢弃扩展类对象的扩展成员
        d.printValue();
    }

    void test2(){
        poly p(1000);
        model& d1 = p;  //派生类对象初始化基类的引用
        d1.interfacefunc();
        d1.~model();  //虚析构函数触发动态绑定，避免派生类对象的内存泄漏
    }

    void test3(){
        poly p(1000);
        model* d2 = &p; //派生类对象的地址赋值给指向基类对象的指针
        d2->interfacefunc();
    }


    class fileoperation{
    public:
        virtual void read() = 0;
        virtual void write() = 0;
        virtual void close() = 0;
    };

    class charFile : public fileoperation{
    public:
        void read() override{
            cout << "charFile read()" << endl;
        }

        void write() override{
            cout << "charFile write()" << endl;
        }

        void close() override{
            cout << "charFile close()" << endl;
        }
    };

    class blockFile : public fileoperation{
    public:
        void read() override{
            cout << "blockFile read()" << endl;
        }

        void write() override{
            cout << "blockFile write()" << endl;
        }

        void close() override{
            cout << "blockFile close()" << endl;
        }
    };

//面向接口编程
    void test4(fileoperation& fop){
        fop.read();
        fop.write();
        fop.close();
    }

    void test5(fileoperation* fop){
        fop->read();
        fop->write();
        fop->close();
    }


/************************功能测试**************************************/
    void polyClassTest(){
       charFile cf;
       blockFile bf;

       test4(cf);
       cout << "*********" << endl;
       test5(&bf);
    }





}





#endif //STL_POLY_H
