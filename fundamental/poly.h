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
 * 虚析构函数： 多态
 *      c++中基类采用virtual析构函数是为了防止内存泄漏。
 *      如果派生类中申请了内存空间，并在其析构函数中对这些内存空间进行释放
 *      假设基类中采用的是非虚析构函数，当删除基类指针指向的派生类对象时就不会触发动态绑定，
 *      因而只会调用基类的析构函数，而不会调用派生类的析构函数
 *      那么在这种情况下，派生类中申请的空间就得不到释放从而产生内存泄漏。
 */


/*
 * 抽象类/接口
 *      含有纯虚函数的类，称为抽象基类，不可实例化。即不能创建对象，存在的意义就是被继承，提供族类的公共接口，java中称为interface。
 *      纯虚函数只有声明，没有实现，被初始化为0.
 *       一个类中声明了纯虚函数，而在派生类中没有对该函数定义，则该函数在派生类中仍然为纯虚函数，派生类仍然为纯虚基类
 */


/*
 * 多态分析：
 *      1. 扩展类对象含有基类的一切元素
 *      2. 声明为基类的引用，但是指向的是扩展类的对象，即该对象包含扩展类的一切元素（包括一张虚函数表）
 *      2. 成员函数执行的本质： 将对象内存地址传递给函数， 函数通过对象地址索引对象元素，然后进行相应的运算
 *
 *      函数地址： 当函数被扩展类覆写时，调用的是扩展类的覆写函数。  ！！！覆写函数和被覆写函数的地址不同
 *      对象地址： 扩展类对象的首地址
 *
 *     结果：
 *          1. 只能索引基类中的元素，因为使用的是基类标识
 *          2. 访问成员变量时，访问的是基类的成员变量
 *          3. 调用函数时： 实际调用哪个函数是由指针指向的对象决定的， 如果指向的是扩展类对象则优先调用扩展覆写的函数。
 *
 *     注： 上述特性之所以可以实现，是因为虚函数表机制，如果访问的是成员变量，则没有此特性
*/


#include "basic_class.h"

namespace fundamental{

//接口
    class FileOperation{
    public:
        virtual void read() = 0;
        virtual void write() = 0;
        virtual void close() = 0;
    };


//接口实现类
    class charFile : public FileOperation{
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


//接口实现类
    class blockFile : public FileOperation{
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
    class File{
    public:
        static void fileReference(FileOperation& file){
            file.read();
            file.write();
            file.close();
        }

        static void filePointer(FileOperation* file){
            file->read();
            file->write();
            file->close();
        }
    };


/*
 *  派生类对象赋值给基类对象
 *  派生类对象初始化基类对象的引用
 *  派生类对象的地址赋值给指向基类对象的指针
 */
    namespace PolyClassTest{

        void test(){
            charFile cf;
            blockFile bf;
            FileOperation &cfReference = cf;    //派生类对象初始化基类对象的引用
            FileOperation *cfPointer = &cf;     //派生类对象的地址赋值给指向基类对象的指针

            File::fileReference(cfReference);
            cout << "**********" << endl;
            File::fileReference(bf);

            cout << "----------" << endl;
            cout << "----------" << endl;

            File::filePointer(cfPointer);
            cout << "**********" << endl;
            File::filePointer(&bf);
        }
    }


}


#endif //STL_POLY_H
