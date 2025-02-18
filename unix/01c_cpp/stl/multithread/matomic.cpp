//
// Created by lhy on 2021/1/3.
//

#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

namespace mthread{

    long data;
    std::atomic<bool> readyFlag(false); //保证对readyFlag的读写操作是不可分割的, 不会因为并发访问带来不明确的行为.

    void provider(){
        std::cout <<  "<return>" << std::endl;
        std::cin.get();

        data = 42;
        readyFlag.store(true);
    }

    void consumer(){
        while(!readyFlag.load()){
            std::cout.put('.').flush();
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        std::cout << "\nvalue: " << data << std::endl;
    }

    void atomictest(){

        auto p = std::async(std::launch::async, provider);
        auto c = std::async(std::launch::async, consumer);
    }


}
