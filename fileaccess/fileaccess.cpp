//
// Created by lhy on 2020/12/3.
//

#include <fstream>
#include <iostream>

using namespace std;

namespace fileaccess{

    std::ofstream func2(const std::string& filename){

        return ofstream(filename, std::ios::app);
    }


    void func3(){
        ifstream file("a.txt");
        char c;
        while(file.get(c)){
            cout.put(c);
        }
        cout << "------" << endl;

        string s;
        file >> s;
        cout << s << endl;

    }



    void func1(void){

        ofstream file("a.txt", std::ios::out);

        if(file){
            file << "hello world" << endl;
        }

        ofstream("b.txt") << "hello";
        ofstream("b.txt", std::ios::app) << " world" << endl;

        std::ofstream of = func2("c.txt");
        of << "aa bb cc dd" << endl;

    }

    void func4(){
        //通过seek来切换读写模式, 默认是在读模式下
        fstream ifs("a.txt", std::ios::in | std::ios::app);
        ifs.seekp(ifs.tellg());
        ifs << "helloaaa" << endl;

        char array[100];
        ifs.seekg(std::ios::beg);
        ifs >> array;
        cout << array;
    }




}
