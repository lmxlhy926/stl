#include <iostream>
#include <chrono>
#include <thread>
#include "basic_class.h"

using namespace std;
using namespace fundamental;
using namespace basicTest;

int main(int argc, char *argv[]){

    basic b = copyConstructInReturn();


    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}

























