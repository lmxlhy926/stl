
#include <iostream>
#include <exception>
#include <system_error>
#include <future>
#include <unistd.h>

using namespace std;

void handler1(){
    throw logic_error("nihao logic error");
}

void handler2(){
    handler1();
}

void handler3(){
    handler2();
}

int main(int argc, char* argv[]){
    try{
        handler2();
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
    
    sleep(2);
    return 0;
}








