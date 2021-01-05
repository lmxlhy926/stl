//
// Created by lhy on 2021/1/5.
//

#include <iostream>

using namespace std;

namespace mstream{


/*
 * 如果设置了failbit, 除非明确予以清除, 否则无法进入下一个操作.
 */
    void statetest(){

        int k{100};

        if(!(std::cin >> k))
            cout << "error happens" << endl;
        cout  << "k: " << k << endl;

        if(cin.rdstate() & std::ios::eofbit){
            cout << "eofbit was set" << endl;

        }else if(cin.rdstate() & std::ios::failbit){
            cout << "failbit was set" << endl;

        }else if(cin.rdstate() & std::ios::badbit)
            cout << "badbit was set" << endl;

        std::cin.clear();
        if(std::cin.good())
            cout << "清除所有errors" << endl;

    }







}