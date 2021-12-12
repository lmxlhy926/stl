

#include <iostream>
#include <thread>
#include "file.h"


int main(int argc, char* argv[]){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
//    cfile file(path);
//    file.write("this is %s\n", "hello world");

    char str[100]{};
    if(cfile(path).getLine(str)){
        std::cout << "str: " << str << std::endl;
    }

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}















