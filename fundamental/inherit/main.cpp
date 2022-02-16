
#include <iostream>
#include <thread>
#include <chrono>
#include "inherit.h"

using namespace std;

int main(int argc, char* argv[]){

    string s = "hello";
    Inherit i(s);
    i.printValue();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}





















