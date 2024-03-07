
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
            std::cout << "_a: " << _a << std::endl;
            std::cout << "_b: " << _b << std::endl;
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


int main(int argc, char* argv[]){

// 创建
std::vector<int> vec({1, 2, 3});
vec.reserve(20);


//插入
vec.insert(vec.end(), 5);
vec.push_back(4);
vec.emplace(vec.end(), 6);
vec.emplace_back(7);


printVector("vec", vec);


//删除
vec.erase(vec.begin());
vec.pop_back();


//元素访问
std::cout << vec.front() << " " << vec.back() << " " << vec[0] << " " << vec.at(0) << std::endl;


//遍历
for(auto& elem : vec){
    std::cout << "elem: " << elem << std::endl;
}

for(auto pos = vec.begin(); pos != vec.end();){
    if(*pos % 2 == 0){
        pos = vec.erase(pos);
    }else{
        ++pos;
    }
}

printVector("vec", vec);


#if 0
//属性
std::cout << "size: " << vec.size() << std::endl;
std::cout << "maxSize: " << vec.max_size() << std::endl;
std::cout << "capacity: " << vec.capacity() << std::endl;
std::cout << "empty(): " << vec.empty() << std::endl;



vec.reserve(100);
std::cout << "capacity: " << vec.capacity() << std::endl;

vec.resize(20);
std::cout << "capacity: " << vec.capacity() << std::endl;


//赋值
std::vector<int> vec2(vec);
vec2.assign({1, 2, 3});
vec2.assign(vec.begin(), vec.end());
vec2.assign(2, 1);

#endif

    return 0;
}







