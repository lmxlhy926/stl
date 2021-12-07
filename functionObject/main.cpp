#include <iostream>
#include <functional>
#include <map>
#include <vector>

using namespace std;
using namespace std::placeholders;

//普通函数
int normalAddFunction(int a, int b){
    return a + b;
}

//函数对象
template<typename T>    //传递类型名为模板参数
class FunctionObject{
private:
    T value;
public:
    explicit FunctionObject(const T& n) : value(n){}

public:
    int operator()(int a, int b){
        return a + b + value;
    }

    int operator()(int a, int b, int c){
        return a + b + c + value;
    }
};


//成员函数
template<int intValue>  //传递参数值为模板参数
class memberAddClass{
private:
    int value = intValue;
public:
    int add(int a, int b){
        return a + b;
    }

    int mul(int a, int b){
        return a * b;
    }

    int addWithNum(int a, int b, int c){
        return a + b + c + value;
    }

    static int sub(int a, int b){   //静态成员函数 == 普通函数
        return a - b;
    }
};




/*
 * 函数指针的三种定义方式，函数指针的引用
 */
void normal_func_pointer(){

    int(*fp)(int, int);
    fp = normalAddFunction;
    std::cout << "fp(1, 2): " << fp(1, 2) << std::endl;

    typedef int(*p2func)(int, int);
    p2func fp1 = normalAddFunction;
    std::cout << "fp1(1, 2): " << fp1(1, 2) << std::endl;

    using p2funciii = int(*)(int, int);
    p2funciii fp2 = normalAddFunction;
    std::cout << "fp2(1, 2): " << fp2(1, 2) << std::endl;
}


//成员函数指针
void member_funcitonPtr() {

    //静态成员函数本质上就是普通函数
    using staticMemberFunction = int (*)(int, int);
    staticMemberFunction smf = &memberAddClass<0>::sub;    //静态成员函数
    std::cout << "smf(1, 2): " << smf(1, 2) << std::endl;

    //成员函数指针
    memberAddClass<0> a;
    int (memberAddClass<0>::*mfp)(int, int) = &memberAddClass<0>::add;  //对于成员函数c++标准强制要求加上'&'
    std::cout << "(a.*mfp)(1, 2): " << (a.*mfp)(1, 2) << std::endl;     //通过对象，成员函数指针来调用成员函数
}


//对象、成员函数指针抽象调用成员方法
void abstract_object_memberFunction(){
    using memfp = int(memberAddClass<0>::*)(int, int);   //声明成员函数指针变量类型

    //成员函数map
    std::map<string, memfp> functionMap;
    functionMap.insert(pair<string, memfp>("add", &memberAddClass<0>::add));
    functionMap.insert(pair<string, memfp>("mul", &memberAddClass<0>::mul));

    //对象map
    std::map<string, memberAddClass<0>> objectMap;
    memberAddClass<0> a;
    objectMap.insert(pair<string, memberAddClass<0>>("memberAddClass", a));

    string object = "memberAddClass";
    string memFunc = "add";

    //通过对象、成员函数指针来调用对象的成员方法
    auto obj = objectMap.find(object);
    if(obj != objectMap.end()){
        auto func = functionMap.find(memFunc);
        if(func != functionMap.end()){
            std::cout << "===>: " << ((obj->second).*(func->second))(1, 2) << std::endl;
        }
    }
}


/*
 * function type wrapper
 * std::function对象的三种赋值方式
 */
void function_wrapper(){

    std::function<int(int,int)> f;

    f = normalAddFunction;    //普通函数
    std::cout << "normalAddFunction: " << f(1, 2) << std::endl;

    f = FunctionObject<int>(0);    //创建一个函数对象实体
    std::cout << "FunctionObject(int, int): " << f(1, 2) << std::endl;
    std::cout << "FunctionObject(int, int): " << FunctionObject<int>(0)(1, 2) << std::endl;

    f = [](int a, int b) ->int {return a + b;};     //lamda
    std::cout << "lambda: " << f(1, 2) << std::endl;
}


/*
 * bind的用法
 *      1. 绑定普通函数
 *      2. 绑定函数对象
 *      3. 绑定成员函数
 */
void bindUsage(){
    FunctionObject<int> a(0);
    memberAddClass<0> b;

    std::cout << "bind(func, arg): " << bind(normalAddFunction, _1, _2)(1, 2) << std::endl;
    std::cout << "bind(object, arg): " << bind(a, _1, _2)(1, 2) << std::endl;
    std::cout << "bind(memFunc, obj, arg...): " << bind(&memberAddClass<0>::add, _1, _2, _3)(b, 1, 2) << endl;
    std::cout << "mem_fn(memFunc)(obj, arg..): " << mem_fn(&memberAddClass<0>::add)(b, 1, 2) << endl;

}


int main(int argc, char* argv[]){


    return 0;
}