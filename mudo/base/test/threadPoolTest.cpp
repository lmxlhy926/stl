
#include <iostream>
#include "base/ThreadPool.h"
#include <chrono>
#include <thread>

void print(){
    std::cout << "hello world" << std::endl;
}

void printStr(string str){
    while(true){
        std::cout << str << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}



int main(int argc, char* argv[]){
    muduo::ThreadPool pool("threadPool");
    pool.start(3);

    pool.run(print);
    pool.run([](){
        printStr("hello");
    });

    std::cout << "-------end--------" << std::endl;
    return 0;
}





























