//
// Created by lhy on 2021/1/23.
//

#ifndef STL_OVERLOAD_H
#define STL_OVERLOAD_H


#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

namespace fundamental{

#if 0
    class Complex{
    private:
        int x_;
        int y_;

    public:
         explicit Complex(int x = 0, int y = 0){
            cout << "Complex constructor(int x, int y)" << endl;
            x_ = x;
            y_ = y;
        }

        Complex(const Complex& c){
            cout << "Complex copy constructor" << endl;
            this->x_ = c.x_;
            this->y_ = c.y_;
        }

        virtual ~Complex(){
             cout << "Complex deconstructor" << endl;
         }

     //运算符重载
        Complex& operator=(const Complex& c){
             cout << "Complex assignment =" << endl;
             if(this == &c)
                 return *this;
             else{
                 this->x_ = c.x_;
                 this->y_ = c.y_;
             }
             return *this;
         }

        Complex& operator+=(const Complex& c){
            this->x_ += c.x_;
            this->y_ += c.y_;
            return *this;
        }

    //同一个类的不同对象，数据是不同的，函数是唯一的。函数通过对象的地址来操作不同的对象
        void manipulate(Complex* c){
             c->x_ = 1;
             c->y_ = 1;
             c->dis();
         }


        void dis(){
            cout << "(" << x_ << "," << y_ << ")" << endl;
        }
    };


/*
 * 形式参数对象，返回栈上的引用和对象
 */
    class ReferenceTest{
    public:
        /*
         * 成员函数的调用原理： 将对象的地址传递给了函数
         * 成员函数的隐式指针形参变量：this
         */
        static void objectfunctest(){
            Complex c(2, 2);
            c.manipulate(&c);
        }


        /*
         * 运算符结果返回值和引用的区别： 返回引用时，在连等式中可以在一个对象中积聚运算的所有副作用
         */
        static void overloadtest1(){
            Complex c(10,1), c1(20, 1), c2(30, 1);

            (c += c1) += c2;    //c的最终值是c的初始值加c1、c2的值
            c.operator+=(c1);   //运算符重载的本质是函数重载

            c.dis(); c1.dis(); c2.dis();
        }

        /*
         * 形式参数传参时发生拷贝构造，函数作用域结束时形式参数对象会自动销毁
         */
        static void overloadFunc2(Complex c){
            cout << "+++++++" << endl;
            cout << "overloadFunc2" << endl;
        }

        static void overloadtest2(){
            Complex c(1,2);
            overloadFunc2(c);   //拷贝构造了一个新对象
            cout << "+++++++" << endl;
        }


        /*
         * 形式参数的类型是引用，不会创建新的对象
         */
        static void overloadFunc3(Complex& c){
            cout << "+++++++" << endl;
            cout << "overloadFunc3" << endl;
        }

        static void overloadtest3(){
            Complex c(1, 2);
            overloadFunc3(c);   //引用，没有创建新对象
            cout << "+++++++" << endl;
        }


        /*
         * 在overloadtest4函数栈上事先开辟了一个临时空间，把这个空间的地址隐式的转到overloadFunc4函数栈上
         * 函数返回时拷贝构造临时对象
         * 再执行赋值操作
         */
        static Complex overloadFunc4(Complex& c){
            cout << "+++++++" << endl;
            return c;   //拷贝构造了一个新对象
        }

        static void overloadtest4(){
            Complex c(1, 2);
            c = overloadFunc4(c);   //赋值操作后，临时对象自动销毁
            cout << "+++++++" << endl;
        }


        /*************返回引用，没有生成新的对象*************/
        static Complex& overloadFunc5(Complex& c){
            return c;
        }

        static void overloadtest5(){
            Complex c(1, 2);
            cout << "c_address: " <<  &c << endl;
            cout << "address: " << &overloadFunc5(c) << endl;
            cout << "+++++++" << endl;
        }


        /*
         * 做了优化，b的构造完成c的构造
         */
        static Complex overloadFunc6(){
            Complex b;
            cout << "func_address: " << &b << endl;
            return b;
        }

        static void overloadtest6(){
            Complex c = overloadFunc6();
            cout << "c_address: " << &c << endl;
        }


        /*
         * 先各自构造，然后发生赋值
         */
        static void overloadtest7(){
            Complex c;
            cout << "c_address: " << &c << endl;
            c = overloadFunc6();
        }
    };


    class MyString{
    private:
        char* str_;

    public:
    //构造函数没有返回值
        explicit MyString(const char* str = nullptr);

        MyString(const MyString& other);

        virtual ~MyString();

    //运算符重载, 运算符重载的本质是函数重载
        MyString& operator=(const MyString& other);

        MyString operator+(const MyString& other);

        bool operator==(const MyString& other);

        char& operator[](int index);

    //显示
        char* tostr() { return str_;}

        void dis(){ cout << "str_address: " << static_cast<void *>(str_) << endl;}
    };


    class Sample{
    public:
        string _name;
    public:
        explicit Sample(string name): _name(std::move(name)){};
    public:
        void dis(){
            cout << "_name: " << _name << endl;
        }

        static void display(){
            cout << "display" << endl;
        }
    };


    class MyStringArrayTest{
    public:
        static void test1(){
            MyString s;
            MyString s1[2] = {MyString("hello"), MyString("world")};
            MyString s2[] = {MyString("hello"), MyString("world")};
            auto *p = new MyString[2]{MyString("hello"), MyString("world")};

            cout << s2[0].tostr() << endl;
            cout << p[0].tostr() << endl;
        }


        static void test2(void(*p)(int)){   //普通指针作为函数参数
            p(10);
        }


        //指向类数据成员的指针
        void test3(){
            Sample s("hello");
            Sample s1("world");

        /*
         * 创建类成员的指针，指向类的一个成员，与普通意义上的指针不一样，存放的是偏移量。
         * ps是一个指针，指向Sample类的成员，具体指向的是Sample类的_name成员
         * 解引用： 使用类成员指针时，需要首先指定类的一个对象，然后通过对象来引用指针所指向的成员。
         */
            string Sample::*ps;
            ps = &Sample::_name;
            Sample* s2 = new Sample("helloworld");

            cout << s.*ps << endl;
            cout << s1.*ps << endl;
            cout << s2->*ps << endl;

            void (Sample::*fpr)() = &Sample::dis;
            void (*sfpr)() = &Sample::display;

            (s.*fpr)();
            (s1.*fpr)();
            (s2->*fpr)();
            sfpr();
        }

    };


    class Widget{
    private:
        void f(int val){ cout << "f() val: " << val << endl;}
        void g(int val){ cout << "g() val: " << val << endl;}
        void (Widget::*fptr[2])(int);

    public:
        explicit Widget(){
            fptr[0] = &Widget::f;
            fptr[1] = &Widget::g;
        };

        void select(int index, int val){
            (this->*fptr[index])(val);
        }

    };


/*
 * const修饰的成员变量，只能在类中初始化或在初始化列表中赋值
 *
 * const对象只能调用const函数, const函数不会修改类内部成员
 * 非const对象优先调用非const函数，
 */
   class Con{
   private:
       const int value{2};
       int a{3};

   public:
       explicit Con(): value(3){

       }

   public:
       void dis() const{
           cout << "dis() const value: " << value << endl;
       }

       void dis() {
           cout << "dis() value: " << value << endl;
       }


   };


/*
 * static成员变量类外存储，存储于data区的rw段
 */
   class Sta{
   private:
       static int value;    //ISO C++ forbids in-class initialization of non-const static member
       static const int val{2};
       static const int v{3};
       int a{2};
   public:
       void dis(){
           cout << "value: " << value << endl;
           cout << "val: " << val << endl;
           cout << "v: " << v << endl;
       }
   };
    int Sta::value = 3; //非常量静态成员只能类外初始化


#endif


}



















#endif //STL_OVERLOAD_H


