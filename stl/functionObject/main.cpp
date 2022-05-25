#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

//普通函数
int normalAddFunction(int a, int b){
    return a + b;
}

//函数对象
class addFunctionObject{
public:
    int operator()(int a, int b){
        return a + b;
    }

    int operator()(int a, int b, int c){
        return a + b + c;
    }
};

//成员函数
class memberAddClass{
private:
    int value = 100;
public:
    int add(int a, int b) const{
        return a + b + value;
    }

    int mul(int a, int b) const{
        return a * b + value;
    }

    int div(int a, int b) const{
        return a / b + value;
    }

    static int sub(int a, int b){
        return a - b;
    }

    int addWithNum(int a, int b) const{
        return a + b + value;
    }

    int addWithNum(int a, int b, int c) const{
        return a + b + c + value;
    }
};


//普通函数：函数名是函数的地址，即函数指针变量的值。
void normal_func_pointer(){

    int(*fp)(int, int);        //定义一个函数指针变量
    fp = normalAddFunction;    //给函数指针变量赋值
    std::cout << "fp(1, 2): " << fp(1, 2) << std::endl;

    typedef int(*addFunc)(int, int);     //定义一个函数指针类型
    addFunc fp1 = normalAddFunction;     //创建一个函数指针变量，并为其赋值
    std::cout << "fp1(1, 2): " << fp1(1, 2) << std::endl;

    using addFunc2 = int(*)(int, int);     //定义一个函数指针类型
    addFunc2 fp2 = normalAddFunction;      //创建一个函数指针变量，并为其赋值
    std::cout << "fp2(1, 2): " << fp2(1, 2) << std::endl;
}


//function type wrapper：其值可以为普通函数，函数对象，lamda
void normal_functionObject_cpp(){

    std::function<int(int,int)> f;

    f = normalAddFunction;      //普通函数
    std::cout << "addFunction: " << f(1, 2) << std::endl;

    f = addFunctionObject();    //函数对象
    std::cout << "addFunctionObject: " << f(1, 2) << std::endl;
    std::cout << "addFunctionObject()(1, 2): " << addFunctionObject()(1, 2) << endl;
    std::cout << "addFunctionObject().operator()(1, 2): " << addFunctionObject().operator()(1, 2) << endl;

    f = [](int a, int b) ->int {return a + b;};   //lamda
    std::cout << "lambda: " << f(1, 2) << std::endl;
}


//成员函数：调用成员函数需要对象，参数
void member_function_cpp(){

//静态成员函数本质上就是普通函数
    using staticMemberFunction = int(*)(int, int);
    staticMemberFunction smf = &memberAddClass::sub;    //静态成员函数
    std::cout << "smf(1, 2): " << smf(1, 2) << std::endl;

//成员函数指针
    int (memberAddClass::*mfp)(int, int) const;   //定义一个成员函数指针变量
    mfp = &memberAddClass::add;                   //给成员函数指针变量赋值，对于成员函数c++标准强制要求加上'&'

    using mfpiii_fpType = int(memberAddClass::*)(int, int, int)const;   //定义成员函数指针类型
    mfpiii_fpType mfp1 = &memberAddClass::addWithNum;

    //调用方法1
    memberAddClass a;
    int result = (a.*mfp)(1, 2);        //通过对象，成员函数指针来调用成员函数
    std::cout << "(a.*mfp)(1, 2): " << result << std::endl;

    //调用方法2
    std::function<int(int, int)> f;
    f = std::bind(mfp, a, std::placeholders::_1, std::placeholders::_2);    //将成员函数和对象绑定到一起
    std::cout << "addClass::addWithNum(int, int): " << f(1, 2) << std::endl;

    f = std::bind(mfp1, a, std::placeholders::_1, std::placeholders::_2, 100);
    std::cout << "addClass::addWithNum(int, int, int): " << f(1, 2) << std::endl;
}


void map_function_call(){
    using memfp = int(memberAddClass::*)(int, int)const;   //声明成员函数指针变量类型

//成员函数map
    std::map<string, memfp> fmap;
    fmap.insert(pair<string, memfp>("add", &memberAddClass::add));
    fmap.insert(pair<string, memfp>("mul", &memberAddClass::mul));
    fmap.insert(pair<string, memfp>("div", &memberAddClass::div));

//对象map
    std::map<string, memberAddClass> memMap;
    memberAddClass a;
    memMap.insert(pair<string, memberAddClass>("mem", a));

    string objectOption = "mem";
    string methodOption = "add";
    int input1 = 1;
    int input2 = 2;

    auto obj = memMap.find(objectOption);
    if(obj != memMap.end()){
        auto f = fmap.find(methodOption);
        if(f != fmap.end()){
            std::cout << "===>: " << (obj->second.*f->second)(input1, input2) << std::endl;
        }
    }
}

//成员函数调用
void mem_func_call(){
    //定义memberAddClass的成员函数类型, 成员函数的默认第一实参类型就是定义该成员函数的类
    using memfunctionType = int(memberAddClass::*)(int, int)const;

    memberAddClass a;
    std::function<int(memberAddClass&, int, int)> mf;  //function模板指明的函数类型：第一实参为memberAddClass类对象
    memfunctionType f = &memberAddClass::add;
    mf = f;
    std::cout << "a.add(1, 2): " << mf(a, 1, 2) << std::endl;   //成员函数的调用方法1（更接近本质的调用）
    std::cout << "a.add(1, 2): " << (a.*f)(1, 2) << std::endl;  //成员函数的调用方法2
}


class print{
public:
    print(){
        std::cout << "print constructor() " << std::endl;
    }

    ~print(){
        std::cout << "print destructor() " << std::endl;
    }

    void echo(){
        std::cout << "message from print" << std::endl;
    }
};

//模板本质上是一种编译期替换
template<typename T>
void test(){
    T().echo();
}


int main(int argc, char* argv[]){

    test<print>();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}



