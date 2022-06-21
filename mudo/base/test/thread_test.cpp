//
// Created by 78472 on 2022/6/20.
//

#include <iostream>
#include <cstdio>
#include "base/Thread.h"

using namespace muduo;

//空参函数
void func1(){
    for(int i = 0; i < 10; i++){
        printf("%d-------------func1\n", i);
    }
}

//带参函数
void func2(int a){
    for(int i = 0; i < 10; i++){
        printf("%d-------------func2\n", i);
    }
}

//成员函数
class Foo{
private:
    int x_;
public:
    explicit Foo(int x) : x_(x){}

    void memFunc1() const{
        printf("========memFunc1----%d\n", x_);
    }

    void memFunc2(string& str) const{
        printf("========memFunc2----%s----%d\n", str.c_str(), x_);
    }
};


int main(int argc, char* argv[]){
    Thread t1(func1, "func1");
    t1.start();
    std::cout << "t1.join: " << t1.join() << std::endl;

    Thread t2([](){ func2(40); }, "func2");
    t2.start();
    std::cout << "t2.join: " << t2.join() << std::endl;

    Foo f(20);
    Thread t3([&](){ f.memFunc1();}, "t3");
    t3.start();
    std::cout << "t3.join: " << t3.join() << std::endl;

    Thread t4(std::bind(&Foo::memFunc2, &f, std::string("hello")), "t4");
    t4.start();
    std::cout << "t4.join: " << t4.join() << std::endl;


    return 0;
}
