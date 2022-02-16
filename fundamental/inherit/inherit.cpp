//
// Created by lhy on 2020/11/28.
//

#include "inherit.h"

namespace  inheritTest{
    void nameCoverageTest(){
        inherit2Base b;
        b.f1();
        b.f1(1);
        b.base::f1(2);
    }
}



