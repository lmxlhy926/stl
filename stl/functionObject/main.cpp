#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;


/*
 * callable object
 *      1.一个函数, 接受额外传入的args作为实参
 *      2.一个指向成员函数的指针,当通过对象调用它, 该对象被传递为第一实参(必须是个reference或pointer),  其它实参则一一对应成员函数的参数
 *      3.一个函数对象, 附带的args被传递作为实参
 *      4.一个lamda, 严格的说它是一种函数对象
 */


//普通函数
int addNormal(int a, int b){
    return a + b;
}

//函数对象：重载函数调用()
class AddFuncObject{
private:
    string printStr;
public:
    explicit AddFuncObject(string str): printStr(std::move(str)){}

    AddFuncObject(const AddFuncObject& af){
        std::cout << "copy ctr AddFuncObject" << std::endl;
    }

    ~AddFuncObject(){ std::cout << "~AddFuncObject：" << printStr << std::endl; }

    int operator()(int a, int b){
        std::cout << "operator()(int a, int b): " << printStr << std::endl;
        return a + b;
    }

    int operator()(int a, int b, int c){
        std::cout << "operator()(int a, int b, int c): " << printStr << std::endl;
        return a + b + c;
    }
};

//成员函数
class Arithmetic{
private:
    string printStr;
public:
    explicit Arithmetic(string str) : printStr(std::move(str)){}

    ~Arithmetic(){ std::cout << "~Arithmetic：" << printStr << std::endl; }

    int add(int a, int b){
        std::cout << "add(int a, int b): " << printStr << std::endl;
        return a + b;
    }

    int sub(int a, int b){
        std::cout << "sub(int a, int b): " << printStr << std::endl;
        return a - b;
    }

    int mul(int a, int b) const{
        std::cout << "mul(int a, int b): " << printStr << std::endl;
        return a * b;
    }

    int div(int a, int b) const{
        std::cout << "div(int a, int b): " << printStr << std::endl;
        return a / b;
    }

    static void show(int a, int b){
        std::cout << "a: " << a << ", b " << b <<std::endl;
    }

    void setPrintStr(string str){
        printStr = std::move(str);
    }
};


/**
 * 函数名是函数所在代码段的地址，即函数指针变量的值。
 * 4种声明函数指针变量的方式：本质上是2种：函数指针；函数覆盖器
 *      1. int(*nfp)(int, int);
 *      2. typedef int(*addFunc)(int, int);
 *      3. using addFunc2 = int(*)(int, int);
 *      4. using ArithmeticFunc = std::function<int(int, int)>
 */
void callNormalFunc(){
    //直接调用
    std::cout << "add(1, 2): " << addNormal(1, 2) << std::endl;

    //通过函数指针变量调用
    int(*nfp)(int, int);          //声明
    nfp = addNormal;              //赋值
    std::cout << "nfp(1, 2): " << nfp(1, 2) << std::endl;

    typedef int(*addFunc)(int, int);    //定义类型
    addFunc nfp1 = addNormal;           //声明并赋值
    std::cout << "nfp1(1, 2): " << nfp1(1, 2) << std::endl;

    using addFunc2 = int(*)(int, int);     //定义类型
    addFunc2 nfp2 = addNormal;             //声明并赋值
    std::cout << "nfp2(1, 2): " << nfp2(1, 2) << std::endl;

    //函数覆盖器
    using ArithmeticFunc = std::function<int(int, int)> ;
    ArithmeticFunc add_normal = addNormal;
    std::cout << "add_normal(1, 2): " << add_normal(1, 2) << std::endl;
}


/**
 * 通过函数对象来完成函数调用:
 *     1. 创建对象：
 *     2. 调用对象的函数(重载函数调用符'()')
 */
void callFunctionObject(){
    AddFuncObject afo("AddFuncObject-afo");
    afo(1, 2);
    afo(1, 2, 3);
    afo.operator()(1, 2);
    afo.operator()(1, 2, 3);
    std::cout << "--------------" << std::endl;

    //创建临时对象来调用函数, 临时对象使用完会被立即析构
    AddFuncObject("AddFuncObject1")(1, 2);
    AddFuncObject("AddFuncObject2")(1, 2, 3);
    AddFuncObject("AddFuncObject3").operator()(1, 2);
    AddFuncObject("AddFuncObject4").operator()(1, 2, 3);
    std::cout << "--------------" << std::endl;

    //函数覆盖器
    using ArithmeticFunc = std::function<int(int, int)> ;
    ArithmeticFunc add_object = AddFuncObject("AddFuncObject5");
    add_object(1, 2);
}


/**
 *      1. 定义并赋值指向类函数的指针变量
 *      2. 通过对象，指向类函数的指针变量来访问该对象的函数
 */
void callMemberFunc(){
    //静态成员函数：普通函数 + 类作用域
    using staticMemberFunction = void(*)(int, int);
    staticMemberFunction smf = Arithmetic::show;    //静态成员函数: 携带类作用域的普通函数
    smf(1, 2);

    //成员函数
    using ArithmeticMemberFunc = int(Arithmetic::*)(int, int);   //指向类成员函数的指针，定义时携带类作用域
    ArithmeticMemberFunc add = &Arithmetic::add;                 //给成员函数指针变量赋值，对于成员函数c++标准强制要求加上'&'
    ArithmeticMemberFunc sub = &Arithmetic::sub;

    using ArithmeticMemberFuncConst = int(Arithmetic::*)(int, int) const;
    ArithmeticMemberFuncConst mul = &Arithmetic::mul;
    ArithmeticMemberFuncConst div = &Arithmetic::div;

    //通过对象调用方法
    Arithmetic a("Arithmetic a");
    (a.*add)(1, 2);
    (a.*sub)(1, 2);

    Arithmetic b("Arithmetic b");
    (b.*mul)(1, 2);
    (b.*div)(1, 2);
    std::cout << "-----------" << std::endl;

    using ArithmeticFunc = std::function<int(int, int)> ;
    ArithmeticFunc add_mem_bind = std::bind(add, a, std::placeholders::_1, std::placeholders::_2);
    ArithmeticFunc add_mem_lamda = [&](int first, int second){
        return b.add(first, second);
    };
    add_mem_bind(1, 2);
    add_mem_lamda(1, 2);
    std::cout << "-----------" << std::endl;

    //函数覆盖器
    using ArithmeticFuncMember = std::function<int(Arithmetic&, int, int)>;
    ArithmeticFuncMember add_member = &Arithmetic::add;
    add_member(a, 1, 2);
}


/**
 *  function type wrapper：其值可以为普通函数，函数对象，lamda
 */
void functionWrapper(){
    using ArithmeticFunc = std::function<int(int, int)>;

    //普通函数
    ArithmeticFunc add_normal = addNormal;
    std::cout << "add_normal: " << add_normal(1, 2) << std::endl;

    //函数对象
    ArithmeticFunc add_object = AddFuncObject("AddFuncObject");
    add_object(1, 2);

    //lamda
    ArithmeticFunc add_lamda = [](int a, int b){
        return a + b;
    };
    std::cout << "add_lamda: " << add_lamda(1, 2) << std::endl;

    //成员函数
    Arithmetic a("Arithmetic_member");
    //方式1
    ArithmeticFunc add_mem_bind = std::bind(&Arithmetic::add, a, std::placeholders::_1, std::placeholders::_2);
    //方式2
    ArithmeticFunc add_mem_lamda = [&](int first, int second)-> int{
        return a.add(first, second);
    };
    add_mem_bind(1, 2);
    add_mem_lamda(1, 2);

    //方式3
    using ArithmeticFuncMember = std::function<int(Arithmetic&, int, int)>;
    ArithmeticFuncMember add_member = &Arithmetic::add;
    add_member(a, 1, 2);
}



void map_func_call(string& key){
    using ArithmeticFunc = std::function<int(int, int)>;
    Arithmetic a("Arithmetic_a");

    //成员函数map
    std::map<string, ArithmeticFunc> arithmeticFuncMap;
    arithmeticFuncMap.insert(std::make_pair("addNormal", addNormal));
    arithmeticFuncMap.insert(std::make_pair("addFuncObject",AddFuncObject("AddFuncObject")));
    std::cout << "-------------" << std::endl;

    arithmeticFuncMap.insert(std::make_pair("lamda", [](int a, int b){
        return a + b;
    }));
    arithmeticFuncMap.insert(std::make_pair("member", [&](int first, int second){
        return a.add(first, second);
    }));

    a.setPrintStr("Arithmetic_aaa");

    auto obj = arithmeticFuncMap.find(key);
    if(obj != arithmeticFuncMap.end()){
        std::cout << obj->second(1, 2) << std::endl;
    }
}


void map_memberFunc_call(Arithmetic& arithmetic, string& key){
    using ArithmeticFuncMember = std::function<int(Arithmetic&, int, int)>;

    std::map<string, ArithmeticFuncMember> mapFunc;
    mapFunc.insert(std::make_pair("add", &Arithmetic::add));
    mapFunc.insert(std::make_pair("sub", &Arithmetic::sub));
    mapFunc.insert(std::make_pair("mul", &Arithmetic::mul));
    mapFunc.insert(std::make_pair("div", &Arithmetic::div));

    auto obj = mapFunc.find(key);
    if(obj != mapFunc.end()){
        obj->second(arithmetic, 1, 2);
    }
}


void funcAssign(){
    using ArithmeticFunc = std::function<int(int, int)>;

    ArithmeticFunc add = AddFuncObject("AddFuncObject");
}

int main(int argc, char* argv[]){
//    Arithmetic a("Arithmetic_a");
//    string key = "add";


//    map_memberFunc_call(a, key);

    callFunctionObject();

    return 0;
}



