//
// Created by 78472 on 2022/7/3.
//

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "log/LogFile.h"
#include "log/TimeStamp.h"
#include "log/Logging.h"
#include "thread/ThreadPool.h"

using namespace muduo;

void timeStampTest(){
    TimeStamp timeStamp = TimeStamp::now();
    std::cout << "timeStamp: " << timeStamp.toFormattedString(true) << std::endl;
}

void appendFileTest(){
    AppendFile af(R"(D:\project\stl\mudo\base\test\output)");
    string outstr = "hello\n";
    for(int i = 0; i < 10000; i++){
        af.append(outstr.c_str(), outstr.size());
    }
}

void logFileTest(){
    LogFile lf(R"(D:\project\stl\mudo\base\test\output)", 1000* 30);
    string outstr = "hello\n";
    for(int i = 0; i < 10000; i++){
        lf.append(outstr.c_str(), outstr.size());
    }
}

void allTypeTest(){
    LogFile lf(R"(D:\project\stl\mudo\base\test\output)", 1000* 30);
    muduo::Logger::setOutput([&](const char* msg, size_t len, muduo::Logger::LogLevel level){
        lf.append(msg, len);
    });

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

    char a[10] = {1, 2, 3};
    LOG_HLIGHT << "&a[0]: " << reinterpret_cast<const void*>(&a[0]);
    LOG_HLIGHT << "&a[1]: " << reinterpret_cast<const void*>(&a[1]);
    LOG_HLIGHT << "&a[2]: " << reinterpret_cast<const void*>(&a[2]);
    LOG_HLIGHT << "&a[3]: " << reinterpret_cast<const void*>(&a[3]);
    LOG_HLIGHT << "&a[4]: " << reinterpret_cast<const void*>(&a[4]);

    string str = "welcome to the world";
    LOG_HLIGHT << str;
}

void setOutputTest(){

    LogFile lf(R"(D:\project\stl\mudo\base\test\output)", 1000* 30);
    muduo::Logger::setOutput([&](const char* msg, size_t len, muduo::Logger::LogLevel level){
        lf.append(msg, len);
    });

    LOG_INFO << "hello" << static_cast<char>('\0') << "hello";
    LOG_INFO << "hello";
}


int main(int argc, char* argv[]){
    allTypeTest();


//    while(true){
//        std::this_thread::sleep_for(std::chrono::seconds(10));
//    }
    return 0;
}













