
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <thread>

using namespace std;

void add(int a){
    std::cout << "normal add : " << a << std::endl;
}

void sub(int a){
    std::cout << "normal sub : " << a << std::endl;
}

class student{
    private:
        int _a;
        float _b;
    public:
        // 空参构造
        student(){
            std::cout << "empty constructor student ..." << std::endl;
        }

        // 构造函数
        student(int a, float b) : _a(a), _b(b) {
            std::cout << "_a: " << _a << ", _b: " << b << std::endl;
            std::cout << "constructor ..." << std::endl;
        }

        // 拷贝构造函数
        student(const student& stu){
            std::cout << "copy constructor student ..." << std::endl;
        }

        // move构造函数
        student(student&& stu){
            std::cout << "move constructor student ..." << std::endl;
        }

        // 析构函数
        ~student(){
            std::cout << "destructor student..." << std::endl;
        }

        // 拷贝赋值函数
        student& operator=(const student &stu){
            std::cout << "copy operator= student ..." << std::endl;
            return *this;
        }

        // move赋值函数
        student& operator=(student&& stu){
            std::cout << "move operator= student..." << std::endl;
            return *this;
        }

        // operator+
        student& operator+(const student &stu){
            std::cout << "operaotr+ student ..." << std::endl;
            return *this;
        }

        // operator()
        void operator()(){
            std::cout << "opeator()() ... " << std::endl;
        }

        void operator()(int a){
            std::cout << "operator(int a) : " << a << std::endl;
        }

        // add
        void add(int a){
            this->_a = 100;
            std::cout << "add ..." << a << std::endl;
        }
};


class person{
private:
    student _stu1;
    student _stu2;
public:
    person(const student& stu): _stu1(stu){     //进入函数之前先拷贝构造，后空参构造
        _stu2 = stu;    //赋值
        std::cout << "person single param constructor ..." << std::endl;
    }

    ~person(){
        std::cout << "destructor person ..." << std::endl;
    }
};



void student_test(){
    student stu;                    //空参构造
    student stu2(stu);              //拷贝构造
    student stu3(std::move(stu));   //move构造
    stu = stu3;                    //拷贝赋值
    stu = std::move(stu3);         //move赋值
    std::cout << "----------" << std::endl;
    person p(stu);
}


void printVector(const string&& hint, const std::vector<int>& vec){
    std::cout << "-------" << hint << "------------" <<  std::endl;
    for(auto& elem : vec){
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void printVectorProperty(const string& hint, const std::vector<int>& vec){
    std::cout << "----------" << hint << "----------" << std::endl;
    std::cout << "capacity: " << vec.capacity() << std::endl;
    std::cout << "size: " << vec.size() << std::endl;
    std::cout << "empty: " << vec.empty() << std::endl;
}


void vector_practice(){
    //创建
    std::vector<student> vec;
    vec.reserve(100);


    //插入
    student stu(1, 2);
    vec.insert(vec.end(), student(3, 4));   //构造、move构造、析构
    vec.insert(vec.end(), stu);  //拷贝构造
    vec.emplace(vec.end(), 4, 5);   //构造


    //删除
    vec.erase(vec.end());   //析构
    std::cout << "-----------" << std::endl;


    std::vector<student> vec2 = vec;    //拷贝构造
    std::cout << "-----------" << std::endl;

    vec2.assign(vec.begin(), vec.end());
}


/**
 * 普通函数：   指针
 * 成员函数：   指针
 * 
 * 函数对象: 对象拷贝
 * lamba: 对象拷贝
 * 
 * 
 * 
*/


int main(int argc, char* argv[]){
/**
 * 普通函数
 * 成员函数
 * 函数对象
 * lamba
*/
    student stu;

    // void(*singleFunc)(int);     //定义一个函数指针变量，指向普通函数名
   

    // void(student::*singleMemFunc)(int); //成员函数指针，指向成员函数
    // singleMemFunc = &student::add;
    // (stu.*singleMemFunc)(300);

    typedef void(*singleFuncType)(int);     //定义函数指针类型
    singleFuncType singleFunc;  //定义函数指针变量
    singleFunc = add;   //直接赋值函数名
    singleFunc(100);
    singleFunc = &sub;  //&函数名，c中2种方式都正确
    singleFunc(200);

    using singleFuncTypeU = void(*)(int);
    singleFuncTypeU singleFuncU = add;
    singleFuncU(300);
    
   



    return 0;
}







