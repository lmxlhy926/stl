
#include <cstdlib>
#include <ctime>

#include <string>
#include <iostream>
#include <sstream>
#include <thread>

#include <unistd.h>
#include <fcntl.h>
using namespace std;

/**
 * 常用的文件操作系统调用：
 *      open    : 打开文件，获取文件描述符，文件描述符指向文件表项
 *      close   ：关闭文件
 *      read    ：从文件读取指定数量的字节到指定的内存区域中
 *      write   ：将指定内存区域的指定数量的字节写入到文件中
 *      lseek   ：定位到文件的指定的位置。读取和写入从当前位置开始。以读写方式打开文件，切换读写时要调用lseek。
 * 
 * 
 * 打开文件时指定oflag参数：
 *      必选且只能选定一个：
 *                  O_RDONLY, O_WRONLY, O_RDWR, O_EXEC
 *      常用可选项:
 *                  O_CREAT     文件不存在则按照指定的文件权限创建一个新文件
 *                  O_TRUNC     打开时将文件长度截断为0
 *                  O_APPEND    在每次写操作之前，将文件偏移量设置在文件的当前结尾处。
 * 
 *      默认打开文件时，文件偏移量指示在文件开头处，每次从当前偏移量处开始进行操作。
 */


//获取当前偏移量
off_t getCurrentPosition(int fd){
    return lseek(fd, 0, SEEK_CUR);
}


/**
 * 文件偏移量指示当前要操作的位置
 * 读、写操作都从当前文件偏移量处开始，并使偏移量增加读写的字节数。
 * 开始位置在0，读取5个字节数据，位置在5；从位置5开始写入5个字节数据，位置指向10
 */
void offset_test(){
    char readBuf[1024];
    int fd = openat(AT_FDCWD, "a.txt", O_RDWR | O_CREAT, 0666);
    printf("pos: %ld\n", getCurrentPosition(fd));   //初始位置，位置0
    read(fd, readBuf, 5);
    printf("pos: %ld\n", getCurrentPosition(fd));   //读取5个字节，位置5
    write(fd, "world", 5);
    printf("pos: %ld\n", getCurrentPosition(fd));   //从位置5写5个字节，位置10
    close(fd);
}


/**
 * 打开指定路径的文件，获取操作文件的文件描述符
 * 通过定位文件获取文件长度
 * 循环读取文件内容，将内容输出到标准输出
 * 读取到文件末尾时，结束读取，关闭文件描述符
 * 
 */
void readFileContent(){
    int fd = openat(AT_FDCWD, "a.txt", O_RDWR | O_CREAT, 0666);
    off_t size = lseek(fd, 0, SEEK_END);    //通过定位到尾端确定文件长度
    lseek(fd, 0, SEEK_SET);                 //重新定位到开头准备读取文件
    char buf[size + 1];
    while(true){
        ssize_t nRead = read(fd, buf, size);
        if(nRead < 0){  //读取错误
            perror("read error");
            exit(-1);
        }else if(nRead == 0){  //读到文件末尾时，返回0。
            printf("reach the end of the file ... \n");
            break;
        }else if(nRead > 0){   //实际读取到的字节数
            buf[nRead] = 0;
            printf("readContent: %s\n", buf);
        }
    }
    close(fd);  //关闭文件描述符
}


/*
 * O_APPEND:
 *      写操作之前，不管当前文件偏移量在哪里，文件偏移量移动到文件末尾，这2步是一个原子操作
 *      具体步骤是用文件i节点表项的文件长度来更新文件表项中的偏移量
 */
void append_test(){
    int fd = open("./a.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    lseek(fd, 0, SEEK_SET);     //定位到文件开头
    printf("1pos: %ld\n", getCurrentPosition(fd));
    write(fd, "world", 5);      //不管当前位置在哪里，<先定位到文件末尾，然后开始写操作>
    printf("2pos: %ld\n", getCurrentPosition(fd));
    lseek(fd, 0, SEEK_SET);     //定位到文件起始处
    char buf[5 + 1]{};
    read(fd, buf, 5);
    buf[5] = 0;
    printf("readBuf: %s\n", buf);
    printf("3pos: %ld\n", getCurrentPosition(fd));
    write(fd, "12345", 5);            //再写入数据
    printf("4pos: %ld\n", getCurrentPosition(fd));
    close(fd);
}


/**
 *      lseek仅将当前的文件偏移量记录在内核中，它并不引起任何I/O操作。然后该偏移量用于下一个读或者写操作。
 *      文件偏移量可以大于文件的当前长度，在这种情况下，对文件的下一次写将加长该文件并在文件中构成一个空洞，这一点是允许的。
 * 位于文件中但没有写过的字节都被读为0
 *      尽管可以实现64位文件偏移量，但是能否创建一个大于2GB的文件则依赖于底层文件系统的类型。
 */
void filehole(){
    int fd = open("./file.hole", O_RDWR | O_CREAT, 0666);
    write(fd, "hello", 5);          
    lseek(fd, 100, SEEK_SET);         
    write(fd, "world", 5);            

    lseek(fd, 0, SEEK_SET);     //定位到文件开头
    char readBuf[105];
    read(fd, readBuf, 105);
    for(int i = 0; i < 105; ++i){
        printf("%d\n", readBuf[i]); //位于文件中但是没有被写过的字节都被读为0
    }
    close(fd);
}


/**
 * STDIN_FILENO、STDOUT_FILENO、STDERR_FILENO
 * 标准输入、标准输出、标准错误
 */
void stdinout(){
#define BUFFSIZE 1024
    stringstream ss;
    ss << "start: " << time(nullptr) << std::endl;
    write(STDERR_FILENO, ss.str().c_str(), ss.str().size());

    ssize_t nRead;
    char buf[BUFFSIZE];
    while((nRead = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){    //从标准输入读取数据
        if(write(STDOUT_FILENO, buf, nRead) != nRead){         //将数据写至标准输出
            fprintf(stderr, "write error...\n");
            exit(-1);
        }
    }

    if(nRead < 0){  //读错误导致的异常退出
        fprintf(stderr, "read error....\n");
    }

    stringstream ss1;
    ss1 << "end: " << time(nullptr) << std::endl;
    write(STDERR_FILENO, ss1.str().c_str(), ss1.str().size());
}


/*
 * 同一个文件在同一个进程中打开2次
        2个文件描述符指向2个不同的文件表项，但是2个文件表项指向的是同一个i节点表项.
 *      由于指向的是2个文件表项，每个文件表项有独立的文件位置偏移量，所有会发生数据覆盖的问题.
 *      使用O_APPEND可以解决数据覆盖的问题
 */
void openFileTwice(){
    int fd1 = open("a.txt", O_WRONLY | O_CREAT , 0666);
    int fd2 = open("a.txt", O_WRONLY | O_CREAT , 0666);
    std::cout << "fd1: " << fd1 << ", fd2: " << fd2 << std::endl;

    thread thread1([fd1]{
        for(int i = 0; i < 100; ++i){
            write(fd1, "hello-", 6);
        }
    });

    thread thread2([fd2]{
       for(int i = 0; i < 100; ++i){
           write(fd2, "world-", 6);
       }
    });

    thread1.join();
    thread2.join();
}


/*
 *  复制文件描述符，使得多个文件描述符指向同一个文件
 */
void dupTest(){
    /**
     * fd1和fd2都指向标准输出
     * dup和dup2都复制文件描述符，区别在于dup2可以指定目标文件描述符，dup不能指定
    */
    int fd1 = dup(STDOUT_FILENO); 
    int fd2 = dup2(STDOUT_FILENO, 4);
    std::cout << "fd1: " << fd1 << ", fd2: " << fd2 << std::endl;

    /**
     * fd1和fd2文件描述符指向同一个文件表项，文件表项指向一个i节点表项。
     * 因为指向的是同一个文件表项，通过多个文件描述符操作和通过一个文件描述符操作的结果是一样的。
    */
    thread thread1([fd1]{
        for(int i = 0; i < 100; ++i){
            write(fd1, "hello-", 6);
        }
    });
    thread thread2([fd2]{
        for(int i = 0; i < 100; ++i){
            write(fd2, "world-", 6);
        }
    });

    thread1.join();
    thread2.join();
}


/**
 * 打印文件表项的状态位
*/
void printFlags(int flags){
    switch(flags & O_ACCMODE){    //取得访问方式位
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;
        default:
            printf("unknown access mode");
    }

    if(flags & O_APPEND)
        printf(", append");
    if(flags & O_NONBLOCK)
        printf(", nonblocking");
    if(flags & O_SYNC)
        printf(", synchronous writes");

    printf("\n");
}


//设置标志位
int set_fl(int fd, int flags){
    int val;
    if((val = fcntl(fd, F_GETFL, 0)) == -1){     //获取标志位
        perror("fcntl F_GETFL error");
        return -1;
    }

    val |= flags;   //设置标志位

    if(fcntl(fd, F_SETFL, val) == -1){          //存储标志位
        perror("fcntl F_SETFL error");
        return -1;
    }
    return 0;
}

/**
 * 文件状态位测试：
 *      有些状态位只能在打开文件的时候设置，有些标志位可以在打开文件后用fcntl设置
*/
int fileFlags_test(){
    int fd = open("./a.txt", O_RDONLY | O_CREAT, 0777);
    set_fl(fd, O_RDWR | O_APPEND | O_SYNC | O_NONBLOCK);
    int flags;
    if((flags = fcntl(fd, F_GETFL, 0))  == -1){
        perror("fcntl error");
        return -1;
    }
    printFlags(flags);
    return 0;
}


int main(int argc, char* argv[]){

    return 0;
}





