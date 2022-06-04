//
// Created by 78472 on 2022/5/28.
//

#include <iostream>
#include <string>
#include "inherit.h"

using namespace fundamental;

//派生类作用域遮掩基类作用域
void nameCoverageTest(){
    inherit2Base b;
    b.f1();
    b.base::f1();
    b.f1(1);
    b.base::f1(1);
}


int main(int argc, char* argv[]){
    string str = "hello";
    Inherit a(str);
    return 0;
}














