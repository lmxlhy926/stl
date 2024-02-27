
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <thread>

using namespace std;

class student{
    public:
        student(){
            std::cout << "empty constructor student ..." << std::endl;
        }

        //拷贝构造函数
        student(const student& stu){
            std::cout << "copy student ..." << std::endl;
        }

        //move构造函数
        student(student&& stu){
            std::cout << "move student ..." << std::endl;
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


void student_test(){
    std::vector<student> vec;
    student stu;

    // stu + stu;

    vec.push_back(stu);
    vec.emplace_back(stu);



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

    student_test();



    return 0;
}






