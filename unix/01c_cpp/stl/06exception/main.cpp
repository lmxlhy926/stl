
#include <iostream>
#include <exception>
#include <functional>
#include <system_error>
#include <thread>
#include <new>
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

void test(){
    try{
        handler2();
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}


int main(int argc, char* argv[]){

    // std::thread t([](){
    //     int a;
    //     int b = a / 0;
    // });

     int a;
        int b = a / 0;

    sleep(2);

     new(int[1024 * 1024 * 1024]);

    for(int i = 0; i < 1024 * 1024 * 1024; ++i){
         new(int[1024 * 1024 * 1024]);
    }
    
   
    std::function<int(int, int)> f;
    f(1, 2);
    sleep(2);    

    return 0;
}








