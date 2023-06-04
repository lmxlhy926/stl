#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;


/*
 * callable object
        函数：
 *          1. 普通函数：接受额外传入的args作为实参
 *          2. 成员函数：当通过对象调用它, 该对象被传递为第一实参(必须是个reference或pointer),  其它实参则一一对应成员函数的参数
 *      类对象：
 *          3. 函数对象: 附带的args被传递作为实参
 *          4. lamda：  严格的说它是一种函数对象
 *
 * std::function<>: 
 *      容纳上述4中形式；
 *      赋值为类对象会调用类对象的拷贝构造函数
 *      
 */


//普通函数
int addNormal(int a, int b){
    return a + b;
}

//函数对象
class funcObject{
private:
    string message;
public:
    explicit funcObject(string str): message(std::move(str)){
        std::cout << "===>funcObject constructor--" << message << std::endl;
    }

    funcObject(const funcObject& af){
        this->message = af.message;
        std::cout << "===>funcObject copyConstructor" << std::endl;
    }

    ~funcObject(){ 
        std::cout << "===>~funcObject:" << message << std::endl; 
    }

    int operator()(int a, int b){
        std::cout << "operator()(int a, int b): " << message << std::endl;
        return a + b;
    }

    int operator()(int a, int b, int c){
        std::cout << "operator()(int a, int b, int c): " << message << std::endl;
        return a + b + c;
    }

    int showMessage(string& info){
        std::cout << "interal: " << message << ", info: " << info << std::endl;
        return 0;
    }

     int add(int a, int b){
        std::cout << "add(int a, int b): " << message << std::endl;
        return a + b;
    }

    int sub(int a, int b){
        std::cout << "sub(int a, int b): " << message << std::endl;
        return a - b;
    }

    int mul(int a, int b) const{
        std::cout << "mul(int a, int b): " << message << std::endl;
        return a * b;
    }

    int div(int a, int b) const{
        std::cout << "div(int a, int b): " << message << std::endl;
        return a / b;
    }

    //静态函数
    static void staticCall(int, int){
        std::cout << "---staticCall---" << std::endl;
    }
};


/**
 * 函数名是函数所在代码段的地址，即函数指针变量的值。
 * 4种声明函数指针变量的方式：本质上是2种：函数指针；函数覆盖器
 *      定义函数指针变量
 *          1. int(*nfp)(int, int);
 *      定义函数指针类型
 *          2. typedef int(*addFunc)(int, int);
 *          3. using addFunc2 = int(*)(int, int);
 *      函数覆盖器
 *          4. using ArithmeticFunc = std::function<int(int, int)>
 */
void normalFuncCall(){
    int(*nfp)(int, int);          //定义函数指针变量
    nfp = addNormal;              //函数指针变量赋值；函数名即是指针变量的值
    std::cout << "nfp(1, 2): " << nfp(1, 2) << std::endl;

    typedef int(*funcType1)(int, int);    //定义函数指针类型
    funcType1 nfp1 = addNormal;           //声明并赋值
    std::cout << "nfp1(1, 2): " << nfp1(1, 2) << std::endl;

    using funcType2 = int(*)(int, int);     //定义函数指针类型
    funcType2 nfp2 = addNormal;             //声明并赋值
    std::cout << "nfp2(1, 2): " << nfp2(1, 2) << std::endl;

    using ArithmeticFunc = std::function<int(int, int)> ;
    std::function<int(int, int)> add_normal{addNormal};
    std::cout << "add_normal(1, 2): " << add_normal(1, 2) << std::endl;
}

void staticFuncCall(){
    using staticMemberFunction = void(*)(int, int);
    staticMemberFunction smf = funcObject::staticCall;    //静态成员函数: 携带类作用域的普通函数
    smf(1, 2);
}


void memberFuncCall(){
    funcObject fc("function object");

    //成员函数
    int (funcObject::*mfp)(int, int);   //指向类成员函数的指针，定义时携带类作用域
    mfp = &funcObject::add;             //给成员函数指针变量赋值，对于成员函数c++标准强制要求加上'&'
    (fc.*mfp)(1, 2);                    //通过对象调用方法

    using memberFuncType = int(funcObject::*)(int, int);   //指向类成员函数的指针，定义时携带类作用域
    memberFuncType add = &funcObject::add;                 //给成员函数指针变量赋值，对于成员函数c++标准强制要求加上'&'
    (fc.*add)(1, 2);

    using memberFuncConstType = int(funcObject::*)(int, int) const; //const成员函数类型的定义
    memberFuncConstType mul = &funcObject::mul;
    (fc.*mul)(1, 2);


    //函数覆盖器方式1
    std::cout << "-------1-------" << std::endl;
    std::function<int(funcObject, int, int)> addMemberFunc1 = &funcObject::add;
    addMemberFunc1(fc, 1, 2);   //传递参数时调用拷贝构造函数

    std::cout << "-------2-------" << std::endl;
    std::function<int(funcObject&, int, int)> addMemberFunc2 = &funcObject::add; 
    addMemberFunc2(fc, 1, 2);   //传递参数时使用引用
    
    //函数覆盖器方式2
    std::cout << "-------3-------" << std::endl;
    std::function<int(int, int)> add_mem_bind2 = std::bind(&funcObject::add, fc, std::placeholders::_1, std::placeholders::_2);
    add_mem_bind2(1, 2);

    std::cout << "-------4-------" << std::endl;
    std::function<int(int, int)> add_mem_bind1 = std::bind(&funcObject::add, &fc, std::placeholders::_1, std::placeholders::_2);
    add_mem_bind1(1, 2);

    std::cout << "-------5-------" << std::endl;
    //函数覆盖器方式3
    std::function<int(int, int)> add_mem_lamda = [&](int first, int second) -> int{
        return fc.add(first, second);
    };
    add_mem_lamda(1, 2);
}



/**
 * 通过函数对象来完成函数调用:
 *     1. 创建对象：
 *     2. 调用对象的函数(重载函数调用符'()')
 */
void functionObjectCall(){
    funcObject fo("AddFuncObject-afo");
    fo(1, 2);
    fo(1, 2, 3);
    fo.operator()(1, 2);
    fo.operator()(1, 2, 3);
    std::cout << "--------------" << std::endl;

    //创建临时对象来调用函数, 临时对象使用完会被立即析构
    funcObject("AddFuncObject1")(1, 2);
    funcObject("AddFuncObject2")(1, 2, 3);
    funcObject("AddFuncObject3").operator()(1, 2);
    funcObject("AddFuncObject4").operator()(1, 2, 3);
    std::cout << "--------------" << std::endl;

    //函数覆盖器
    using ArithmeticFunc = std::function<int(int, int)> ;
    std::function<int(int, int)> add_object = funcObject("AddFuncObject5");
    add_object(1, 2);
}


void lamdaCall(){
    funcObject fo("funcObject");
    string s;
    auto lamda = [s, fo](int, int)->int{
        std::cout << "in lamda call" << std::endl;
        fo.div(1, 2);
        std::cout << s << std::endl;
        return 0;
    };
    lamda(1, 2);

    std::function<int(int, int)> lamdaFunction = lamda;
    lamdaFunction(1, 2);
}


void map_func_call(){
    using ArithmeticFunc = std::function<int(int, int)>;
    funcObject a("Arithmetic_a");

    //成员函数map
    std::map<string, ArithmeticFunc> arithmeticFuncMap;
    arithmeticFuncMap.insert(std::make_pair("addNormal", addNormal));
    arithmeticFuncMap.insert(std::make_pair("addFuncObject",funcObject("funcObject")));
    arithmeticFuncMap.insert(std::make_pair("lamda", [](int a, int b){
        return a + b;
    }));
    arithmeticFuncMap.insert(std::make_pair("member", [&](int first, int second){
        return a.add(first, second);
    }));


    std::cout << "-------------" << std::endl;

    std::vector<string> keyVec{"addNormal", "addFuncObject", "lamda", "member"};
    for(auto& elem : keyVec){
        auto obj = arithmeticFuncMap.find(elem);
        if(obj != arithmeticFuncMap.end()){
            std::cout << obj->second(1, 2) << std::endl;
        }
    }
}


void map_memberFunc_call(funcObject& object, string& key){
    using ArithmeticFuncMember = std::function<int(funcObject&, int, int)>;
    std::map<string, ArithmeticFuncMember> mapFunc;
    mapFunc.insert(std::make_pair("add", &funcObject::add));
    mapFunc.insert(std::make_pair("sub", &funcObject::sub));
    mapFunc.insert(std::make_pair("mul", &funcObject::mul));
    mapFunc.insert(std::make_pair("div", &funcObject::div));

    auto obj = mapFunc.find(key);
    if(obj != mapFunc.end()){
        obj->second(object, 1, 2);
    }
}


int main(int argc, char* argv[]){

    
    lamdaCall();


    return 0;
}



