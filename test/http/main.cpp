
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <thread>

using namespace std;

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

        // 成员函数
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

void add(int a){
    std::cout << "normal add : " << a << std::endl;
}

void sub(int a){
    std::cout << "normal sub : " << a << std::endl;
}


void student_test(){
    student stu;                    //空参构造
    student stu2(stu);              //拷贝构造
    student stu3(std::move(stu));   //move构造
    stu = stu3;                     //拷贝赋值
    stu = std::move(stu3);          //move赋值
    std::cout << "----------" << std::endl;
    person p(stu);
}



int main(int argc, char* argv[]){
/*
    普通函数
    成员函数

    函数对象
    lamba
*/
    student stu;

    std::function<void(int)> singleFunc;
    singleFunc = &add;
    singleFunc(100);

    std::function<void(student&, int)> singleMemFunc;
    singleMemFunc = &student::add;
    singleMemFunc(stu, 200);

    singleFunc = [stu](int a)mutable{
        stu.add(a);
    };
    singleFunc(300);
    




    return 0;
}







