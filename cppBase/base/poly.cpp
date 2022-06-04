//
// Created by lhy on 2020/11/30.
//
#include "poly.h"

    using namespace fundamental;

/*
 * 纯虚函数也可以有自己的实现
 */
    void memoryDevice::open() {
        std::cout << "device open" << endl;
    }
    void memoryDevice::read() {
        std::cout << "device read" << endl;
    }
    void memoryDevice::write() {
        std::cout << "device write" << endl;
    }
    void memoryDevice::close() {
        std::cout << "device close" << endl;
    }

    void memoryDevice::printMessage(){
        std::cout << "---in memoryDevice printMessage---" << std::endl;
    }









