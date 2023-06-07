
#include <iostream>
using namespace std;

void print(const string& message, const string& printStr){
    std::cout << message << ": " << printStr << std::endl;
}

void stringConstruct(){
//c-string构造string
    string s("string");         //c-string的cstr
    string s2("string", 2);     //字符数组的前n个字符
    string s3;                  //空string
    
//用substr来构造
    string s4(s);       //拷贝构造函数
    string s5(s, 3);    //指定起始位置
    string s6(s, 2, 3); //指定起始位置 + 字符数量, 长度顶多为n.
    string s7(s, 2, 100);

//n个字符, 迭代器, 初值列
    string s8(2, 'c');                  //n个字符
    string s9(s.cbegin(), s.cend());    //迭代器指示位置
    string s10({'a', 'b','c'});         //初值列

    print("s", s);
    print("s2", s2);
    print("s3", s3);
    print("s4", s4);
    print("s5", s5);
    print("s6", s6);
    print("s7", s7);
    print("s8", s8);
    print("s9", s9);
    print("s10", s10);
}


int main(int argc, char* argv[]){

    stringConstruct();

    return 0;

}








