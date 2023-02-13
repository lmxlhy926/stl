
#include <cstdlib>
#include <ctime>

#include <string>
#include <iostream>
#include <sstream>
#include <thread>

#include <unistd.h>
#include <fcntl.h>
using namespace std;

/*
 * oflag参数：
 *      必选且只能选定一个：
 *                  O_RDONLY, O_WRONLY, O_RDWR, O_EXEC
 *      常用可选项:
 *                  O_CREAT     文件不存在则按照指定的文件权限创建一个新文件
 *                  O_TRUNC     打开时将文件长度截断为0
 *                  O_APPEND    在每次写操作之前，将文件偏移量设置在文件的当前结尾处
 *                  都没有指定，则打开文件，文件偏移量指示在文件开头处，每次从当前偏移量处开始进行操作。
 */

/*
 * 1. 确定文件长度
 * 2. 读取文件，并打印文件内容
 */
void readFileContent(){
    int fd = openat(AT_FDCWD, "a.txt", O_RDWR | O_CREAT, 0666);
    off_t size = lseek(fd, 0, SEEK_END);    //定位到尾端确定文件长度
    std::cout << "size: " << size << std::endl;
    lseek(fd, 0, SEEK_SET);                 //定位到开头准备读取文件
    if(size > 0){
        char buf[size];
        ssize_t nRead = read(fd, buf, size);
        std::cout << string(buf, nRead) << ", size: " << nRead;
    }
    close(fd);
}


/*
 * 文件偏移量：读、写操作都从当前文件偏移量处开始，并使偏移量增加读写的字节数。
 * 开始位置在0，读取5个字节数据，位置在5；从位置5开始写入5个字节数据，位置指向10
 */
void positionForward(){
    char readBuf[1024];
    int fd = openat(AT_FDCWD, "a.txt", O_RDWR | O_CREAT, 0666);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    read(fd, readBuf, 5);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    write(fd, "world", 5);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    close(fd);
}


/*
 * O_APPEND: 写操作之前，文件偏移量移动到文件末尾
 */
void file3(){
    int fd = open("/home/lhy/test/a.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    write(fd, "hello", 5);  //先定位到文件末尾，然后开始写操作
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    lseek(fd, 0, SEEK_SET);     //定位到文件起始处
    char buf[1024];
    ssize_t nRead = read(fd, buf, 5);   //从文件起始处读
    std::cout << string(buf, nRead) << std::endl;
    close(fd);
}

/*
 * 从标准输入读取数据，将数据写至标准输出
 * 重定位标准输入、标准输出，达到复制文件的目的
 * 将打印信息输出到标准错误
 */
void stdinout(){
#define BUFFSIZE 1024
    time_t before = time(nullptr);
    stringstream beforess;
    beforess << before << std::endl;
    write(STDERR_FILENO, beforess.str().c_str(), beforess.str().size());

    ssize_t n;
    char buf[BUFFSIZE];
    while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
        if(write(STDOUT_FILENO, buf, n) != n){
            std::cout << "write error..." << std::endl;
            exit(-1);
        }
    }
    if(n < 0){
        std::cout << "read error...." << std::endl;
    }

    time_t after = time(nullptr);
    stringstream afterss;
    afterss << after << std::endl;
    write(STDERR_FILENO, afterss.str().c_str(), afterss.str().size());
}

/*
 * command > file   ：将输出重定向到file
 * command < file   ：将输入重定向到file
 * command >> file  ：将输出以追加的方式重定向到file
 * n>&m             ：将描述符n重定向到m
 */
void redirectionTest(){
    string str = "hello";
    write(STDOUT_FILENO, str.c_str(), 5);
    write(STDERR_FILENO, str.c_str(), 5);
}

/*
 *  dup, dup2
 */
void dupTest(){
    int fdFirst = dup(1);
    int fdSecond = dup2(1, 4);
    std::cout << "fdFirst: " << fdFirst << ", fdSecond: " << fdSecond << std::endl;

    thread thread1([fdFirst]{
        for(int i = 0; i < 100; ++i){
            write(fdFirst, "hello\n", 6);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    thread thread2([fdSecond]{
        for(int i = 0; i < 100; ++i){
            write(fdSecond, "world\n", 6);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    thread1.join();
    thread2.join();
}

int main(int argc, char* argv[]){
    positionForward();
    return 0;
}