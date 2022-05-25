//
// Created by lhy on 2020/11/18.
//


#include "multi.h"

using namespace std;

int add(int a, int b){

#ifdef DOUBLEADD
    return (a + b) * 2 ;
#else
    return a + b;
#endif
}



