
#include <iostream>
#include <bitset>
#include <complex>
#include <string>
#include <iosfwd>
#include <string>
#include <limits>
#include <unistd.h>

using namespace std;

/**
 * operator>> 和 operator<< 被相应的stream class重载，分别用于输入和输出。于是c++的移位操作符摇身一变成了I/O操作符。
 * c/c++的操作符<<和>>分别用于整数中位的左移和右移，然而basic_istream<>和basic_ostream<>重载了它们，使之成为标准I/O操作符。
 * 
*/

void input(){
    int i;
    float f;
    string s;
    std::cin >> i >> f >> s;
    std::cout << "read: " << i << " " << f << " " << s << std::endl;
}

/**
 * basic_ostream将<<定义为output操作符，对所有基础(语言内建)类型均重载，不包括void和nullptr_t，但包括char*和void*。
 * 
 * 我们可以重载operator<<，使其第二实参为我们想要的任意数据类型，如此一来我们自己的数据类型就可以整合进入I/O系统。
 * 编译器负责调用正确的函数以输出第二实参。当然这个正确的函数的本分应该是把第二实参转换成字符序列发送给stream。
 * 
 * 输出机制的可扩展性，让用户自定义类型得以天衣无缝地融入I/O系统。比起C的printf()I/O机制，这是一个巨大的进步。
 * 程序员不再需要指定待打印的类型，只要针对不同类型进行重载，就可以保证编译器会自动推断出正确的打印函数。
 * 这一机制并不局限于标准类型，因此程序员可以利用这一机制完成所有类型的操作。
 * 
 * operator<<还可以在单一语句中打印多个对象。按规定它会返回第一实参，也就是ouput stream。
 * 
 * 自c++11起，并发输出采用相同的stream对象是可能的，但是可能导致交错的字符。
*/
void outputOperator(){
    //int
    std::cout << 7 << std::endl;

    //float
    std::cout << 4.5 << std::endl;

    //complex
    std::cout << std::complex<float>(3.1, 7.4) << std::endl;

    //bitset
    std::cout << std::bitset<10>(7) << std::endl;

    //const char*
    std::cout << "hello world" << std::endl;

    //void* 输出地址
    char buf[10]{'h', 'e', 'l', 'l', 'o', 0};
    void* ptr = reinterpret_cast<void*>(buf);
    std::cout << ptr << std::endl;
    std::cout << *(reinterpret_cast<char*>(ptr)) << std::endl;

    std::cout << "first " << "second " << "third " << std::endl;
}

/**
 * basic_istream将>>定义为input操作符。
 * basic_istream也对几乎所有基础类型重载了operator>>，不包括void和nullptr_t，但包括char*和void*。
 * 
 * 和<<一样，我们也可对任意类型重载input操作符，并串联运用它们；
 * 为了让这成为可能，默认会跳过一开始的空白字符，但这项功能也可以关闭。
 * 
 * 在c++11起，并发输入采用相同的stream对象是可能的，但可能导致读入的字符并未在读取它的线程中有定义。
*/
void inputOperator(){
    int i;
    float f;
    string s;
    std::cin >> i >> f >> s;
    std::cout << "i: " << i << " f: " << f << " s: " << s << std::endl;
}


void printStreamState(std::istream& cin){
    if(std::cin.rdstate() & std::ios::eofbit){
        std::cout << "end of file" << std::endl;
    }
    if(std::cin.rdstate() & std::ios::failbit){
        std::cout << "fail" << std::endl;
    }
    if(std::cin.rdstate() & std::ios::badbit){
        std::cout << "bad" << std::endl;
    }
}

/**
 * int istream::get()
 *      读取一个字符
 *      返回读取的字符或EOF
 *      对istream而言，返回类型是int, EOF则为常量EOF。
 *      返回值不能是字符类型，必须是范围更大的数值类型，否则就无法根据相应实值区分EOF和一般字符。
 * 
 * 读取一个字符后，需要判断istream的状态，来判断流的出错标志是否设置。
*/
void get_char_test(){
    int a;
    while(true){
        a = std::cin.get();
        if(std::cin){
            std::cout << "read: " << a << std::endl;
        }else{
            printStreamState(std::cin);
            break;
        }
    }
}

/**
 * istream& istream::get(char& c)
 *      读取一个字符到c中
 *      返回stream，其状态可说明是否读取成功
 *      比返回结果的get更加方便，可以一条语句完成读取和检测。
*/
void get_char_test1(){
    char c;
    while(std::cin.get(c)){
        std::cout << "read: " << c << std::endl;
    }
    printStreamState(std::cin);
}

/**
 * istream& istream::get(char* str, streamsize count)
 * istream& istream::get(char* str, streamsize count, char delim)
 *      * 调用者必须保证str足够存入count个字符
 *      * 两种形式均可读取count-1个字符，再附加一个null字符，存入str所指向的字符序列中
 *      * 不会读取终止符delim
 * 
 *      读取终止条件：
 *          * 读取，直到遇到delim或end-of-file
 *          * 最多count-1个字符
 *          遇到delim, 但是stream没有读取到任何内容，stream会报错
 * 
 * 目的：在遇到delim之前读取，遇到delim则读取结束
 * 
*/
void get_chars_test(){
    char buf[1024]{};
    while(std::cin.get(buf, 3, ' ')){
        std::cout << "read: " << buf << std::endl;
    }
    printStreamState(std::cin);
}

/**
 * istream& istream::getline(char* str, streamsize count)
 * istream& istream::getline(char* str, streamsize count, char delim)
 *      * 调用者必须保证str足够存入count个字符
 *      * 两种形式均可读取count-1个字符，再附加一个null字符，存入str所指向的字符序列中
 *      * 读取delim，但是不会存储
 *      * 如果在count个字符内没有遇到delim，则读取出错。即在指定字符内没有读取到一行，则读取出错。
 *      * 只要在count个字符内有delim就行，只有delim读取也是正确的。
 * 
 * 目的：在count个字符内读取到一行，如果没有读取到一行，则读取出错。
*/
void get_line_test(){
    char buf[1024];
    while(std::cin.getline(buf, 3, ',')){
        std::cout << "read: " << buf << std::endl;
    }
    printStreamState(std::cin);
}


/**
 * istream& istream::read(char* str, streamsize count)
 *      * 读取count个字符，存入str中
 *      * 读取过程中如遇到end-of-file会出错，则failbit会被设置。
 * 
 * 目的：读取指定数量的字符，直到遇到end-of-file
*/
void read_test(){
    char buf[1024];
    while(std::cin.read(buf, 3)){
        std::cout << "read: " << string(buf, 3) << std::endl;
    }
    printStreamState(std::cin);
}

/**
 * istream& istream::ignore()
 * istream& istream::ignore(streamsize count)
 * istream& istream::ignore(streamsize count, int delim)
 *      * 所有形式均提取字符并舍弃不用
 *      * 第一种形式忽略一个字符
 *      * 第二种形式忽略多达count个字符
 *      * 第三种形式可忽略多达count个字符，直到提取并舍弃delim。
 *      * 如果count的值等于std::numeric_limits<std::streamsize>::max()，那么delim或end-of-file之前的所有字符均被舍弃。
*/
void ignore_test(){
    //忽略最开始的5个字符，读取一行内容
    char buf[1024];
    std::cin.ignore(5);
    std::cin.getline(buf, 1024);
    if(std::cin){
        std::cout << "readLine: " << buf << std::endl;
    }else{
        printStreamState(std::cin);
        return;
    }
    
    //读取5个字符，忽略本行剩下的内容
    std::cin.read(buf, 5);
    std::cout << "readLine: " << string(buf, 5) << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //忽略本行剩余部分

    //读取一行内容并输出
    std::cin.getline(buf, 1024);
    std::cout << "readLine: " << buf << std::endl;
}

/**
 * int istream::peek()
 *      * 返回stream之内"下一个将被读取的字符"，但不真的将它读取出来。下一次读取结果便是该字符(除非改变了读取位置)。
 *      * 如果不再能够读入任何字符，返回EOF。
*/
void peek_test(){
    int c = std::cin.peek();
    if(c != EOF){
        std::cout << static_cast<char>(c) << std::endl;
    }else{
        printStreamState(std::cin);
        return;
    }
    char buf[1024];
    std::cin.getline(buf, 1024);
    std::cout << "readLine: " << buf << std::endl;
}

void output_test(){
    std::cout.put('a').put('b');
    std::cout.write("hello", 5);
    std::cout.flush();
}


void istream_input(){
    int read1 = std::cin.get();
    std::cout << "read1: " << read1 << std::endl;

    char read2;
    std::cin.get(read2);
    std::cout << "read2: " << read2 << std::endl;

    char buf[1024];
    std::cin.get(buf, 1024, '\n');
    std::cout << buf;

    std::cin.getline(buf, 1024, '\n');
    std::cout << buf;

    char readbuf[5];
    std::cin.read(readbuf, 5);
    std::cout << "readbuf: " << string(readbuf);

    std::cin.ignore(5, '\n').read(readbuf, 5);
    std::cout << "readbuf: " << string(readbuf);
}

void ostream_output(){
    std::cout.put('a') << std::endl;
    std::cout.write("hello", 5);
    std::cout.flush();
}

void test(){
   int read;
   while(!(std::cin >> read).fail()){
        std::cout << "read: " << read << std::endl;
   }
   std::cout << "read error..." << std::endl;
}



int main(int argc, char* argv[]){
    output_test();

    return 0;
}


