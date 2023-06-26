
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


/**
 * 当程序执行时，其main函数是如何被调用的?
 * 命令行参数是如何传递给新程序的?
 * 典型的存储空间布局是什么样式？
 * 如何分配另外的存储空间？
 * 进程如何使用环境变量？
 * 进程的各种不同终止方式？
 * longjmp和setjmp函数以及它们与栈的交互作用。
 * 
*/

/**
 * int main(int argc, char* argv[])
 * argc是命令行参数的数目，argv是指向参数的各个指针所构成的数组。
 * 
 * 当内核执行C程序时(使用一个exec函数)，在调用main前先调用一个特殊的启动例程。可执行程序文件
 * 将此启动例程指定为程序的起始地址。启动例程从内核取得命令行参数和环境变量值，然后为按上述方式
 * 调用main函数做好安排。
 * 
 * 启动例程是这样编写的，使得从main返回后立即调用exit函数。如果启动例程以C代码形式表示，实际该例程
 * 常常是用汇编语言编写的，则它调用main函数的形式可能是：
 * exit(main(argc, argv))
*/


static void myExit1(){
    printf("-->myExit handler1....\n");
}

static void myExit2(){
    printf("-->myExit handler2....\n");
}

void enter(){
    printf("-->enter start...\n");
    exit(0);
    printf("-->enter end...\n");
}

void exitTest(){
    atexit(myExit1);
    atexit(myExit2);
    atexit(myExit2);

    printf("main is done...\n");
    abort();
    enter();
}

//打印环境变量数组
int printEnviron(){
    for(int i = 0; *(environ + i) != nullptr; ++i){
        printf("%s\n", *(environ + i));
    }
    return 0;
}


//通过内存映射，可以用操作内存的方式，操作文件内容
void mmapTest(){
    int fd = open("/home/lhy/project/stl/unix/04processEnv/a.txt", O_RDWR);
    char *ptr = static_cast<char *>(mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    for(int i = 0; i < 7; ++i){
        printf("%c\n", *(ptr + i));
        *(ptr + i) = 49 + i;
    }

    FILE* fp = fdopen(fd, "a+");
    char buf[1024]{};
    printf("%s\n", fgets(buf, sizeof(buf), fp));
}


int main(int argc, char* argv[]){
    
    mmapTest();


    return 0;
}













