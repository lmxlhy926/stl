#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>


int main(int argc, char* argv[]){

    assert(false);

    std::cout << "hello" << std::endl;

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}





















