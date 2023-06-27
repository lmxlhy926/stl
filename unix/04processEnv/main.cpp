
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


/**
 * 退出函数
 *   #include <stdlib.h>
 *      void exit(int status);
 *      void _Exit(int status);
 *   #include <unistd.h>
 *      void _exit(int status);
 * 
 *      使用不同头文件的原因是exit和_Exit是由ISO C说明的，而_exit是由POSIX.1说明的。
 *      由于历史原因，exit函数总是执行一个标准I/O库的清理关闭操作；对所有打开流调用fclose函数。
 *      main函数返回一个整型值与用该值调用exit是等价的。在main函数中： return(0) == exit(0)
 *  
 *  atexit函数：
 *      按照ISO C规定，一个进程可以登记多至32个函数，这些函数将由exit自动调用。这些函数称为终止处理程序(exit handler)。
 *      exit调用这些函数的顺序与它们登记时候的顺序相反。同一个函数如果登记多次，也会被调用多次。
 *      根据ISO C和POSIX.1，exit首先调用各种终止处理程序，然后关闭所有打开流。
 *      POSIX.1扩展了ISO C标准，它说明，如若程序调用exec函数族中的任一函数，则将清除所有已安装的终止处理程序。
 * 
*/

static void atExitHandler1(){
    printf("-->atExitHandler1....\n");
}

static void atExitHandler2(){
    printf("-->atExitHandler2....\n");
}

void commonFunc(){
    printf("-->commonFunc start...\n");
    abort();    //进程接收到信号而异常终止，不执行exit函数。
    exit(0);
    printf("-->commonFunc end...\n");
}

/**
 * 终止处理程序是由exit来调用执行的
 * 无论exit在何处调用，只要exit被调用，则exit就会执行终止处理程序.
 * 如果进程是因为接收到某个信号而异常终止，则不会执行exit函数。
 * 
*/
void atexitTest(){
    atexit(atExitHandler1);
    atexit(atExitHandler2);
    atexit(atExitHandler2);
    commonFunc();
}


/**
 * 重定向标准输出到文件，标准输出变为全缓冲，即只有缓冲区满才会flush内容到文件中。
 * 进程因接收到信号而异常终止，因此不会调用exit。
 * 因此进程终止时没有对打开的流调用fclose，流中的内容不会flush到文件中。
 * 
*/
void exitFlushAbort(){
    for(int i = 0; i < 10; ++i){
         printf("hello world\n");
         usleep(100 * 1000);
    }
    abort();
}


/**
 * 重定向标准输出到文件，标准输出变为全缓冲，即只有缓冲区满才会flush内容到文件中。
 * 进程主动调用exit函数，正常结束。
 * exit会自动对所有打开的流调用fclose函数，流中缓存的内容会刷新到文件中。
 * 
*/
void exitFlushExit(){
    for(int i = 0; i < 10; ++i){
         printf("hello world\n");
         usleep(100 * 1000);
    }
    exit(0);
}


/**
 * 输出重定向的文件
 * 直接调用系统API，无缓冲输出。
 * 输出内容会立即交给内核进行输出。
 * 
*/
void exitFlushInstant(){
    sleep(1);
    for(int i = 0; i < 10; ++i){
        write(STDOUT_FILENO, "HELLO ", 6);
        usleep(100 * 1000);
    }
    write(STDOUT_FILENO, "HELLO", 5);
    abort();
}



/**
 * 命令行参数
 *      当执行一个程序时，调用exec的进程可将命令行参数传递给新程序。
 *      ISO C和POSIX.1都要求argv[argc]是一个空指针。
 *      执行execl传参时，最后一个参数是nullptr。
*/
void printCommandLine(int argc, char* argv[]){
    for(int i = 0; i < argc; ++i){
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    printf("\n");
    for(int i = 0; argv[i] != nullptr; ++i){
        printf("argv[%d]: %s\n", i, argv[i]);
    }
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
    printCommandLine(argc, argv);
    mmapTest();


    return 0;
}













