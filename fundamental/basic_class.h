//
// Created by lhy on 2020/11/14.
//

#ifndef STL_BASIC_CLASS_H
#define STL_BASIC_CLASS_H

/*
类和对象：
	类：一个模板, 声名了包含的变量, 以及处理这些变量的函数, 类中的变量和函数都称为类的成员。
	对象：类的实例化, 依据类的模板所创建的变量, 每个对象都占用相应的内存空间(占用空间的大小由所包含的变量的大小决定)
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
        拷贝构造函数的几种常见用法：
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
	在c++中，this指针存放对象成员变量的地址。this指针是所有成员函数的隐含参数。因此在成员函数内部，它可以用来
	标识调用对象。友元函数没有this指针，因为友元不是类的成员。只有非静态成员函数才有this指针。

指向类的指针
    一个指向C++类的指针与指向结构的指针类似，使用类的指针访问类的成员需要使用成员访问运算符->。
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
#include <utility>
#include <cstring>

using namespace std;
namespace fundamental{

/*
 * 构造一个简单类：
 *      普通成员变量/静态成员变量
 *      构造函数/拷贝构造函数
 *      析构函数
 *      普通成员函数
 *      虚函数
 *      静态函数
 */
    class Basic{
//成员变量/静态成员变量
    protected:
        string str_;
    public:
        static int global_;
        int classFlag{1};

//构造函数, 拷贝构造函数
        Basic() = default;

        Basic(string& str) : str_(str){
            cout << "==>Basic constructor" << endl;
        }

        Basic(const Basic& other){
            cout << "==>Basic copy constructor" << endl;
            this->str_ = other.str_;
            this->classFlag = other.classFlag;
        }

//析构函数
        virtual ~Basic(){
            cout << "==>Basic deconstructor" << endl;
        }

//普通成员函数，虚函数
        void showMessage(){
            cout << "Basic:: show message" << endl;
        }

        virtual void printValue(){
            cout << "Basic::  str_: " << str_ << " global_: " << global_ << endl;
        }

//类函数
        static void showGlobal(){
            cout << "global: " << global_ << endl;
        }
    };
    int Basic::global_ = 100;   //类外声明全局变量


/*
 * 简单的继承类
 *      扩展成员变量
 *      覆写虚函数
 */
    class Extend : public Basic{
    private:
        string str_;
    public:
        int classFlag{2};

    public:
        explicit Extend(string& str) : str_("world"), Basic(str){
            cout << "==>Extend constructor" << endl;
        }

        ~Extend() override{
            cout << "==>Extend deconstructor" << endl;
        }

        void printValue() override{
            cout << "Extend::  Basic::str_: " << Basic::str_ << " Extend::str_: " << str_ << " global_: " << global_ << endl;
        }
    };


/*
 * 多态特性：
 *      多态是针对虚函数的(由于虚函数表得以实现)
 *
 */
    class Poly{
    public:
        static void interfaceoop(Basic& b){
            b.printValue();     //扩展类函数
            b.showMessage();    //基类函数
            cout << "classFlag: " << b.classFlag << endl;   //基类元素
        }
    };


//简单测试
    namespace classTest{
        void test1(){
            string s = "hello";
            Basic b(s);
            Basic::showGlobal();

            b.printValue();         // b.printValue() == b.Basic::printValue()
            b.Basic::printValue();  // 类中的变量和函数都是被类名所修饰的
        }

        void test2(){
            string s = "hello";
            Extend e(s);
            e.Basic::printValue();

            e.printValue();     //  e.printValue() == e.Extend::printValue()
            e.Extend::printValue(); //没有类名修饰时按照派生类到子类的顺序查找

        }

        void test3(){
            string s = "hello";
            Extend e(s);
            Poly::interfaceoop(e);
        }

    }




#if 0
    class comment{
    private:
        string str_;
    public:
        comment()= default;

        explicit comment(string str) : str_(std::move(str)){
            cout << "==>comment(string)" << endl;
        }

        comment(const comment& cm){
            cout << "==>comment(const comment&)" << endl;
            str_ = cm.str_;
        }

        virtual ~comment(){
            cout << "==>~comment()" << endl;
        }

        comment& operator+(const comment& other){
            str_ = str_ + other.str_;
            return *this;
        }

        void showStr(){
            cout << "str_ in_comment: " << str_ << endl;
        }
    };


    class model{
    protected:
    //成员变量
        int a_;
        double *b_;
        static int modelGlobal; //本质上就是一个全局变量, 必须在类的外部通过范围解析运算符重新声明静态变量来对其进行初始化.
        comment cm;
        struct {
            int max = 2;
            int min = 1;
        }range;

    public:
    //构造函数
        model(int a, float b, string commentstr);   //普通构造函数

        model(const model& m);  //拷贝构造函数

    //静态函数
        void static showGlobal();   //static只能在类定义中声明

    public:
    //成员函数
        void printValue();

        model modelChange(model& m);

        void lamda();

    //虚函数
        virtual void show(){
            cout << "---in model show()" << endl;
            cout << "model:a_: " << this->model::a_ << endl;
            cout << "model:b_： " << *this->model::b_ << endl;
        }

        virtual void interfacefunc(){
            cout << "in model interfacefunc" << endl;
        }


    //析构函数
        virtual ~model(){
            cout << "===>model deconstructor" << endl;
            delete(b_); //释放堆资源
        }

    };

    void basicClassTest();

//引用，运算符重载， 返回栈上引用与对象




/*
 * 类的基本构成
 * new/delete的用法
 */
    class Basic{
    private:
        int a_;
        char* str_{nullptr};
        comment cm;
    public:
        Basic() = default;

        Basic(int a, char* str, char* cmstr) : a_(a), cm(cmstr){
            cout << "==>Basic(int, char*, char*)" << endl;
            if(str == nullptr){
               str_ = new char[1];
               str_[0] = '\0';
           }else{
               int len = strlen(str);
               str_ = new char[len + 1];
               strcpy(str_, str);
           }
        }

        Basic(const Basic &other){
            cout << "==>Basic(const Basic& other)" << endl;
            a_ = other.a_;
            cm = other.cm;
            int len = strlen(other.str_);
            str_ = new char[len];
            strcpy(str_, other.str_);
        }

        virtual ~Basic(){
            cout << "==>~Basic()" << endl;
            delete(str_);
        }

        static void showGlobal(){
            cout << "global function" << endl;
        }

        void showValue(){
           cout << "a_: " << a_ << endl;
           cout << "strAddress_: " << static_cast<void *>(str_) << " str_: " << str_ << endl;
           cm.showStr();
        }

        virtual void printValue(){
            cout << "Basic::a_: " << a_ << endl;
        }

        Basic& operator+(const Basic& other){
            this->a_ = this->a_ + other.a_;
            int len = strlen(str_) + strlen(other.str_);
            char * temp = new char[len + 1];
            strcpy(temp, str_);
            strcpy(temp + strlen(str_), other.str_);
            delete(str_);
            str_ = temp;
            cm = cm + other.cm;
            return *this;
        }

        Basic& operator=(const Basic& other){
            if(this == &other)
                return *this;
            this->a_ = other.a_;
            delete(str_);
            int len = strlen(other.str_);
            str_ = new char[len + 1];
            strcpy(str_, other.str_);
            cm = other.cm;
            return *this;
        }

    };


/*
 * static变量的初始化
 */
    class BasicStatic{
    private:
        const int a_{1};
        const static int b_{1};
        static int c_;
    public:
        void show(){
            cout << "a_: " << a_ << " b_: " << b_ << " c_: " << c_ << endl;
        }
    };


    class MinBasic{
    public:
        int a_;
    public:
        explicit MinBasic(int a): a_(a){}

        virtual void dis(){
            cout << "a_: " << a_ << endl;
        }
    };



    class MinExtend :public MinBasic{
    public:
        int a_;
        int b_{1000};
    public:
        explicit MinExtend(int a): a_(a), MinBasic(100){}

        void dis() override {
            cout << "a_: " << a_ << endl;
            cout << "b_: " << b_ << endl;
        }

        void disaddress(){
            cout << "basic: " << static_cast<void *>(&this->MinExtend::a_) << endl;
            cout << "extend: " << static_cast<void *>(&this->MinExtend::MinBasic::a_) << endl;
        }
    };

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



}































#endif //STL_BASIC_CLASS_H
