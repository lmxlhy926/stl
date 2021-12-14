

#include <iostream>
#include <thread>
#include "file.h"
#include <array>


/*
 * 读取指定字节数的字符到数组中，readCount为实际读取到的字节数
 * 如果文件没有内容，则设置文件结束标志，但是不会设置文件操作错误标志
 */
void readNumBytes(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path);   //自动以默认方式打开文件
    std::array<char, 100> carray{};

    size_t readCount = file.read(carray.data(), 100 - 1);
    std::cout << "readCount: " << readCount << std::endl;
    std::cout << "carray: " << carray.data() << std::endl;

    if(file.eof())  std::cout << "file.eof() is true" << std::endl;
    if(file.error())    std::cout << "file.error() is true" << std::endl;
}

void readChar(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path);
    char c;
    while(file.read(c)) {
        std::cout << "read the char: " << c << std::endl;
    }
}

void readLine(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path);
    std::array<char, 1024> lineArray{};

    while(file.getLine(lineArray.data())){
        std::cout << "line: " << lineArray.data();
    }
}

void write(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path, "w");

    string s = "hello\n";
    file.write('a');
    file.write(s);
    file.write("%s--%s\n", "ni", "hao");
    file.write("good morning\n");
}


int main(int argc, char* argv[]){

    write();


    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}















