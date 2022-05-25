#include <iostream>
#include <thread>
#include <chrono>
#include "sharePointer.h"

using namespace std;
int main(int argc, char* argv[]){

    test2();

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}























