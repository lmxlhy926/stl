
#include <iostream>
#include <thread>
#include "cfile.h"
#include <array>
#include <sstream>


/*
 * 读取指定字节数的字符到数组中，readCount为实际读取到的字节数
 * 如果文件没有内容，则设置文件结束标志，但是不会设置文件操作错误标志
 */
void readNumBytes(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path);   //自动以默认方式打开文件
    std::array<char, 100> carray{};

    size_t readCount = file.readBytes(carray.data(), 100 - 1);
    std::cout << "readCount: " << readCount << std::endl;
    std::cout << "carray: " << carray.data() << std::endl;

    if(file.eof())  std::cout << "file.eof() is true" << std::endl;
    if(file.error())    std::cout << "file.error() is true" << std::endl;
}

void readChar(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path);
    char c;
    while(file.readChar(c)) {
        std::cout << "read the char: " << c << std::endl;
    }
}

void readLine(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path);
    std::array<char, 1024> lineArray{};

    while(file.readLine(lineArray.data(), 1024)){
        std::cout << "line: " << lineArray.data();
    }
}

void write(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path, "w");

    string s = "hello\n";
    file.writeChar('a');
    file.write(s);
    file.write("%s--%s\n", "ni", "hao");
    file.write("good morning\n");
}


/*
 * r+：文件必须存在，读写，从文件开头
 * 从开头写入字符串，回到文件开头，读取字符串直到遇到EOF，在文件尾端写入
 */
void file1(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path, "r+");

    file.write("hello world");
    file.setPos(0, cfile::start);
    char c;
    printf("string: ");
    while(file.readChar(c)){
        putchar(c);
    }
    putchar('\n');
    file.write(" ok");  //如果读取遇到eof，则不需要调用定位函数
}


/*
 * w+：文件可以不存在，先清空文件再读写
 * 清空文件，写入字符串，定位，读取但是读取到EOF，写入字符，回到开头，写入字符串
 */
void file2(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path, "w+");

    file.write("file operate w+");
    file.getPos();  //读写之间需要定位
    char c;
    if(!file.readChar(c))   puts("end of file");
    file.write(" hhh");     //读取到EOF，之后可以直接写入，不需要定位
    file.setPos(0, cfile::start);
    file.write("xxxx");
}


/*
 * a+: 文件可以不存在，在尾端更新
 * 首先在尾端写入，然后位置回到开头，从头到尾读取字符直到遇到EOF
 */
void file3(){
    string path = R"(D:\project\stl\mudo\file\log.txt)";
    cfile file(path, "a+");

    file.write(" goodgood");
    file.setPos(0, cfile::start);
    char c;
    while(file.readChar(c)){
        putchar(c);
    }
    putchar('\n');
}

int main(int argc, char* argv[]){

    try{
        file1();
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }


    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}






























