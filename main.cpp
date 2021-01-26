//
// Created by lhy on 2020/11/10.
//

#include <iostream>
#include "stlString.h"
#include "basic_class.h"
#include "inherit.h"
#include "poly.h"
#include "callableObj.h"
#include "fileaccess.h"
#include "opoverload.h"
#include "mthread.h"
#include "mmutex.h"
#include "condition.h"
#include "matomic.h"
#include "stream.h"
#include "overload.h"

#include <thread>
#include <future>

using namespace std;
using namespace fundamental;
using namespace mthread;
using namespace mstream;

void func(int a){
    std::cout << "a: " << a << endl;
}

int main(int argc, char *argv[]){

//    MyString m1("hello");
//    MyString m2("world");
//    MyString m3;
//
//    cout << (m1 + m2).tostr() << endl;
//    cout << m1[0] << endl;
//    m3 = m2 = m1;
//    cout << m3.tostr() << endl;

//    ReferenceTest::overloadtest7();



//    MyStringArrayTest::test2(func);

//    MyStringArrayTest t;
//    t.test3();

//    Widget g;
//    g.select(0, 10);
//    g.select(1, 20);

//    const Con c;


    Sta s;
    s.dis();
    cout << "sizeof(Sta): " << sizeof(s) << endl;


    return 0;
}