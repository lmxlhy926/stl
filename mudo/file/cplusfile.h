//
// Created by 78472 on 2021/12/16.
//

#ifndef STL_CPLUSFILE_H
#define STL_CPLUSFILE_H

#include <iostream>
#include <sstream>
#include <fstream>


/*
 * 和C的文件访问机制相比, c++ file stream class的最大好处就是文件的自动管理.
 * 文件可在构造时期自动打开, 析构时期自动关闭.
 *
 * 对于可读又可写的stream, 不能在读写动作之间任意转换其读写属性,
 * 如果一定要转换, 必须进行一个seek动作, 到达当前位置, 再转换读写属性
 * 唯一例外是, 如果已经读到end-of-file, 可立即接着写入字符. 如果违背该规则, 将导致不确定性结果.
 *
 * 执行文件的打开动作后, 成功与否会反映在stream状态中, 因该在构造stream后检查其状态
 *
 * 自c++11开始, file stream提供了rvalue和move语义（move构造函数, move assignment操作符）
 * 因此, 可以使用临时创建的stream对象
 * 可以传入一个file stream作为函数实参, 或让某个函数返回一个file stream. 即可以做到file对象被使用超过其“被创建时的作用域”
 *
 */



/*
打开模式
    起始位置：
        r           in
        r+          in | out

    先清空：
        w           out 或 out | trunc
        w+          in | out | trunc

    尾端添加：
        a           app 或 out | app
        a+          in | app 或 in | out | app

    可读可写时, 当切换读写时必须调用一个seek操作
 */


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
 *
 * 输出：
 *      1. 输出一个字符
 *      2. 输出一个字符串
 */

using namespace std;

class cplusfile {

public:
/*
 * 读取一个字符
 * stringstream的默认打开方式：r+
 */
    static void getCharTest(){
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
    static void getUntilDelimiterTest(){
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
            if(str.eof() && !str.fail()){
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
        if(str1.fail() && !str1.eof()){    //没有遇到end-of-file，但是读取失败没有读到任何字符，只设置failbit
            std::cout << "failed: " << "array1: " << array << ", count: " << str1.gcount() << std::endl;
        }
    }


/*
 * 读取一行数据，读取但是不存储newline。最多读取count - 1个字符，自动添加'\0'。
 * 读取到数据但是遇到文件末尾，只设置eofbit。
 * 没有读取到数据，且遇到文件末尾，同时设置eofbit和failbit
 */
    static void getLineTest(){
        stringstream  str("123\nhello\nss");
        char array[100]{};

        while(str.getline(array, 100, '\n')){   //第三次读取，读取到数据且遇到文件末尾，只设置eofbit
            std::cout << "array: " << array << ", count: " << str.gcount() << std::endl;
            if(str.eof() && !str.fail()){
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
    static void getSomeCharTest(){
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
            array1[readCount] = '\0';
            std::cout << "read: " <<  array1 << " readCount: " << readCount << std::endl;
        }
        if(str1.good()){
            std::cout << "readCount: " << readCount << std::endl;
        }
    }


/*
 * 读取字符并忽略
 * 遇到文件末尾，只设置eofbit. 已达到文件末尾还继续读取，则会设置failbit。
 */
    static void getIngoreCharTest(){
        stringstream str("hl");
        str.ignore(100);    //提取时遇到文件末尾，设置eofbit
        if(str.eof() && !str.fail()){
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


/*
 * 将读取到的字符放回去
 * 一般在2次读操作之间，只能放回去一次
 */
    static void ungetCharTest(){
        stringstream str("hello\n");
        char array[100]{};

        str.ignore(5);   //提取字符
        str.unget();        //将上一个提取的字符放回去
        str.getline(array, 100);
        std::cout << "array: " << array << std::endl;
    }


/*
 * 输出一个字符，输出一个字符串
 */
    static void putCharStringTest(){
        stringstream str;
        str.put('h');
        str.write("ello", 4);
        std::cout << "str: " << str.str() << std::endl;
    }


/*
 * 基本文件操作
 */
    static void fileBaseTest(){
        string path = R"(D:\project\stl\mudo\file\cppfile\log.txt)";
        fstream file(path, std::ios::in | std::ios::out);  //构造函数自动按照指定模式打开文件
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


/*
 * 文件类只决定文件可以进行的操作，文件标识是传给streambuf使用的
 * std::ios::app会导致只在尾端执行写操作
 * std::ios::ate: 文件打开后位置跳到末尾
 */
    static void ofstreamTest(){
        string path = R"(D:\project\stl\mudo\file\cppfile\log.txt)";
        ofstream file;

        file.open(path, std::ios::in | std::ios::out | std::ios::ate);
        file.write("world\n", 6);
        file.seekp(0, std::ios::beg);
        file.write("hi\n", 3);
        file.close();

        fstream file1;
        file1.open(path, std::ios::in | std::ios::out | std::ios::app);
        file1.seekp(0, std::ios::beg);
        std::cout << "position before: " <<  file1.tellp() << std::endl;
        file1 << "ww";
        std::cout << "position after: " <<  file1.tellp() << std::endl;
    }


/*
 *  定位到开头，覆写一个字符
 *  定位到结尾，添加一个字符
 */
    static void ostringstreamTest(){
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
    static void istringstreamTest(){
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
    static void stringstreamTest(){
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


};



















#endif //STL_CPLUSFILE_H
