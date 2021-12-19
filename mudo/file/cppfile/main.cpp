
#include <iostream>
#include <thread>
#include <array>
#include <sstream>
using namespace std;


/*
 * 读取一个字符
 */
void cplusfiletest(){
    stringstream str("hello, world");
    char c;
    while(str.get(c)){  //读取一个字符，遇到end-of-file设置eofbit和failbit
        putchar(c);
    }
    putchar('\n');
    if(str.eof() && str.fail()){
        std::cout << "end-of-file" << std::endl;
    }
}


/*
 * 读取指定字符之前的数据，不读取指定字符，且最多读取count -1 个字符，自动添加'\0'
 * 如果读到数据但是遇到文件结束，只设置eofbit
 * 如果没有读取到任何数据，则只设置failbit
 * 如果没有读取到任何数据且遇到文件结尾，则同时设置eofbit和failbit。
 */
void cplusfiletest1(){
    stringstream str("123");
    char array[100]{};

    /*
     * 最多读取count - 1个字符，自动添加'\0'
     * 读取停止条件：下一个字符为指定的字符，或者已经读取到count -1个字符。不会读取指定的字符。
     * 如果一个字符都没有读取到，则会设置failbit。并且buffer的第一个字符被设置为'\0'。
     */

    //第一次读取，读取到数据且到文件末尾，所以只设置eofbit.
    //清空错误后进行第二次读取，读取到文件末尾且没有读取到任何数据，所以同时设置eofbit和failbit
    while(str.get(array, 100, '\n')){
        std::cout << "array: " << array << ", count: " << str.gcount() << std::endl;
        if(str.eof()){
            std::cout << "array: end-of-file" << std::endl;
            str.clear();
        }
    }
    if(str.fail() && str.eof())
        std::cout << "failed: " << "array: " << array << ", count: " << str.gcount() << std::endl;
    std::cout << std::endl;


    stringstream str1("123\nhello");
    char array1[100]{};
    while(str1.get(array1, 100, '\n')){
        std::cout << "array1: " << array << ", count: " << str1.gcount() << std::endl;
        std::cout << "the next char is: " << str1.peek() << std::endl;
    }
    if(str1.fail()){    //没有遇到end-of-file，但是读取失败没有读到任何字符，只设置failbit
        std::cout << "failed: " << "array1: " << array << ", count: " << str1.gcount() << std::endl;
    }
}


/*
 * 读取一行数据，读取但是不存储newline。最多读取count - 1个字符，自动添加'\0'。
 * 读取到数据但是遇到文件末尾，只设置eofbit。
 * 没有读取到数据，且遇到文件末尾，同时设置eofbit和failbit
 */
void cplusfiletest2(){
    stringstream  str("123\nhello\nss");
    char array[100]{};

    while(str.getline(array, 100, '\n')){   //第三次读取，读取到数据且遇到文件末尾，只设置eofbit
        std::cout << "array: " << array << ", count: " << str.gcount() << std::endl;
        if(str.eof()){
            std::cout << "end of file" << std::endl;
            str.clear();
        }
    }
    if(str.fail() && str.eof()) //最后一次读取，没有读取到任何数据且遇到文件末尾，同时设置eofbit,failbit
        std::cout << "failed: " << "array: " << array << ", count: " << str.gcount() << std::endl;
}


/*
 * 读取指定数量的字符
 */
void cplusfiletest3(){
    stringstream str("helloworld");
    char array[100]{};

    /*
     * 读取count个字符，不自动添加'\0'
     * 期望读取count个字符，如果是因为遇到end-of-file而没有读取到这么多字符，会同时设置eofbit和failbit
     */
    while(str.read(array, 4)){
        std::cout << "array: " << array << ", count: " << str.gcount() << std::endl;
    }
    if(str.fail() && str.eof())
        std::cout << "failed: " << "array: " << array << ", count: " << str.gcount() << std::endl;
    std::cout << std::endl;


    stringstream str1("hellohello");
    char array1[100]{};
    long readCount{};

    /*
     * 期望读取count个字符，不自动添加'\0'
     * 返回实际读取的字符数，遇到end-of-file并不算错，不会设置eofbit和failbit。
     */
    while((readCount = str1.readsome(array1, 4)) != 0){
        std::cout << "readCount: " << readCount << std::endl;
    }
    if(str1.good()){
        std::cout << "readCount: " << readCount << std::endl;
    }
}


/*
 * 读取字符并忽略
 * 遇到文件末尾，只设置eofbit. 已达到文件末尾还继续读取，则会设置failbit。
 */
void cplusfiletest4(){
    stringstream str("hl");
    str.ignore(100);
    if(str.eof() && !str.fail()){   //提取时遇到文件末尾，设置eofbit
        std::cout << "end-of-file==>" << "readCount: " << str.gcount() << std::endl;
        str.clear();
    }

    str.ignore();
    if(str.eof() && !str.fail()) {  //清除eofbit后，再次读取，再次只设置eofbit
        std::cout << "end-of-file again==>" << "readCount: " << str.gcount() << std::endl;
    }

    str.ignore();
    if(str.eof() && str.fail()) {   //设置eofbit后再次读取，设置failbit
        std::cout << "failed" << std::endl;
    }

    stringstream str1("h");
    str1.clear(std::ios::eofbit);
    str1.ignore();
    if(str1.eof() && str1.fail()){  //设置eofbit后再读取会设置failbit
        std::cout << "1-failed" << std::endl;
        std::cout << "1-readCount: " << str1.gcount() << std::endl;
    }
}


void cplusfiletest5(){
    stringstream str("hello\n");
    char array[100]{};

    str.ignore(5);   //提取字符
    str.unget();        //将上一个提取的字符放回去
    str.getline(array, 100);
    std::cout << "array: " << array << std::endl;
}

void cplusfiletest6(){
    stringstream str;
    str.put('h');
    str.write("ello", 4);
    std::cout << "str: " << str.str() << std::endl;
}





int main(int argc, char* argv[]){

    cplusfiletest4();

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}


