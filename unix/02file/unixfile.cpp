
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

off_t getLocation(int fd){
    return lseek(fd, 0, SEEK_CUR);
}


/*
 * 1. 通过定位到文件末尾，确定文件长度
 * 2. 读取文件全部内容
 */
void readFileContent(){
    int fd = openat(AT_FDCWD, "a.txt", O_RDWR | O_CREAT, 0666);
    off_t size = lseek(fd, 0, SEEK_END);  //通过定位到尾端确定文件长度
    if(size > 0){
        char buf[size];
        lseek(fd, 0, SEEK_SET);            //定位到开头准备读取文件
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
    printf("pos: %ld\n", getLocation(fd));   //初始位置，位置0
    read(fd, readBuf, 5);
    printf("pos: %ld\n", getLocation(fd));   //读取5个字节，位置5
    write(fd, "world", 5);
    printf("pos: %ld\n", getLocation(fd));   //从位置5写5个字节，位置10
    close(fd);
}


/*
 * O_APPEND: 写操作之前，文件偏移量移动到文件末尾，这2步是一个原子操作
 */
void writeAppend(){
    int fd = open("a.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    lseek(fd, 0, SEEK_SET);     //定位到文件开头
    printf("pos: %ld\n", getLocation(fd));
    write(fd, "world", 5);             //不管当前位置在哪里，<先定位到文件末尾，然后开始写操作>
    printf("pos: %ld\n", getLocation(fd));
    lseek(fd, 0, SEEK_SET);     //定位到文件起始处
    printf("pos: %ld\n", getLocation(fd));
    write(fd, "12345", 5);            //再写入数据
    printf("pos: %ld\n", getLocation(fd));
    close(fd);
}


/*
 * lseek仅将当前的文件偏移量记录在内核中，它并不引起任何I/O操作。然后该偏移量用于下一个读或者写操作。
 *
 * 文件偏移量可以大于文件的当前长度，在这种情况下，对文件的下一次写将加长该文件并在文件中构成一个空洞，这一点是允许的。
 * 位于文件中但没有写过的字节都被读为0
 *
 * 尽管可以实现64位文件偏移量，但是能否创建一个大于2GB的文件则依赖于底层文件系统的类型。
 */
void filehole(){
    char buf1[] = "abcdefg";
    char buf2[] = "ABCDEFG";
    int fd = open("/home/lhy/file.hole", O_RDWR | O_CREAT, 0666);
    write(fd, buf1, 7);                 //pos: 7
    lseek(fd, 16384, SEEK_SET);   //pos: 16384
    write(fd, buf2, 7);                  //pos: 16391

    lseek(fd, 0, SEEK_SET);     //定位到文件开头
    char buf3[1024];
    read(fd, buf3, 16);
    for(int i = 0; i < 16; ++i){
        printf("%d\n", buf3[i]);      //位于文件中但是没有被写过的字节都被读为0
    }
    close(fd);
}


/*
 * 从标准输入读取数据，将数据写至标准输出
 * 重定位标准输入、标准输出，达到复制文件的目的
 * 将打印信息输出到标准错误
 */
void stdinout(){
#define BUFFSIZE 1024
    time_t begin = time(nullptr);
    stringstream ss;
    ss << "start: " << begin << std::endl;
    write(STDERR_FILENO, ss.str().c_str(), ss.str().size());

    ssize_t nRead;
    char buf[BUFFSIZE];
    while((nRead = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){    //从标准输入读取数据
        if(write(STDOUT_FILENO, buf, nRead) != nRead){             //将数据写至标准输出
            fprintf(stderr, "write error...\n");
            exit(-1);
        }
    }

    if(nRead < 0){
        fprintf(stderr, "read error....\n");
    }

    time_t end = time(nullptr);
    stringstream ss1;
    ss1 << "end: " << end << std::endl;
    write(STDERR_FILENO, ss1.str().c_str(), ss1.str().size());
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
 *  复制文件描述符，使得多个文件描述符指向同一个文件
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
    filehole();
    return 0;
}