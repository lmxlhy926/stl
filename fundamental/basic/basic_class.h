//
// Created by lhy on 2020/11/14.
//

#ifndef STL_BASIC_CLASS_H
#define STL_BASIC_CLASS_H

/*
类和对象：
	类：一个模板, 声名了包含的变量, 以及处理这些变量的函数, 类中的变量和函数都称为类的成员。
	对象：类的实例化, 依据类的模板所创建的变量, 每个对象都占用相应的内存空间(占用空间的大小由所包含的成员变量的大小决定)
	访问：成员访问运算符(.), 范围解析运算符(::)

类成员函数：
	成员函数可以在类声明中直接定义，或者在类外单独使用类名称+范围解析运算符::来定义。
	在类中定义的成员函数把函数声明为内联的，即便没有使用inline标识符。
    **调用时调用的对象会将其内存地址传递给函数的默认隐含参数即this指针变量
    **有了this指针形参的存在, 使得相同的函数可以操作不同的对象. 即对象的内存空间是独占的, 但类的函数是唯一的共享的.

数据封装：
	数据封装是面向对象编程的一个重要特点，可以限制对类内部成员的访问。
	访问修饰符：public, private, protected
	每个标记区域在下一个标记区域开始之前或者在遇到类主体结束右括号之前都是有效的。
	public:在程序中类的外部是可以访问的。
	private:在类的外部是不可访问的，只有本类和友元函数可以访问私有成员。默认情况下，类的所有成员都是私有的。
	protected:和private很相似，可以被派生类所访问。
*/


/*
类的构造函数和析构函数：初始化/收尾处理
	构造函数：
        在每次创建类的新对象时执行, 先创建对象再执行构造函数进行成员变量的初始化
        构造函数的名称和类的名称是完全相同的，并且没有返回类型。
        主要的作用是初始化某些成员变量, 以及执行某些初始化动作

	拷贝构造函数：
        特殊的构造函数，在创建对象时，使用同一类中之前创建的对象来初始化新创建的对象
        **拷贝构造函数的几种常见用法：
            1.通过使用一个同类型的已存在的对象来初始化新创建的对象
            2.复制对象把它作为参数传递给函数
            3.复制对象，并从函数返回这个对象
        如果在类中没有定义拷贝构造函数，编译器会自行定义一个。如果类带有指针变量，并有动态内存分配，则它必须有一个拷贝构造函数。
        构造和赋值的区别：构造是创建新的对象, 赋值是给已创建的对象赋值

	析构函数：
        在每次删除所创建的对象时执行
        名称与类的名称完全相同，只是在名称前加个(~)作为前缀。不返回任何值，也不能带有任何参数。
        主要作用是跳出程序前释放资源
*/


/*
友元函数：
    类的友元函数定义在类外部，但有权访问类的所有私有成员和保护成员。尽管友元函数的原型有在类定义中
	出现过，但是友元函数并不是成员函数。友元可以是一个函数，该函数被称为友元函数；友元也可以是一个类，该
	类被称为友元类。在这种情况下，整个类及其所有成员都是友元。

    如果要声明函数为一个类的友元，需要在类定义中该函数原型前使用关键字friend。
*/


/*
内联函数
	c++内联函数通常与类一起使用。如果一个函数是内联的，那么在编译时，编译器会把该函数的代码副本放置在每个调用该函数的地方。
	对内联函数进行任何修改，都要重新编译函数的所有客户端，因为编译器需要重新更换一次所有的代码，否则将会继续使用旧的函数。
	在函数名前面放置关键字inline，在调用函数之前需要对函数进行定义。

	**在类定义中的定义的函数都是内联函数，即使没有使用inline说明符。
*/


/*
this指针
	在c++中，this指针存放对象的地址。this指针是所有成员函数的隐含参数。因此在成员函数内部，它可以用来
	标识调用对象。友元函数没有this指针，因为友元不是类的成员。只有非静态成员函数才有this指针。

指向类的指针
    一个指向C++类的指针与指向结构体的指针类似，使用类的指针访问类的成员需要使用成员访问运算符->。
    必须在使用指针之前，对指针进行初始化。
*/


/*
静态成员变量
    使用static关键字把类成员变量定义为静态的。当声明类的成员为静态时，这意味着无论创建多少个类的对象，静态成员都只是同一个。
	静态成员在类的所有对象中是共享的。不能把静态成员的初始化放置在类的定义中，可以在类的外部通过使用范围解析运算符::来重新声明
    静态变量从而对它进行初始化。

静态成员函数
    如果把函数成员声明为静态的，就可以把函数和类的任何特定对象独立开来。静态成员函数没有this指针形参, 因此不依赖与任何对象,
    在类对象不存在的情况下也能被调用，静态函数只要使用类名加范围解析运算符::就可以访问。
    静态成员函数只能访问静态成员数据、其它静态成员函数和类外部的其它函数。

静态成员函数和普通成员函数的区别：
	*静态成员函数没有this指针，只能访问静态成员(包括静态成员变量和静态成员函数)
	*普通成员函数有this指针，可以访问类中任意成员
*/

#include <iostream>
#include <cstring>

using namespace std;
namespace fundamental {

    class basic {
    private:
        string str;
        /*
         * 本质上就是一个全局变量, 此处为声明
         * 必须在类的外部通过范围解析运算符完成对静态变量的定义
         */
        static int intVar;

    public:
        //构造器本质上就是函数，在对象被创建时调用
        basic(){
            std::cout << "in basic constructor()..." << std::endl;
        }

        //构造函数重载
        explicit basic(string &inputStr) : str(inputStr) {
            std::cout << "basic constructor(string)..." << std::endl;
        }

        //拷贝构造函数(构造函数没有返回值)：用已有对象创建新的对象
        basic(const basic &other) {
            str = other.str;
            std::cout << "basic copy constructor ..." << std::endl;
        }

        //析构函数没有返回值，没有参数：在对象被销毁时创建
        virtual ~basic() {
            std::cout << "basic destructor..." << std::endl;
        }

        //普通成员函数 (this指针是成员函数的隐含传入参数)
        void showMessage() {
            std::cout << "in memberFunction showMessage..." << std::endl;
            std::cout << "str: " << this->str << std::endl;         //隐含作用域
            std::cout << "str: " << this->basic::str << std::endl;  //显式作用域
        }

        //访问修饰符对类的调用者提供封装性。在类中可以访问本类的所有成员
        string stringContact(const basic& b){
            return this->basic::str += b.str;
        }

        //静态成员函数：没有创建的对象即可使用，本质上就是普通函数
        static void printMessage();

        //类的友元函数在类中进行声明，类取消对友元函数的封装性。
        //还需要在类外进行声明以及定义
        friend void changeMember(const basic& a);

        //运算符重载：操作符的操作数中必须有该类对象。
        //操作符的意义是由操作数决定的
        basic& operator =(const basic& other){
            std:cout << "in operator=...." << std::endl;
            this->str = other.str;
            return *this;
        }
    };

    void changeMember(const basic& a);    //函数声明


}// namespace fundamental



using namespace fundamental;
namespace basicTest{

    void basicUsage();    //成员函数的调用

    void pointerUsage();   //通过指针调用成员

    void copyConstructInParam(basic in);    //调用拷贝构造函数生成实参

    void referenceInParam(basic& in);       //使用引用的方式，传参时不进行对象的创建，引用的本质是传指针

    basic copyConstructInReturn();          //返回对象
}


#if 0

//简单测试
    namespace classTest{

        void test2(){
            string s = "hello";
            Extend e(s);
            e.Basic::printValue();

            e.printValue();     //  e.printValue() == e.Extend::printValue()
            e.Extend::printValue(); //没有类名修饰时按照派生类到父类的顺序查找

        }


    }



/*
 * 类的基本构成
 * new/delete的用法
 */



        void disaddress(){
            cout << "basic: " << static_cast<void *>(&this->MinExtend::a_) << endl;
            cout << "extend: " << static_cast<void *>(&this->MinExtend::MinBasic::a_) << endl;
        }

    class Mintest{
    public:
        /*
         * 多态分析：
         *      1. 声明为基类的指针，但是指向的是扩展类的对象，即该对象包含扩展类的一切元素（包括一张虚函数表）
         *      2. 调用函数的本质还是将对象内存地址传递给函数，然后函数提取对象的元素进行处理
         *      3. 传递的对象是扩展类对象，函数地址指向扩展类覆写的函数
         * 结论：
         *      使用的地址： 扩展类对象首地址
         *      找到的函数： 扩展类中覆写的函数
         *      结果： 写的是基类调用，结果是扩展类调用
         */
        static void show(MinBasic& m){
            cout << "m.a_: " << m.a_ << endl;
            cout << "------" << endl;
            m.dis();
            cout << "------" << endl;
            m.MinBasic::dis();
        }
    };

#endif



















#endif //STL_BASIC_CLASS_H
