//
// Created by lhy on 2020/11/14.
//
#include "basic_class.h"


using namespace std;

namespace fundamental{

    void func(void){ cout << "func out of class" << endl;}

    int model::modelGlobal = 2;   //必须在类外声明


    model::model(int a, float b, double c) : a_(a), b_(b){
        cout << "--- constructor(int a, float b) ---" << endl;
        modelGlobal++;
        c_ = new double();
        *c_ = c;
    }

    model::model(const fundamental::model &m) {
        cout << "--- copy constructor ---" << endl;
        modelGlobal++;
        a_ = m.a_;
        b_ = m.b_;
        c_ = new double();
        *c_ = *(m.c_);
    }

    void model::printValue() {
        cout << "a_: " << a_ << " b_: " << b_ << " c_: " << c_ << endl;
    }

    void model::showGlobal() {
        cout << "object create: " << modelGlobal << endl;
        func();
    }


}