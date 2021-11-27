#include <iostream>
#include <functional>
#include <map>
#include <vector>

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
    int add(int a, int b){
        return a + b + value;
    }

    int mul(int a, int b){
        return a * b + value;
    }

    int div(int a, int b){
        return a / b + value;
    }

    static int sub(int a, int b){   //静态成员函数 == 普通函数
        return a - b;
    }

    int addWithNum(int a, int b){
        return a + b + value;
    }

    int addWithNum(int a, int b, int c){
        return a + b + c + value;
    }
};


using namespace std;
int main(int argc, char* argv[]){


    return 0;
}

//普通函数指针的定义和引用
void normal_func_pointer(){

    int(*fp)(int, int);            //定义一个函数指针变量
    fp = normalAddFunction;       //给函数指针变量赋值
    std::cout << "fp(1, 2): " << fp(1, 2) << std::endl;

    typedef int(*p2func)(int, int);     //定义一个函数指针类型
    p2func fp1 = normalAddFunction;     //创建一个函数指针变量，并为其赋值
    std::cout << "fp1(1, 2): " << fp1(1, 2) << std::endl;

    using p2funciii = int(*)(int, int);     //定义一个函数指针类型
    p2funciii fp2 = normalAddFunction;      //创建一个函数指针变量，并为其赋值
    std::cout << "fp2(1, 2): " << fp2(1, 2) << std::endl;
}


//std::function和函数对象
void normal_functionObject_cpp(){

    std::function<int(int,int)> f;
    std::function<int(int, int, int)> f1;

    f = normalAddFunction;    //普通函数
    std::cout << "addFunction: " << f(1, 2) << std::endl;

    f = addFunctionObject();    //创建一个函数对象实体
    std::cout << "addFunctionObject(int, int): " << f(1, 2) << std::endl;
    std::cout << "addFunctionObject(int, int): " << addFunctionObject()(1, 2) << std::endl;


    f1 = addFunctionObject();   //创建一个函数对象实体
    std::cout << "addFunctionObject(int, int, int): " << f1(1, 2, 3) << std::endl;
    std::cout << "addFunctionObject(int, int, int): " << addFunctionObject()(1, 2, 3) << std::endl;


    f = [](int a, int b) ->int {return a + b;};     //lamda
    std::cout << "lambda: " << f(1, 2) << std::endl;
}


//成员函数指针
void member_funciton_cpp(){

    //静态成员函数本质上就是普通函数
    using staticMemberFunction = int(*)(int, int);
    staticMemberFunction smf = &memberAddClass::sub;    //静态成员函数
    std::cout << "smf(1, 2): " << smf(1, 2) << std::endl;

    //成员函数指针
    int (memberAddClass::*mfp)(int, int);      //定义一个成员函数指针变量
    mfp = &memberAddClass::add;               //给成员函数指针变量赋值，对于成员函数c++标准强制要求加上'&'

    using mfpiii_fpType = int(memberAddClass::*)(int, int, int);   //定义成员函数指针类型
    mfpiii_fpType mfp1 = &memberAddClass::addWithNum;

    memberAddClass a;
    int result = (a.*mfp)(1, 2);        //通过对象，成员函数指针来调用成员函数
    std::cout << "(a.*mfp)(1, 2): " << result << std::endl;

    std::function<int(int, int)> f;
    f = std::bind(mfp, a, std::placeholders::_1, std::placeholders::_2);    //将成员函数和对象绑定到一起
    std::cout << "addClass::addWithNum(int, int): " << f(1, 2) << std::endl;

    f = std::bind(mfp1, a, std::placeholders::_1, std::placeholders::_2, 100);
    std::cout << "addClass::addWithNum(int, int, int): " << f(1, 2) << std::endl;
}


//对象、成员函数指针抽象调用成员方法
void abstract_object_memberFunction(){
    using memfp = int(memberAddClass::*)(int, int);   //声明成员函数指针变量类型

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

    //通过对象、成员函数指针来调用对象的成员方法
    auto obj = memMap.find(objectOption);
    if(obj != memMap.end()){
        auto f = fmap.find(methodOption);
        if(f != fmap.end()){
            std::cout << "===>: " << ((obj->second).*(f->second))(input1, input2) << std::endl;
        }
    }
}



