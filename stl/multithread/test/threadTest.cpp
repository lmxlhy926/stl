//
// Created by 78472 on 2022/7/24.
//

#include <iostream>
#include "mthread.h"
#include "mmutex.h"

int main(int argc, char* argv[]){

    mthread::mutex_lockunlock();

    std::cout << "----main end-----" << std::endl;
    return 0;
}