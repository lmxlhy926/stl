//
// Created by 78472 on 2021/7/2.
//
#include "mset.h"

using namespace std;

void container::set_comp() {

    std::set<int, less<>> s;
    s.insert({4,3,2,1});
    for(auto &elem: s){
        cout << "elems: " << elem << endl;
    }

    std::set<int, less<>> s1;
    s1.insert({9,8,7,6});
    for(auto &elem : s1){
        cout << "elems1: " << elem << endl;
    }

    std::set<int, greater<>> s2(s1.cbegin(), s1.cend());
    for(auto &elem : s2){
        cout << "elems2: " << elem << endl;
    }

    std::set<int> s3 = {11,12,13,14};
    std::set<int>::iterator s3iterator;
    s3iterator = s3.find(12);
    cout << "s3.find(12): " << *s3iterator << endl;


}


void container::set_insert_erase(){

    std::set<int> s({1,2,3,4});

    s.insert(5);    //单值插入
    s.insert({8, 9, 10});  //初值列插入
    for(auto &elem : s){
        cout << "third: " << elem << endl;
    }

    s.erase(1);
    s.erase(s.begin(), s.find(8));  //迭代器必须指向合法位置，区间的起始位置不能在结束位置之后
    for(auto &elem : s){
        cout << "fourth: " << elem << endl;
    }

}


void container::set_comp_own(){

    IntSet s({4,3,2,1});    //排序准则是个对象：调用类的default构造函数产生的函数对象
    for(auto &elem : s){
        cout << "elem_first: " << elem << endl;
    }

    IntSet s1(runtimeCmp(1));   //排序准则类的对象
    s1 = {6,7,8,9};
    for(auto &elem : s1){
        cout << "elem_second: " << elem << endl;
    }

//模板参数指定的是排序准则类的类型，实际使用的是类产生的对象
    IntSet s2(s1.begin(), s1.end(), runtimeCmp(1));
    for(auto &elem : s2){
        cout << "elem_third: " << elem << endl;
    }

}

//赋值元素和排序准则对象
void container::set_assign(){

    IntSet s1({1,2,3}, runtimeCmp());
    IntSet s2({6,5,4}, runtimeCmp(1));

    for(auto &elem : s2){
        cout << "elem_first: " << elem << endl;
    }

    s1 = s2;
    for(auto &elem : s1){
        cout << "elem_second: " << elem << endl;
    }

}


















