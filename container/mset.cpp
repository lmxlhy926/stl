//
// Created by 78472 on 2021/7/2.
//
#include "mset.h"

using namespace std;




void container::set_insert_erase(){

    std::set<int, greater<>> a({6,7,8,9});
    std::set<int> s({1,2,3,4});

    s.insert(a.begin(), a.end());   //区间插入（区间只是确定有哪些值，排序还是由当前的排序准则确定）
    s.insert(5);    //单值插入
    s.insert({10,11,12});  //初值列插入
    for(auto &elem : s){
        cout << "elem: " << elem << endl;
    }

    int count = s.erase(1); //删除特定值，返回删掉的数量
    s.erase(s.begin(), s.find(8));  //迭代器必须指向合法位置，区间的起始位置不能在结束位置之后
    for(auto &elem : s){
        cout << "fourth: " << elem << endl;
    }

    std::set<int>::iterator ite;
    ite = s.find(8);
    if(ite != s.end()){   //判断是否找到
        cout << "s.find(8): " << *ite << endl;
    }
    cout << "the count of erase(1): " << count << endl;

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
    IntSet s2(s1.begin(), s1.end(), runtimeCmp());
    for(auto &elem : s2){
        cout << "elem_third: " << elem << endl;
    }
}


//赋值元素和排序准则的对象
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


















