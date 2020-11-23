//
// Created by lhy on 2020/11/14.
//
#include "basic_class.h"


using namespace std;

namespace fundamental{

    void func(){ cout << "func out of class" << endl;}

    int model::modelGlobal = 0;   //必须在类外声明

    model::model(int a, float b, double c) : a_(a), b_(b), cm("hello"){
        cout << "===>model constructor(int a, float b) ---" << endl;
        modelGlobal++;
        c_ = new double();
        *c_ = c;
    }

    model::model(const fundamental::model &m) : cm("hello"){
        cout << "===>model copy constructor " << endl;
        modelGlobal++;
        a_ = m.a_;
        b_ = m.b_;
        c_ = new double();
        *c_ = *(m.c_);
    }

    void model::printValue() {
        cout << "a_: " << a_ << endl;
        cout << "b_: " << b_  << endl;
        cout << "*c_: " << *c_ << endl;
        cout << "max: " << range.max << endl;
        cout << "min: " << range.min << endl;
        cm.showStr();
    }

    model model::modelChange(model &m) {
        this->a_ = m.a_;
        this->b_ = m.b_;
        *(this->c_) = *(m.c_);
    }

    void model::lamda(){
        int lamda_x = 222;
        int lamda_y = 333;

        auto l = [lamda_x, this, &lamda_y] () -> bool {     // 指定capture用来处理外部作用域内未被传递为实参的数据
            cout << "x: " << lamda_x << endl;
            cout << "a_: " << this->a_ << endl;
            cout << "y: " << lamda_y << endl;
            return true;
        };
        l();
        lamda_x = 2222;
        this->a_ = 1111;
        lamda_y = 3333;
        l();
    }


    void model::showGlobal() {
        cout << "object create: " << modelGlobal << endl;
    }

    void basicClassTest(){
        model m(1, 2,3 );
//        m.printValue();
//        model::showGlobal();
//
//        model::comment cm("sss");
//        cm.showStr();

        m.lamda();


    }



}