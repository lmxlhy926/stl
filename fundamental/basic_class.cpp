//
// Created by lhy on 2020/11/14.
//
#include "basic_class.h"

#include <utility>


using namespace std;

namespace fundamental{

    void printMessage(){ cout << "object created ==> ";}

    int model::modelGlobal = 0;   //必须在类外声明

    model::model(int a, float b, string commentstr) : a_(a), cm(std::move(commentstr)){
        cout << "===>model constructor(int, float, commentstr)" << endl;
        modelGlobal++;
        b_ = new double();
        *b_ = b;
    }

    model::model(const fundamental::model &m) : cm("hello"){
        cout << "===>model copy constructor " << endl;
        modelGlobal++;
        a_ = m.a_;
        b_ = new double();
        *b_ = *(m.b_);
    }

    void model::printValue() {
        cout << "a_: " << a_ << endl;
        cout << "*b_: " << *b_ << endl;
        cout << "max: " << range.max << endl;
        cout << "min: " << range.min << endl;
        cm.showStr();
    }

    model model::modelChange(model &m) {
        this->a_ = m.a_;
        *(this->b_) = *(m.b_);
    }

    void model::lamda(){
        int lamda_x = 222;
        int lamda_y = 333;

        auto l = [lamda_x, this, &lamda_y] () -> bool {     // 指定capture用来处理外部作用域内未被传递为实参的数据
            cout << "a_: " << a_ << endl;
            cout << "x: " << lamda_x << endl;
            cout << "y: " << lamda_y << endl;
            return true;
        };
        l();
        lamda_x = 2222;     //传递普通数值
        this->a_ = 1111;    //传递地址
        lamda_y = 3333;     //传递地址
        l();
    }

    void model::showGlobal() {
        printMessage();
        cout << modelGlobal << endl;
    }

    void basicClassTest(){
        model m(1, 2, "hello");
        m.lamda();
    }

}