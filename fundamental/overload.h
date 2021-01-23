//
// Created by lhy on 2021/1/23.
//

#ifndef STL_OVERLOAD_H
#define STL_OVERLOAD_H


#include <iostream>

using namespace std;

namespace fundamental{

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

        Complex& operator=(const Complex& c){
             cout << "Complex assignment =" << endl;
             this->x_ = c.x_;
             this->y_ = c.y_;
             return *this;
         }

        Complex& operator+=(const Complex& c){
            this->x_ += c.x_;
            this->y_ += c.y_;
            return *this;
        }

        void dis(){
            cout << "(" << x_ << "," << y_ << ")" << endl;
        }
    };


/*************************功能测试***************************************************************/
    void overloadtest1(){
        Complex c(10,1), c1(20, 1), c2(30, 1);
        (c += c1) += c2;    //c的最终值是c的初始值加c1、c2的值
        c.operator+=(c1);   //运算符重载的本质是函数重载

        c.dis();
        c1.dis();
        c2.dis();
    }

/******用拷贝构造函数创建形参对象*******/
    void overloadFunc2(Complex c){
        cout << "+++++++" << endl;
        cout << "overloadFunc2" << endl;
    }

    void overloadtest2(){
        Complex c(1,2);
        overloadFunc2(c); //传值，调用了拷贝构造函数
        cout << "+++++++" << endl;
    }


/******形参是引用类型，不创建新的对象*******/
    void overloadFunc3(Complex& c){
        cout << "+++++++" << endl;
        cout << "overloadFunc3" << endl;
    }

    void overloadtest3(){
        Complex c(1, 2);
        overloadFunc3(c);   //引用，没有创建新对象
        cout << "+++++++" << endl;
    }


/******************生成一个新的对象返回*************/
    Complex overloadFunc4(Complex& c){
        cout << "+++++++" << endl;
        return c;
    }

    void overloadtest4(){
        Complex c(1, 2);
        overloadFunc4(c);   //临时变量没有使用，直接销毁
        cout << "+++++++" << endl;
    }


/******************返回引用，没有生成新的对象************************/
    Complex& overloadFunc5(Complex& c){
        return c;
    }

    void overloadtest5(){
        Complex c(1, 2);
        cout << "c_address: " <<  &c << endl;
        cout << "address: " << &overloadFunc5(c) << endl;
        cout << "+++++++" << endl;
    }


    void overloadtest(){
        overloadtest5();
    }




}



















#endif //STL_OVERLOAD_H


