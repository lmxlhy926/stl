//
// Created by 78472 on 2022/1/16.
//

#include "sharePointer.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>



using namespace std;
void test1(){
    //RAII：获取资源即初始化，可以提供析构函数
    std::shared_ptr<string> sp(new string("hello"), [](string* p){
        std::cout << "delete: " << *p << endl;
        delete p;
    });

    //复制shared_ptr对象，内部采用引用计数
    std::vector<shared_ptr<string>> vec;
    vec.push_back(sp);
    vec.push_back(sp);

    //访问管理的对象的内容
    (*sp)[0] = 's';
    sp->replace(1,1,"b");
    cout << "first element of vec: " << *(vec[0].get()) << endl;
    cout << "second element of vec: " << vec[0]->c_str() << endl;

    //删除对象
    vec.clear();
    cout << "===>vec.clear()" << endl;
    sp = nullptr;   //丧失最后一个引用导致，析构会释放分配的对象
    cout << "===>sp = nullptr" << endl;
}


void test2(){
    try{
        shared_ptr<string> sp(new string("newstring"), [](string* sp){
            cout << "delete: " << *sp << endl;
            delete sp;
        });
        weak_ptr<string> wp = sp;
        sp.reset();
        cout << "wp.use_count: " << wp.use_count() << endl;
        cout << std::boolalpha << wp.expired() << endl;
        shared_ptr<string> p(wp);

    }catch(const std::exception& e){
        cout << "exception: " << e.what() << endl;
    }
}

























