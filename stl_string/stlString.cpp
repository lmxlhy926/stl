//
// Created by lhy on 2020/11/10.
//

#include "stlString.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

void stringConstruct(){
//c-string构造string
    string s("string"); //c-string的cstr
    string s2("string", 2); //字符数组的前n个字符
    string s3;  //空string
    //string, st,

//string构造string, position + num
    string s4(s);   //拷贝构造函数
    string s5(s, 3);    //指定起始位置
    string s6(s, 2, 3); //指定起始位置以及字符数量, 长度顶多为n.
    string s7(s, 2, 100);
    //string, ing, rin, ring

//n个字符, 迭代器, 初值列
    string s8(2, 'c');  //n个字符
    string s9(s.cbegin(), s.cend()); //迭代器指示位置
    string s10({'a', 'b','c'});  //初值列
    //cc, string, abc
}

/*
 * c++标准将string literal(字符串字面常量)的类型由char*更改为const char*.
 * 为保证向后兼容性c++提供了一个隐式转换, 可从const char*隐式转换为char*
 * 存在一个const char*到string的隐式类型转换,不存在string object到C-string的自动类型转换.
 */

void str_cstring(){
//    string s("abcd");
//    cout << "s.size(): " << s.size() << endl;
//    char *p = const_cast<char *>(s.c_str());
//    *p = 'c';
//    cout << p << endl;

    string s("ab");
    string s1(1, '\0');
    s = s + s1 + "cd";
    cout << s.size() << endl;
    cout << strlen(s.c_str()) << endl;
}