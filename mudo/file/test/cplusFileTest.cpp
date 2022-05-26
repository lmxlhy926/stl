#include <iostream>
#include <thread>
#include <array>
#include <sstream>
#include <streambuf>
#include <fstream>
#include <unistd.h>
#include <iomanip>

using namespace std;

class fdbuf : public std::streambuf{
protected:
    static const int bufferSize = 5;
    char buffer[bufferSize]{};  //内部缓冲区
    int _fd;
public:
    explicit fdbuf(int fd) : _fd(fd){
        std::cout << "==fdbuf constructor==" << std::endl;
        setp(buffer, buffer + (bufferSize - 1));    //设置缓冲区
    };

    ~fdbuf() override{     //析构时清空缓冲区
        sync();
    }

protected:
    std::streamsize xsputn(const char* s, std::streamsize num) override{
        std::cout << "==xsputn==" << std::endl;
        return write(_fd, s, num);
    }

    int flushBuffer(){
        std::cout << "==flushBuffer==" << std::endl;
        int num = static_cast<int>(pptr() - pbase());  //计算刷新输出的字符个数
        std::cout << "==>num: " << num << std::endl;
        if(write(_fd, buffer, num) != num){     //写入失败
            std::cout << "write to fd failed" << std::endl;
            return EOF;
        }
        pbump(-num);    //移动当前指针位置到缓冲区开头
        return num;
    }

    //写入的字符为EOF，或者刷新错误，返回EOF
    int_type overflow(int_type c) override{     //write缓冲区，只要还剩一个字符空间，就调用overflow()
        std::cout << "==overflow==" << std::endl;
        if(c != EOF){
            *pptr() = c;    //写入最后一个字符
            pbump(1);    //涂写位置向后移动一位
        }
        if(flushBuffer() == EOF)   //刷新缓冲区
            return EOF;

        return c;
    }

    int sync() override{    //刷新缓冲区。 当输入回车后，会自动调用此函数
        std::cout << "==sync==" << std::endl;
        if(flushBuffer() == EOF)
            return -1;
        return 0;
    }
};


class fdostream : public std::ostream{
protected:
    fdbuf buf;
public:
    explicit fdostream(int fd) : buf(fd), std::ostream(&buf){
        std::cout << "==fdostream constructor==" << std::endl;
    }
};


class outbuf : public std::streambuf{
protected:
    int_type overflow(int_type c) override{
        if(c != EOF){
           c = std::toupper(c);
           if(std::putchar(c) == EOF)
               return EOF;
        }
        return c;
    }
};


/*
 *  bool以数字表示：false为0，true为1。如果以数字方式读入boolean遇到0和1以外的数，就会出错。
 *  bool以文本表示：读入的数字需为true, false。
 */
void manip1(){
    bool b = false;
    std::cout << std::noboolalpha << "b: " << b << std::endl;
    std::cout << std::boolalpha << "b: " << b << std::endl;

    std::cout << "please input a boolValue: " << std::endl;
    while(!std::cin.fail()){
        std::cin >> std::boolalpha >> b;
        std::cout << "value: " << b << std::endl;
        std::cin.ignore();
    }

    std::cout << "input error" << std::endl;
}


/*
 * setw(value): 设置最小栏位宽度，栏位宽度的设定只到下一次格式化输出
 * setfill(char): 设置格式化表述和最小栏位宽度之间的字符
 * 设置对齐方式：左对齐，右对齐，左右对齐
 *
 * 运用栏位宽度来定义读取的最大字符数
 */
void manip2(){
    int num = 6;
    std::cout << std::setfill('-') << std::left << std::setw(6) << num << std::endl;
    std::cout << std::right << std::setw(6) << num << std::endl;
    std::cout << std::internal << std::setw(6) << std::showpos << num << std::endl;
    std::cout << std::right << std::setw(6) << "num" << std::endl;
}


/*
 * 进制设置：dec, oct, hex
 * 如果没有设置上述任何flag, 或者同时设置了上述多个flag, 输出时采用10进制
 * showbase, 如果设置就显示数值的进制
 */
void manip3(){
    std::cout << std::dec << "num: " << 16 << std::endl;
    std::cout << std::oct << "num: " << std::showbase << 16 << std::endl;
    std::cout << std::hex << "num: " << std::showbase << 16 << std::endl;

    std::cout << 0xf << std::endl;
    std::cout << 022 << std::endl;
    std::cout << 10 << std::endl;
}



int main(int argc, char* argv[]){

    manip3();


    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}


