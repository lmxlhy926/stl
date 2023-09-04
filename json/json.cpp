
#include <iostream>
#include <exception>
#include <string>

using namespace std;

class Exception : public std::exception{
public:
    Exception(string msg) : msg_(std::move(msg)){}
    ~Exception() noexcept override = default;
    char const* what() const noexcept override{
        return msg_.c_str();
    }
protected:
    string msg_;
};


class LogicError : public Exception{
public:
    LogicError(string const& msg) : Exception(msg) {}
};


// string const& msg : 可以传入右值
void throwLogicError(string const& msg){
    throw Exception(msg);
}

//enum struct: 限制更加严格
enum struct color{
    red,
    blue,
    green
};


void func(color c){
    if (c == color::red){
        std::cout << "red" << std::endl;
    }
}


int main(int argc, char* argv[]){
    color c = color::red;
    func(c);
    throwLogicError("msg");
    return 0;
}





