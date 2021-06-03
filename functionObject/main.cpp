#include <iostream>
#include <functional>
#include <map>


//普通函数
int addFunction(int a, int b){
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
class addClass{
private:
    int value = 100;
public:
    int add(int a, int b){
        return a + b + value;
    }

    int mul(int a, int b){
        return a * b;
    }

    int div(int a, int b){
        return a / b;
    }

    static int sub(int a, int b){
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

    using memfp = int(addClass::*)(int, int);   //声明成员函数指针变量类型
    std::map<string, memfp> fmap;
    fmap.insert(pair<string, memfp>("add", &addClass::add));
    fmap.insert(pair<string, memfp>("mul", &addClass::mul));
    fmap.insert(pair<string, memfp>("div", &addClass::div));

    addClass a;
    string option = "div";

    auto f = fmap.find(option);
    if(f != fmap.end()){
        std::cout << "--: " << (a.*f->second)(1, 2) << std::endl;
    }


    return 0;
}

//普通函数指针
void normal_func_pointer(){

    int(*fp)(int, int);     //定义一个函数指针变量
    fp = addFunction;       //给函数指针变量赋值
    std::cout << "fp(1, 2): " << fp(1, 2) << std::endl;

    typedef int(*p2func)(int, int);     //定义一个函数指针类型
    p2func fp1 = addFunction;           //创建一个函数指针变量，并为其赋值
    std::cout << "fp1(1, 2): " << fp1(1, 2) << std::endl;

    using p2funciii = int(*)(int, int);     //定义一个函数指针类型
    p2funciii fp2 = addFunction;            //创建一个函数指针变量，并为其赋值
    std::cout << "fp2(1, 2): " << fp2(1, 2) << std::endl;
}

void normal_functionObject_cpp(){

    std::function<int(int,int)> f;

    f = addFunction;    //普通函数
    std::cout << "addFunction: " << f(1, 2) << std::endl;

    f = addFunctionObject();    //创建一个函数对象实体
    std::cout << "addFunctionObject: " << f(1, 2) << std::endl;

    f = [](int a, int b) ->int {return a + b;};
    std::cout << "lambda: " << f(1, 2) << std::endl;
}

void member_funciton_cpp(){

    //静态成员函数本质上就是普通函数
    using staticMemberFunction = int(*)(int, int);
    staticMemberFunction smf = &addClass::sub;
    std::cout << "smf(1, 2): " << smf(1, 2) << std::endl;

    //成员函数指针
    int (addClass::*mfp)(int, int);     //定义一个成员函数指针变量
    mfp = &addClass::add;               //给成员函数指针变量赋值，对于成员函数c++标准强制要求加上'&'

    using mfpiii_fpType = int(addClass::*)(int, int, int);     //定义成员函数指针类型
    mfpiii_fpType mfp1 = &addClass::addWithNum;

    addClass a;
    int result = (a.*mfp)(1, 2);        //通过对象，成员函数指针来调用成员函数
    std::cout << "(a.*mfp)(1, 2): " << result << std::endl;

    std::function<int(int, int)> f;
    f = std::bind(mfp, a, std::placeholders::_1, std::placeholders::_2);    //将成员函数和对象绑定到一起
    std::cout << "addClass::addWithNum(int, int): " << f(1, 2) << std::endl;

    f = std::bind(mfp1, a, std::placeholders::_1, std::placeholders::_2, 100);
    std::cout << "addClass::addWithNum(int, int, int): " << f(1, 2) << std::endl;
}





