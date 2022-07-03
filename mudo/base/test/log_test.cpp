
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <cstdio>
#include <algorithm>
#include "log/Logging.h"
#include "log/LogFile.h"
#include "log/TimeStamp.h"
#include "thread/ThreadPool.h"

using namespace muduo;
//using namespace httplib;

void timeStampTest(){
    TimeStamp timeStamp = TimeStamp::now();
    std::cout << "timeStamp: " << timeStamp.toFormattedString(true) << std::endl;
}


void allTypeTest(){
    LOG_INFO << "true: "<< true;
    LOG_INFO << "false: " << false;

    LOG_INFO << "short: " << static_cast<short>(100);
    LOG_INFO << "unsigned short: " << static_cast<unsigned short>(100);
    LOG_INFO << "int: " << static_cast<int>(100);
    LOG_INFO << "unsigned int: " << static_cast<unsigned int>(100);
    LOG_INFO << "long: " << static_cast<long>(100);
    LOG_INFO << "unsigned long: " << static_cast<unsigned long>(100);
    LOG_INFO << "long long: " << static_cast<long long>(100);
    LOG_INFO << "unsigned long long: " << static_cast<unsigned long long>(100);

    LOG_HLIGHT << "float: " << static_cast<float>(2.53);
    LOG_HLIGHT << "double: " << static_cast<double>(2.53);

    int a[10] = {1, 2, 3};
    LOG_HLIGHT << "&a[0]: " << reinterpret_cast<const void*>(&a[0]);
    LOG_HLIGHT << "&a[1]: " << reinterpret_cast<const void*>(&a[1]);

    string str = "welcome to the world";
    LOG_HLIGHT << str;
}

void setOutputTest(){
    ThreadPool threadPool;
    threadPool.start(10);

    LogFile lf(R"(D:\project\stl\mudo\base\test\output)", 1000* 30);

    muduo::Logger::setOutput([&](const char* msg, size_t len, muduo::Logger::LogLevel level){
        lf.append(msg, len);
    });

    threadPool.run([](){
        for(int i = 0; i < 1000; i++){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            LOG_INFO << 123123456789;
        }
    });

    threadPool.run([](){
        for(int i = 0; i < 1000; i++){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            LOG_INFO << 123123456789;
        }
    });
    threadPool.run([](){
        for(int i = 0; i < 1000; i++){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            LOG_HLIGHT << 123123456789;
        }
    });
}


void reverseTest(){
    char buffer[] = "hello";
    char* start = buffer;
    char* end = buffer + strlen(buffer);

    std::reverse(start, end);   //不操作最后一个位置，！= end();
    printf("===>%s\n", buffer);
}

int main(int argc, char* argv[]){
    setOutputTest();
    std::cout << "-----------shutdown----------" << std::endl;

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}


















