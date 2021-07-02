//
// Created by 78472 on 2021/7/2.
//
#include "mset.h"

using namespace std;

void container::set_comp() {

    std::set<int, less<int>> s;
    s.insert({4,3,2,1});
    for(auto &elem: s){
        cout << "elems: " << elem << endl;
    }

    std::set<int, less<int>> s1;
    s1.insert({6,7,8,9});
    for(auto &elem : s1){
        cout << "elems1: " << elem << endl;
    }

    s.swap(s1);

}




















