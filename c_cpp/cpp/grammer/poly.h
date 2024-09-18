//
// Created by lhy on 2020/11/30.
//

#ifndef STL_POLY_H
#define STL_POLY_H

/*
 * 虚函数：用virtual声明的成员函数
 *      1. 类外实现虚函数时，不必再加virtual。
 *      2. 在派生类中重新定义此函数称为覆写，要求函数名，返回值类型，函数参数个数及类型全部匹配。并根据派生类的需要重新定义函数体。
 *      3. 当一个成员函数被声明为虚函数后，派生类中对其覆写的函数也为虚函数。可以添加virtual明示。
 *      4. 定义一个指向基类对象的指针，并使其指向其派生类的对象，通过该指针调用虚函数，此时调用的就是该指针变量指向的对象的同名函数
 *      5. 派生类中覆写的函数，可以为任意访问类型，依派生类需求决定。
 *
 *      虚函数仅适用于有继承关系的类对象
 *      构造函数不能是虚函数。调用构造函数后，对象的创建才算真正的完成
 *      析构函数可以是虚函数且通常声明为虚函数
 *
 * 虚析构函数： 目的是保证派生类的析构函数被调用，如果调用了派生类的析构函数，则基类的析构函数一定会被调用
 *      c++中基类采用virtual析构函数是为了防止内存泄漏。
 *      如果派生类中申请了内存空间，并在其析构函数中对这些内存空间进行释放
 *      假设基类中采用的是非虚析构函数，当删除基类指针指向的派生类对象时就不会触发动态绑定，
 *      因而只会调用基类的析构函数，而不会调用派生类的析构函数
 *      那么在这种情况下，派生类中申请的空间就得不到释放从而产生内存泄漏。
 *
 * 纯虚函数：
 *      纯虚函数只有声明，没有实现，被初始化为0.
 *
 * 抽象类：
 *      含有纯虚函数的类，称为抽象类，不可实例化。即不能创建对象，存在的意义就是被继承，提供族类的公共接口
 *
 * 接口：
 *      只包含纯虚函数的类，称之为接口类；
 *      一个类中声明了纯虚函数，而在派生类中没有对该函数定义，则该函数在派生类中仍然为纯虚函数，派生类仍然为抽象类，不可实例化；
 */


/*
 * 赋值兼容规则： 在需要基类对象的任何地方都可以使用public派生类的对象来替代
 *      1.派生类的对象可以赋值给基类对象（丢弃派生类中增加的成员）
 *      2.派生类的对象可以初始化基类的引用
 *      3.派生类对象的地址可以赋值给指向基类的指针
 *
 * 函数多态：编译态/运行态；具体调用哪个函数由运行时的对象决定；
 *      1. 基类中有虚函数
 *      2. 派生类覆写基类中的虚函数
 *      3. 通过指向基类的指针或者引用来访问虚函数。   **指针或引用
 *
 * 多态分析：
 *      1. 扩展类对象含有基类的一切元素
 *      2. 声明为基类的引用，但是指向的是扩展类的对象，即该对象包含扩展类的一切元素（包括一张虚函数表）
 *      2. 成员函数执行的本质： 将对象内存地址传递给函数， 函数通过对象地址索引对象元素，然后进行相应的运算
 *
 *      函数地址： 当函数被扩展类覆写时，调用的是扩展类的覆写函数。覆写函数和被覆写函数的地址不同
 *      对象地址： 扩展类对象的首地址
 *
 *     结果：
 *          1. 只能索引基类中的元素，因为使用的是基类标识
 *          2. 访问成员变量时，访问的是基类的成员变量
 *          3. 调用函数时： 实际调用哪个函数是由指针指向的对象决定的， 如果指向的是扩展类对象则优先调用扩展覆写的函数。
 *
 *     注： 上述特性之所以可以实现，是因为虚函数表机制，如果访问的是成员变量，则没有此特性
 *
 */


#include "basic.h"

namespace fundamental{

//接口类
    class memoryDevice{
    public:
        //纯虚函数：派生类继承其声明, 必须在派生类中进行实现
        virtual void open() = 0;
        virtual void read() = 0;    //纯虚函数
        virtual void write() = 0;
        virtual void close() = 0;

        //虚函数：派生类继承其声明以及实现
        virtual void printMessage();
    };

//接口实现类
    class usbDevice : public memoryDevice{
    public:
        void open() override{
            cout << "call memoryDevice::open()" << endl;
            memoryDevice::open();
        }

        void read() override{
            cout << "usb device read()" << endl;
        }

        void write() override{
            cout << "usb device write()" << endl;
        }

        void close() override{
            cout << "usb device close()" << endl;
        }

        void printMessage() override{
            cout << "printMessage function in usbDevice..." << endl;
        }
    };


    class diskDevice : public memoryDevice{
    public:
        void open() override{
            std::cout << "diskDevice open" << endl;
        }

        void read() override{
            cout << "diskDevice read()" << endl;
        }

        void write() override{
            cout << "diskDevice write()" << endl;
        }

        void close() override{
            cout << "diskDevice close()" << endl;
        }

        void printMessage() override{
            cout << "printMessage function in diskDevice..." << endl;
        }
    };


/*
 * 继承时，基类的析构函数须为虚析构函数
 * 基类指针指向派生类对象，如果析构函数不为virtual函数，则通过基类指针删除对象时，只会调用基类的析构函数。
 */
    class polyBase{
    public:
        int * intptr;
    public:

        explicit polyBase(int num){
            cout << "polyBase constructor...." << endl;
            intptr = new int;
            *intptr = num;
        }

        virtual ~polyBase(){  //基类的析构函数要声明为virtual函数
            cout << "polyBase destructor...." << endl;
            delete intptr;
        }
    };

    class polyInherit : public polyBase{
    public:
        int * intptr;
    public:
        polyInherit() : polyBase(1){
            cout << "polyInherit constructor...." << endl;
            intptr = new int;
            *intptr = 2;
        }

        ~polyInherit() override{
            cout << "polyInherit destructor...." << endl;
            delete intptr;
        };
    };
}


#endif //STL_POLY_H
