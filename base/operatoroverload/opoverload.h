//
// Created by lhy on 2020/12/13.
//

#ifndef STL_OPOVERLOAD_H
#define STL_OPOVERLOAD_H

#include <iostream>
using namespace std;

class op{
public:
    int a_;
    int b_;

public:
    op(int a, int b): a_(a), b_(b){
        cout << "op constructor(int, int)" << endl;
    }

    op operator+(op& c){
        this->op::a_ += c.a_;
        this->op::b_ += c.b_;
        return *this;
    }

    void show(){
        cout << "a_: " << a_ << endl;
        cout << "b_: " << b_ << endl;
    }

};

void optest(){
    op d(1, 2);
    op d1(1, 2);

    d + d1;
    d.show();
    d.operator+(d1);
    d.show();


}
















#endif //STL_OPOVERLOAD_H
