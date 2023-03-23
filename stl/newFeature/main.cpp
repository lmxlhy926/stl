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
    sampleA(std::initializer_list<int>){    //接受一个初值列作为参数
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
//------------------------------------------------------------------------------------------------------



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
//------------------------------------------------------------------------------------------------------


/*
 * range-based for循环
 */

class RangeBase{
public:
    RangeBase(){
        std::cout << "RangeBase()..." << std::endl;
    }
    explicit RangeBase(int a){
        std::cout << "RangeBase(int a)..." << std::endl;
    }
    RangeBase(const RangeBase& base){
        std::cout << "RangeBase(const RangeBase& base)..." << std::endl;
    }
    RangeBase& operator= (const RangeBase& other){
        std::cout << "operator= ..." << std::endl;
        return *this;
    }
    ~RangeBase(){
        std::cout << "~RangeBase()..." << std::endl;
    }
};

void range_based_for(){
    for(int i : {1, 2, 3}){
        std::cout << i << std::endl;
    }

    RangeBase rbase;
    std::vector<RangeBase> vec;
    vec.reserve(10);
    vec.emplace_back(rbase);      //创建临时对象，拷贝构造，临时对象析构
    vec.emplace_back(rbase);
    std::cout << "--------" << std::endl;

    for(RangeBase rb : vec){    //针对每个元素会调用拷贝构造函数、析构函数

    }
    std::cout << "--------" << std::endl;


    for(RangeBase& rb : vec){   //使用引用，避免每个元素的拷贝构造函数和析构函数

    }
    std::cout << "--------" << std::endl;


    const RangeBase rbValue = rbase;             //拷贝构造
    const RangeBase& rbReference = rbase;        //引用赋值
}


/*
 * range-base for循环的本质定义式
 * 本质上是转换为下列2种格式进行处理的
 */
void range_based_for1(){
    std::vector<int> vec;

    for(auto pos = vec.begin(); pos != vec.end(); ++pos){
        const auto elem = *pos;       //值访问，会构造一个临时对象，使用完后析构掉
        std::cout << elem << std::endl;
    }

    for(auto pos = vec.begin(); pos != vec.end(); ++pos){
        const auto& elem = *pos;    //引用访问，不会构造临时对象
        std::cout << elem << std::endl;
    }
}
//------------------------------------------------------------------------------------------------------


class MoveClass{
public:
    MoveClass(){
        std::cout << "MoveClass()..." << std::endl;
    }
    MoveClass(const MoveClass& mc){
        std::cout << "MoveClass(const MoveClass& mc)..." << std::endl;
    }
    MoveClass(MoveClass&& mv) noexcept {
        std::cout << "MoveClass(MoveClass&& mv)..." << std::endl;
    }
    ~MoveClass(){
        std::cout << "~MoveClass()..." << std::endl;
    }
};

/*
 * std::move()自身并不做任何moving工作，它只是将其实参转换为一个所谓的rvalue reference，那是一种申明为X&&的类型。
 * 必须确保对于被传对象的任何改动——特别是析构——都不至于冲击新对象的状态。
 */
void move(){
    MoveClass mc;                   //default构造函数创建一个新对象
    std::vector<MoveClass> vec;
    vec.reserve(10);
    vec.push_back(mc);              //调用拷贝构造函数创建一个新对象
    vec.push_back(std::move(mc));   //调用move构造函数创建一个新对象
}/* 3个对象析构 */

/*
 * 如果只实现void foo(X&), 而没有实现void foo(X&&);
 *      foo()可因lvalue但不能因rvalue被调用；
 *
 * 如果实现void foo(const X&), 而没有实现void foo(X&&);
 *      foo()可因lvalue也可因rvalue被调用
 *
 * 如果实现void foo(X&), void foo(X&&); 或 void foo(const X&), void foo(X&&);
 *      可以区分"为rvalue服务"和"为lvalue服务"
 *
 * 如果只实现void foo(X&&)
 *      foo()可因右值被调用，当尝试以lvalue调用它，会触发编译错误。
 */

void lrvalue(MoveClass& m){         //只能是左值
    std::cout << "lrvalue(MoveClass& m)..." << std::endl;
}

void lrvalue(const MoveClass& m){   //左值、右值都可以
    std::cout << "lrvalue(const MoveClass& m)..." << std::endl;
}

void lrvalue(MoveClass&& m){       //只能是右值
    MoveClass mc = std::move(m);
    std::cout << "lrvalue(MoveClass&& m)..." << std::endl;
}


int main(int argc, char* argv[]){
    lrvalue(MoveClass());

    return 0;
}






































