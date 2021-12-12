

#include <time.h>
#include "lhytime.h"
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char *argv[]){

    tmTest();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}





















