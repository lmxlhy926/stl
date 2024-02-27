
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <thread>

using namespace std;

class student{
    public:
        //空参构造
        student(){
            std::cout << "empty constructor student ..." << std::endl;
        }

        //拷贝构造函数
        student(const student& stu){
            std::cout << "copy constructor student ..." << std::endl;
        }

        //move构造函数
        student(student&& stu){
            std::cout << "move constructor student ..." << std::endl;
        }

        ~student(){
            std::cout << "destructor student..." << std::endl;
        }

        //拷贝赋值函数
        student& operator=(const student &stu){
            std::cout << "copy operator= student ..." << std::endl;
            return *this;
        }

        //move赋值函数
        student& operator=(student&& stu){
            std::cout << "move operator= student..." << std::endl;
            return *this;
        }

        student& operator+(const student &stu){
            std::cout << "operaotr+ student ..." << std::endl;
            return *this;
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


class functional{
    public:
        void operator()(){
            std::cout << "in functional : " << std::endl; 
        }
};

void normalFunc(const string &str){
    std::cout << "in normalFunc : " << str << std::endl;
}

std::vector<std::thread> threadVec;


int main(int argc, char* argv[]){

   std::vector<int> vec;

//创建


//访问


//插入


//删除、清空


//遍历


//属性： 大小



    return 0;
}






