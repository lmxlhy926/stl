//
// Created by lhy on 2020/11/10.
//

#include "stlString.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>

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
 *
 * data()和c_str()以数组形式返回string内容, 该数组在位置[size()]上有一个end-of-string字符,所以其结果是一个内含'\0'字符的有效C-string.
 * data()和c_str()返回的字符数组由该string拥有,调用者不因该改动它或者释放其内存.
 *
 * copy()将string内容复制到“调用者提供的字符数组中”，其末尾不添加'\0'字符。
 *
 */

void str_cstring(){

    string s("123456789");
    char p[100] = {};
    s.copy(p, 2, 2);    //最多复制的字符数量, 起始位置
    cout << p << endl;
    cout << s.c_str() << endl;
    cout << s.data() << endl;

    string s1(1, '\0');
    s1 = s + s1 + "-";
    cout << s1.size() << endl; //string中'\0'没有特殊含义, 当作一个普通字符对待
    cout << strlen(s1.c_str()) << endl; //C-string中, ’\0‘用来标示结束
}



/*
 * size()和length()：string的现有字符数
 *
 * max_size()：string最多能够包含的字符数, 一般是索引类型之最大值减1.
 *    原因：（1）最大值本身是npos;
 *         (2) 具体实现中可因此轻易在内部缓冲区之末尾添加一个'\0', 以便将这个string当作C-string使用.
 * capacity(): 重新分配内存之前, string所能包含的最大字符数.
 *
 * reserve(): 用来避免重分配行为, 它使你得以预留一定容量, 并确保该容量用尽之前reference一直保持有效.
 *       注：拿一个小于现有容量的实参来调用reserve(), 实际上就是一种非强制性所建请求. 是否响应该请求由具体的实现来决定。
 */
void str_sizeCapacity(){
    string s("abcd");
    cout << "s.size(): " << s.size() << endl;
    cout << "s.length(): " << s.length() << endl;
    cout << "s.max_size(): " << s.max_size() << endl;
    cout << "s.capacity(): " << s.capacity() << endl;
    s.reserve(80);
    cout << "after reserve s.capacity(): " << s.capacity() << endl;

//    s.size(): 4
//    s.length(): 4
//    s.max_size(): 9223372036854775807
//    s.capacity(): 15
//    after reserve s.capacity(): 80
}

/*
 * string允许我们对其所包含的字符进行读写.访问单一字符的2种方法：[], at().
 * operater[]不检查索引是否有效, 当指定的索引无效时其行为不明确——可能访问非法内存,引起某些讨厌的边缘效应甚至崩溃.
 * at()检查索引是否有效, 当索引无效时系统会抛出out_of_range异常.
 * front(),back()分别用来访问第一字符和最末字符.
 *
 * 空string: front()返回'\0', back()会导致不明确的行为.
 * 当前字符个数： 对于[]来说是一个有效索引, 返回由字符类型之default构造函数所产生的字符, 对于string来说就是'\0'
 *
 * 为了允许改动string的内容, operator[]的non-const版本以及at(),front(),back()都允许返回字符的reference.
 * 但是一旦string的内存发生重分配行为, 那些reference立即失效.
 */
void str_elementAccess(){
    string s("abcdefg");
    char &c = s[0];  //引用
    char *p = &s[1]; //指针
    c = 'q';
    *p = 'w';
    cout << s << endl; //qwcdefg

    string s1("abc");
    s1[0] = 'q'; //s1[0]为char&类型
    cout << s1 << endl; //qbc

    string s2;
    cout << s2.front() << endl;

//引用是变量的别名, 不占用运行程序的内存空间, 即可执行程序不会为它分配栈空间或者堆空间.
    char pp = '3';
    char &ppp = pp;
    printf("%p\n", &pp);  //0x7ffd211aa827
    printf("%p\n", &ppp); //0x7ffd211aa827

}
















