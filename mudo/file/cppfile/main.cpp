
#include <iostream>
#include <thread>
#include <array>
#include <sstream>
#include <fstream>
#include <cstdlib>
using namespace std;


/*
 * 读取：
 *      1. 读取一个字符;
 *         **遇到eof，设置eofbit和failbit.
 *
 *      2. 读取字符串直到delimiter（包含或者不包含该字符)，但是都不存储该字符, 最多读取count - 1个字符
 *         **遇到eof,但是读取到字符，只设置eofbit
 *         **没有遇到eof，没有读取到数据，设置failbit
 *
 *      3. 读取固定数量的字符
 *          **read, 遇到eof导致没有读满字符，同时设置eofbit和failbit
 *          **readsome, 返回实际读取到的字符数，不设置标志
 *
 *      4. 提取并抛弃字符直到某个指定字符（该指定字符会被抛弃），最多提取count个字符
 *          **遇到文件末尾只设置eofbit
 *          **遇到文件末尾还继续提取，设置failbit
 *
 *      5. 放回上次读取的一个字符
 * 输出：
 *      1. 输出一个字符
 *      2. 输出一个字符串
 */


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



/*
 * 文件操作
 */
void cplusfiletest7(){
    string path = R"(D:\project\stl\mudo\file\cppfile\log.txt)";
    fstream file(path);  //构造函数自动按照指定模式打开文件
    if(!file){  //判断打开文件操作是否失败
        std::cerr << "cant open the file" << std::endl;
        exit(-1);
    }

    file << "first\n";
    file.flush();   //将缓存区内容写入文件
    file.close();   //关闭文件

    file.open(path);  //显示打开文件
    char array[100]{};
    while(file.getline(array, 100)){  //从文件中按行读取数据，直到遇到文件结尾导致eofbit和failbit被设置
        std::cout << "readline: " << array << std::endl;
    }
    file.clear();   //清空标志，因为后面还要使用该对象，标志只能显示清空
    file.close();   //显示关闭文件
}


void cplusfiletest8(){
    string path = R"(D:\project\stl\mudo\file\cppfile\log.txt)";
    ofstream file;

    file.open(path, std::ios::app);
    file.seekp(0, std::ios::beg);
    file.write("hello\n", 6);
    file.seekp(0, std::ios::beg);
    file.write("world\n", 6);
}

/*
 *  定位到开头，覆写一个字符
 *  定位到结尾，添加一个字符
 */
void cplusfiletest9(){

    ostringstream os("abcdefg");
    std::cout << "os1: " << os.str() << std::endl;
    os.seekp(0, std::ios::beg);     //定位到开头
    os << "1";
    os.seekp(0, std::ios::end);     //定位到结尾
    os << "2";
    std::cout << "os2: " << os.str() << std::endl;
    std::cout << std::endl;

    ostringstream oss("hello", std::ios::out | std::ios::ate);  //从末尾插入
    oss << "1";
    std::cout << "oss1: " << oss.str() << std::endl;
    oss.seekp(0, std::ios::beg);
    oss << "2";
    std::cout << "oss2: " << oss.str() << std::endl;
}


/*
 * 读取一行
 * 定位到开头，读取剩余的数据
 */
void cplusfiletest10(){
    istringstream is("first\nsecond\nthrid\n");
    char array[100]{};

    is.getline(array, 100);
    std::cout << "line1: " << array << std::endl;

    is.seekg(0, std::ios::beg);
    is.getline(array, 100);
    std::cout << "line1 again: " << array << std::endl;

    is.getline(array, 100);
    std::cout << "line2: " << array << std::endl;

    is.getline(array, 100);
    std::cout << "line3: " << array << std::endl;
}


/*
 * 读取，写入，读取
 */
void cplusfiletest11(){
    stringstream ss("hello");
    ss.seekp(0, std::ios::end);
    ss << " world";
    ss.seekg(0, std::ios::beg);

    char array[100]{};
    ss.getline(array, 100);
    std::cout << "array: " << array << std::endl;
    ss.clear();  //必须清除eofbit, 否则后续的操作全部无法成功

    ss.seekp(0, std::ios::beg);
    ss << "world";
    ss.seekp(0, std::ios::end);
    ss << " 123";

    std::cout << "ss: " << ss.str() << std::endl;
}

class outbuf : public streambuf{
public:
    explicit outbuf(){};
};

void cplusfiletest12(){
   outbuf ob;
   ob.sputc('a');
   ob.sputn("hello", 5);
}


#include <streambuf>


int main(int argc, char* argv[]){

    cplusfiletest11();

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}


