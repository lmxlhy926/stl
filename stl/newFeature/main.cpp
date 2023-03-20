//
// Created by 78472 on 2023/3/20.
//

#include <iostream>
#include <vector>
using namespace std;

/*
 *  nullptr是个新关键字。它被自动转换为各种pointer类型，但不会被转换为任何整数类型。它拥有类型std::nullptr_t。
 *  std::nullptr_t被视为一个基础类型
 */
void f(int){
    std::cout << "f(int)" << std::endl;
}

void f(char*){
    std::cout << "f(void*)" << std::endl;
}

void f(int*){
    std::cout << "f(int*)" << std::endl;
}

void f(std::nullptr_t){
    std::cout << "f(std::nullptr_t)" << std::endl;
}
//------------------------------------------------------------------------------------------------------



/*
 *  初始化可因为小括号、大括号或赋值操作符的出现而发生
 *  c++11引入了“一致性初始化”的概念，意思是面对任何初始化动作，可以使用相同语法，也就是使用大括号。
 *
 */
void print(std::initializer_list<int> vals){
    for(auto pos = vals.begin(); pos != vals.end(); pos++){
        std::cout << *pos << std::endl;
    }
}

class sampleA{
public:
    sampleA(){
        std::cout << "sampleA()..." << std::endl;
    }
    sampleA(int, int){
        std::cout << "sampleA(int, int)...." << std::endl;
    }
    sampleA(std::initializer_list<int>){
        std::cout << "sampleA(std::initializer_list<int>)..." << std::endl;
    }
};

void uniformInitialization(){
    int values[] {1, 2, 4};
    std::vector<int> v {1, 2, 3, 4, 5};
    std::vector<string> cities {"new york", "london", "beijing"};

    /*
     * 初值列会强迫造成所谓的value initialization,
     * 意思是即使某个local变量属于某种基础类型（通常会有不明确的初值), 也会被初始化为0（或nullptr, 如果它是个pointer的话）
     */
    int i;      //  i has undefined value
    int j{};    //  j is initialized by 0
    int* p;     //  p has undefined value
    int* q{};   //  q is initialized by nullptr

    sampleA sa{};
    sampleA sa1(1, 2);
    sampleA sa2{1, 2};      //优先调用初值列函数
    sampleA sa3 = {1, 2};   //优先调用初值列函数
    sampleA sa4({1, 2});    //优先调用初值列函数
}

class p{
public:
    p(int a, int b){
        std::cout << "p(int a, int b)" << std::endl;
    }
    explicit p(int a, int b, int c){
        std::cout << "p(int a, int b, int c)" << std::endl;
    }
};

void fp(const p&){}

void uniform(){
    p x(1, 2);
    p x1{1, 2};
    p x2 = {1, 2};
    p x3{1, 2, 3};
    //p x4 = {1, 2, 3};

    fp({1, 2});             // implicit conversion of {1, 2} into p
    //fp({1, 2, 3});              // ERROR due to explicit
    fp(p{1, 2});            // explicit conversion of {1, 2} into p
    fp(p{1, 2, 3});      // explicit conversion of {1, 2, 3} into p
}



/*
 * lambda
 */
void lamda(){
    []{
        std::cout << "hello lambda..." << std::endl;
    }();

    auto l = []{ std::cout << "hello lambda..." << std::endl;};
    l();

    int x, y;
    x = y = 10;
    auto q = [x, &y]() mutable {
        x++;    //passed by value，必须声明mutable才能更改其值
        y++;
        std::cout << "x: " << x << std::endl;
    };
    q();
    q();
    std::cout << "x: " << x << ", y: " << y << std::endl;

    auto qq = [=, &y]{
        //y捕获引用，其它捕获值
    };
    qq();
}



int main(int argc, char* argv[]){
    lamda();

    return 0;
}






































