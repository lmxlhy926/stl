
#include <iostream>
#include <thread>
#include <chrono>
#include "poly.h"

using namespace std;
using namespace fundamental;
using namespace PolyClassTest;

int main(int agrc, char* argv[]){

    destructor();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}


























