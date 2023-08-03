
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <bitset>
#include <complex>
#include <string>
#include <iosfwd>
#include <string>
#include <limits>
#include <unistd.h>
#include <fcntl.h>
#include <streambuf>
#include <sstream>

using namespace std;

/**
 * operator>> 和 operator<< 被相应的stream class重载，分别用于输入和输出。于是c++的移位操作符摇身一变成了I/O操作符。
 * c/c++的操作符<<和>>分别用于整数中位的左移和右移，然而basic_istream<>和basic_ostream<>重载了它们，使之成为标准I/O操作符。
 * 
 * basic_istream将>>定义为input操作符。
 * basic_istream也对几乎所有基础类型重载了operator>>，不包括void和nullptr_t，但包括char*和void*。
 * 
 * 和<<一样，我们也可对任意类型重载input操作符，并串联运用它们；
 * 为了让这成为可能，默认会跳过一开始的空白字符，但这项功能也可以关闭。
 * 
 * 在c++11起，并发输入采用相同的stream对象是可能的，但可能导致读入的字符并未在读取它的线程中有定义。
 * 
*/
void format_raad1(){
    int i;
    float f;
    string s;
    std::cin >> i >> f >> s;
    std::cout << "read: " << i << " " << f << " " << s << std::endl;
}


//格式化读取，每读取一次要验证流对象错误标志是否设置
void format_read2(){
   int read;
   while(std::cin >> read){   
        std::cout << "read: " << read << std::endl;
   }
   std::cout << "read error..." << std::endl;
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
void format_output(){
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



//打印输入流的状态
void printStreamState(std::ios& cin){
    if(std::cin.rdstate() & std::ios_base::eofbit){
        std::cout << "end of file" << std::endl;
    }
    if(std::cin.rdstate() & std::ios_base::failbit){
        std::cout << "fail" << std::endl;
    }
    if(std::cin.rdstate() & std::ios_base::badbit){
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
 * istream& istream::get(char* str, streamsize count)
 * istream& istream::get(char* str, streamsize count, char delim)
 *      * 调用者必须保证str足够存入count个字符；
 *      * 两种形式均可读取count-1个字符，再附加一个null字符，存入str所指向的字符序列中
 *      * 不会读取终止符delim
 * 
 *      读取终止条件：
 *          * 如果遇到delim或end-of-file，则停止。如果字符数目已经达到count-1个字符，则停止。
 *          * 遇到delim或end-of-file, 但是stream没有读取到任何内容，stream会报错
 * 
 * 目的：一次性读取，在遇到delim之前读取，遇到delim则读取结束
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
 *      * 读取delim，但是不会存储。这样在count个字符内有delim就行。
 *      
 *      读取终止条件：
 *          * 如果遇到delim则停止，如果在count个字符内没有遇到delim，则读取出错。即在指定字符内没有读取到一行，则读取出错。
 *          * 只要在count个字符内有delim就行，只有delim读取也是正确的。
 * 
 * 目的：在count个字符内读取到一行，如果没有读取到一行，则读取出错。只要读取到delim，即读取到一行，即使读取到的内容是空的。
*/
void get_line_test(){
    char buf[1024];
    while(std::cin.getline(buf, 3, ',')){
        std::cout << "read: " << buf << std::endl;
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
    if(std::cin.read(buf, 5)){
        std::cout << "readLine: " << string(buf, 5) << std::endl;
    }else{
        printStreamState(std::cin);
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //忽略本行剩余部分

    //读取一行内容并输出
    if(std::cin.getline(buf, 1024)){
        std::cout << "readLine: " << buf << std::endl;
    }else{
        printStreamState(std::cin);
    }
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

/**
 * 输出流：
 *      * 输出一个字符
 *      * 输出一个字符串
 *      * 刷新输出
*/
void output_test(){
    std::cout.put('a').put('b');
    std::cout.write("hello", 5);
    std::cout.flush();
}


class outbuf : public std::streambuf{
protected:
    virtual int_type overflow(int_type c) override{
        if(c != EOF){
            c = std::toupper(c);
            if(std::putchar(c) == EOF){
                return EOF;
            }
        }
        return c;
    }
};

void streambuf_test(){
    outbuf ob;
    std::ostream out(&ob);  //给ostream配置流缓冲区
    out << "你 ";
    out << 3 << " " << 3.14 << " helloworld" << " from china " <<  "你好" << std::endl;
}


//通过重载streambuf的overflow函数，定义输出的输出源
class fdoutbuf : public std::streambuf{
    protected:
        int fd;
    public:
        fdoutbuf(int _fd) : fd(_fd){}
    protected:
        virtual int_type overflow(int_type c) override{
            if(c != EOF){
                char z = c;
                if(write(fd, &z, 1) != 1){  //输出通道
                    return EOF;
                }
            }
            return c;
        }
};

    
//通过继承ostream，定义新的输出流
class fdostream : public std::ostream{
    protected:
        fdoutbuf buf;
    public:
        fdostream(int fd) : std::ostream(nullptr), buf(fd){
            rdbuf(&buf);    //配置流缓冲区
        }
};


void fdostream_test(const char* filepath){
    int fd = open(filepath, O_RDONLY | O_WRONLY);
    fdostream cout(fd);
    cout << 1 << " " << 2 << " helloworld" << std::endl;
}


//带缓冲的streambuf
class outbuf_buffer : public std::streambuf{
    protected:
        static const int bufferSize = 3;
        char buffer[bufferSize]{};
        int fd;
    public:
        outbuf_buffer(int fd_): fd(fd_) {
            setp(buffer, buffer + (bufferSize - 1));    //给streambuf设置缓冲区
        }

        virtual ~outbuf_buffer(){
           sync();
        }
    protected:
        //flush the characters in the buffer
        int flushBuffer(){
            int num = pptr() - pbase();
            if(write(fd, buffer, num) != num){
                return EOF;
            }
            pbump(-num);    //pptr回退到开始处
            return num;
        }

        //建立的这个write缓冲区，只要还剩一个字符空间，就会调用overflow()。buffer满了，输出c和之前的所有字符
        virtual int_type overflow(int_type c) override{
            if(c != EOF){
                *pptr() = c;    //将最后一个字符存入缓冲区。
                pbump(1);       //当前位置前进1。pptr超过epptr。
            }
            if(flushBuffer() == EOF){
                return EOF;
            }
            return c;
        }

        virtual int sync() override{
            if(flushBuffer() == EOF){
                return -1;
            }
            return 0;
        }

};


void streambuf_buffer_test(const char* filepath){
    int fd = open(filepath, O_RDONLY | O_WRONLY);
    outbuf_buffer ob(fd);
    std::ostream os(&ob);

    os << "hh";
    os << "aa";
    os.flush();

    sleep(1);
    os << "bb";

    sleep(1);
    std::cout << "quit..." << std::endl;
}


class inbuf : public std::streambuf{
    protected:
        static const int bufferSize = 10;   //缓冲区大小
        char buffer[bufferSize];    //缓冲区        
    public:
        inbuf(){
            /**
             * 设置缓冲区的初始指针
             *      * 保留四个空位用来存放回退字符
             *      * 初始时eback() == gptr() == egptr()，即无回退字符可读，也无缓冲字符可读
            */
            setg(buffer + 4, buffer + 4, buffer + 4);   
        }
    protected:
        virtual int_type underflow() override{
            /**
             * gptr() == egptr() 意味着缓冲区没有数据
             * 如果缓冲区有数据，则直接读取
            */
            if(gptr() < egptr()){
                return traits_type::to_int_type(*gptr());
            }

            /**
             * 缓冲区初始时没有数据，或者缓冲区数据已经被读取完毕，需要从外部表述读取新的数据。
             * 先计算当前位置和eback()的距离，决定要保存的回退字符数量
             * 这里最大保存4个回退字符
            */
            int numPutback;
            numPutback = gptr() - eback();
            if(numPutback > 4){
                numPutback = 4;
            }
            std::memmove((buffer + 4) - numPutback, gptr() - numPutback, numPutback);

            //读取新数据到缓冲区
            int num = read(0, buffer + 4, bufferSize - 4);
            if(num <= 0){
                return EOF;
            }

            //读取完新数据后，调整eback()，gptr()，egptr()。
            setg((buffer + 4) - numPutback, buffer + 4, (buffer + 4) + num);

            //返回当前指向的数据
            return traits_type::to_int_type(*gptr());
        }
};

void inbuf_test(){
    inbuf ib;
    std::istream in(&ib);
    char buf[1024]{};

    in.getline(buf, 1024);
    std::cout << "getline: " << buf << std::endl;

    for(int i = 0; i < 8; ++i){
        if(!in.unget()){
            std::cout << "---fails in unget()---" << std::endl;
            return;
        }
    }

    in.getline(buf, 1024);
    std::cout << "getline: " << buf << std::endl;

    in.peek();
}


void streambuf_iterator_test(){
    istreambuf_iterator<char> inpos(cin);
    istreambuf_iterator<char> endpos;
    ostreambuf_iterator<char> outpos(cout);
    copy(inpos, endpos, outpos);
    std::cout << "end..." << std::endl;
}


/**
 * stringstream
*/
void ostringstream_test(){
    ostringstream os;
    os << "dec: " << 15 << hex << " hex: " << 15 << endl;
    cout << os.str() << std::endl;

    bitset<8> b(7);
    os << "float: " << 4.67 << " bitset: " << b << endl;
    cout << os.str() << std::endl;

    os.seekp(0);
    os << "oct: " << dec << 22;
    cout << os.str() << std::endl;
}

void istringstream_test(){
    int x;
    float f;
    std::string s = "3.7";
    std::istringstream is(s);
    is >> x >> f;
    std::cout << "x: " << x << " f: " << f << std::endl;

    

    cout << "is.str(): " << is.str() << std::endl;
}



int main(int argc, char* argv[]){
    istringstream_test();

    return 0;
}


