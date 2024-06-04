
/**
 * 常用的进程间通信方式
 *      管道（使用最简单）
 *      信号（开销最小）
 *      共享映射区（无血缘关系）
 *      本地套接字（最稳定)
 * 
 *      linux环境下，进程地址空间相互独立，每个进程各自有不同的用户地址空间。任何一个进程的全局变量在另一个进程中都看不到，
 * 所以进程和进程之间不能相互访问。交换数据必须通过内核，在内核中开辟一块缓冲区，进程1把数据从用户空间拷贝到内核缓冲区，
 * 进程2再从内核缓冲区把数据读走，内核提供的这种机制称为进程间通信。
 * 
 * 管道是UNIX系统IPC的最古老形式，所有UNIX系统都提供此种通信机制。
 *      (1) 历史上，它们是半双工的。
 *      (2) 管道只能在具有公共祖先的2个进程之间使用。
 *      每当在管道中键入一个命令序列，让shell执行时，shell都会为每一条命令单独创建一个进程，然后用管道将
 * 前一条命令进程的标准输出与后一条命令的标准输入相连接。   
 * 
 * 
 * 管道是一种最基本的IPC机制，作用于有血缘关系的进程之间，完成数据传递。
 *      1.本质是一个伪文件（实际为内核缓冲区）
 *      2.由2个文件描述符引用，一个表示读端，一个表示写端。
 *      3.规定数据从管道的写端流入管道，从读端流出。
 *
 * 局限性
 *      采用的是半双工通信方式，一条管道的数据只能有一个流动方向
 *      数据自己读不能自己写
 *      数据一旦被读走，便不再在管道中存在，不可反复读取      
 *      只能在有公共祖先的进程间使用管道
 * 
 * int pipe(int pipefd[2]); 成功:0； 失败:-1； 设置errno.
 *      函数调用成功后返回r/w两个文件描述符。无需open，但是需要手动close。fd[0] --> read; fd[1] --> write.
 *
 * 使用方式：
 *      1. 父进程调用pipe函数创建管道，得到2个文件描述符fd[0],fd[1],指向管道的读端和写端。
 *      2. 父进程调用fork()创建子进程，那么子进程也有2个文件描述符指向同一管道。
 *      3. 一端关闭读端，一端关闭写端。数据从写端流入，读端流出，实现了通信。
 *
 * 管道的读写行为：
 *      读管道：
 *          1.管道中有数据, read返回实际读到的字节数。
 *          2.管道中无数据：
 *              .管道写端被全部关闭, read返回0(读到文件结尾)*
 *              .写端没有全部关闭, read阻塞等待
*        写管道：
 *          1.管道读端全部被关闭, 进程异常终止(也可捕捉SIGPIPE信号，使进程不终止)*
 *          2.管道读端没有全部关闭：
 *              管道未满, write将数据写入，并返回实际写入的字节数
 *              管道已满, write阻塞。
 *              
 * 
 * 当管道的一端被关闭后，下列2条规则起作用。
 *      （1）当读一个写端已被关闭的管道时，在所有数据都被读取后，read返回0，表示文件结束。  
 *      （2）如果写一个读端已被关闭的管道，则产生信号SIGPIPE。如果忽略该信号或者捕捉该信号并从其处理程序返回，
 * 则write返回-1，errno设置为EPIPE。
 * 
 * 多进程写管道，避免交叉写入的方法:
 *      在写管道(FIFO)时，常量PIPE_BUF规定了内核的管道缓冲区大小。如果对管道调用write，而且要求写的字节数小于
 * 等于PIPE_BUF，则此操作不会与其它进程对同一管道(或FIFO)的write操作交叉进行。但是，若有多个进程同时写一个管道
 * 或FIFO，而且我们要求写的字节数超过PIPE_BUF，那么我们所写的数据可能会与其它进程所写的数据相互交叉。
 *
 * 
 * 管道的缺点：
 *      1. 只能单向通信, 双向通信需建立两个管道。
 *      2. 只能用于父子、兄弟进程间的通信。该问题后来使用fifo有名管道解决。
 * 
*/

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

void print_process_exit_status(int status){
    if(WIFEXITED(status)){  //正常退出，打印退出状态
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

    }else if(WIFSIGNALED(status)){  //异常退出，打印造成退出的信号
        printf("abnormal termination, signal<%s> %s\n", strsignal(WTERMSIG(status)),
        #ifdef WCOREDUMP
            WCOREDUMP(status) ? "(core file generated)" : "");
        #else
            "");
        #endif

    }else if(WIFSTOPPED(status)){   //进程暂停，打印造成暂停的信号
        printf("child stopped, signal<> = %s\n", strsignal(WSTOPSIG(status)));

    }else if(WIFCONTINUED(status)){
        printf("child continued....\n");
    }
}

/**
 * 建立父进程到子进程的管道
 * 子进程接收到父进程的数据后，将数据输出到标准输出
 * 注：
 *      管道对象只有一个
 *      fork后有2个文件描述符指向管道的读端，有2个文件描述符指向管道的写端
*/
void pipe1(){
    int fd[2]{};
    pid_t pid;
    char line[1024]{};
    /**
     * fork之前调用pipe，这样父子进程才能都拥有管道的控制端
    */
    if(pipe(fd) < 0){
        perror("pipe error");
        exit(-1);
    }
    printf("fd[0]: %d, fd[1]: %d\n", fd[0], fd[1]);

    if((pid = fork()) < 0){
        perror("fork err");
        exit(-1);

    }else if(pid == 0){
        close(fd[0]);   //关闭读
        write(fd[1], "helloworld\n", 11);   //写管道：通过unix接口
        dprintf(fd[1], "%d:%d\n", 1, 2);    //写管道：通过标准库
        exit(0xffe);

    }else if(pid > 0){
        close(fd[1]);   //关闭写
        sleep(1);
        int n = read(fd[0], line, 1024);    //读管道
        write(STDOUT_FILENO, line, n);
    }

    //回收子进程
    int status;
    waitpid(pid, &status, 0);
    print_process_exit_status(status);
    exit(0);
}


/**
 * execl执行程序前，指定程序的标准输入、标准输出、标准错误
*/
void pipe2(int argc, char* argv[]){
    //创建管道
    int fd[2];
    if(pipe(fd) < 0){   
        perror("pipe error");
        exit(-1);
    }

    //fork进程
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);

    }else if(pid == 0){     //执行的分页程序从管道读数据
        /**
         * 子进程关闭管道的写端，接收管道发送的数据
         * 将管道的读端指向的文件表复制到标准输入，此时标准输入和管道读端描述符都指向同一个文件表
         * 关闭管道读端描述符对管道文件表的索引，此时只有标准输入描述符索引管道文件表
        */

        //修改标准输入为管道的读端
        close(fd[1]);   
        if(fd[0] != STDIN_FILENO){
            if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
                perror("dup2 error");
                exit(-1);
            }
            close(fd[0]);
        }

        //修改标准输出为文件
        int fdout = open("./output", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        dup2(fdout, STDOUT_FILENO);
        close(fdout);

        /**
         * 执行分页进程
         * exec函数族继承当前进程的控制终端，即继承了当前进程的标准输入，即从管道读数据。
        */
        if(execlp("cat", "cat", nullptr) < 0){
            perror("execlp error");
            exit(-1);
        }
    }

    close(fd[0]);  //关闭管道读端
    FILE *fdout = fdopen(fd[1], "w");
    if(fdout == nullptr){
        printf("fdopen error ....\n");
        exit(-1);
    }

    char line[1024]{};
    while(fgets(line, 1024, stdin) != nullptr){
        fputs(line, fdout);
    }

    if(ferror(stdin)){
        perror("fgets error");
        exit(-1);

    }else if(feof(stdin)){  //读到文件末尾
        printf("fgets end normal ... \n");
    }
    fclose(fdout);   //关闭写端。这样管道的读端可以知道写端已结束

    printf("child pid: %d\n", pid);
    int status;
    waitpid(pid, &status, 0);
    print_process_exit_status(status);
    exit(0);
}



/**
 *      常见的操作是创建一个连接到另一个进程的管道，然后读其输出或向其输入端发送数据。
 *      为此，标准I/O库提供了2个函数popen和pclose。这2个函数实现的操作是：创建一个管道，fork一个子进程，
 * 关闭未使用的管道端，执行一个shell运行命令，然后等待命令终止。
 * 
 * #include <stdio.h>
 *  FILE* popen(const char *cmdstring, const char *type);
 *          返回值：若成功，返回文件指针；若出错，返回nullptr。
 *  
 *  int pclose(FILE *fp);
 *          返回值：若成功，返回cmdstring的终止状态；若出错，返回-1。
 * 
*/
void pipe3(int argc, char* argv[]){
    FILE *fpWrite;
    if((fpWrite = popen("cat > output", "w")) == nullptr){    //创建通信标准流
        perror("popen error");
        exit(-1);
    }

    /**
     * 从文件读取数据，将数据写入通信管道
    */
    char line[1024];
    while(fgets(line, 1024, stdin) != nullptr){
        if(fputs(line, fpWrite) == EOF){
            perror("fputs error to pipe");
            exit(-1);
        }
    }
    
    if(ferror(stdin)){
        printf("fgets error ... \n");
    }else if(feof(stdin)){
        printf("fgets end normal ... \n");
    }

    /**
     * 关闭父进程端通信管道，阻塞回收子进程。
    */
    pclose(fpWrite);
    exit(0);
}

void sig_handler(int signo){
    printf("caught sig %s\n", strsignal(signo));
}


/**
 *      FIFO有时候被称为命名管道。未命名的管道只能在2个相关的进程之间使用，而且这2个相关的进程
 * 还要有一个共同的创建了它们的祖先进程。但是通过FIFO，不相关的进程也能交换数据。
 * 
 * 当open一个FIFO时，非阻塞标志(O_NONBLOCK)会产生下列影响。
 *          在一般情况下(没有指定O_NONBLOCK)，只读open要阻塞到某个其它进程为写而打开这个FIFO为止。
 *      类似地，只写open要阻塞到某个其它进程为读而打开它为止。     
 * 
 *      类似管道，若write一个尚无进程为读而打开的FIFO，则产生信号SIGPIPE。若某个FIFO的最后一个写进程关闭了
 * 该FIFO，则将为该FIFO的读进程产生一个文件结束标志。
*/
void fifo(int argc, char* argv[]){
    signal(SIGPIPE, sig_handler);
    mkfifo("./cfifo", 0777);
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){
        FILE* fpRead = fdopen(open("./cfifo", O_RDONLY), "r");
        char line[1024];
        while(fgets(line, 1024, fpRead) != nullptr){
            if(strncmp(line, "quit", 4) == 0){
                printf("child process end, pid = %d\n", getpid());
                exit(0);
            }
            fprintf(stdout, "childRead: %s", line);
        }
        printf("child process end, pid = %d\n", getpid());
        exit(0);
    }
    
    char buffer[1024];
    int fifoFd_write = open("./cfifo", O_WRONLY);
    while(fgets(buffer, 1024, stdin) != nullptr){
        int length = strlen(buffer);
        write(fifoFd_write, buffer, length);
    }
    printf("parent process end, pid = %d\n", getpid());
}


void pipe_test(){

    //创建管道
    int fd[2];
    if(pipe(fd) < 0){
        perror("pipeError");
        exit(-1);
    }


    //fork
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);

    }else if(pid == 0){
        //子进程逻辑
        close(fd[0]);
        FILE *fdWrite = fdopen(fd[1], "w");
        if(fdWrite != nullptr){
            fprintf(fdWrite, "hello world from %s\n", "liuhiayang");
            fflush(fdWrite);
            sleep(3);
            fprintf(fdWrite, "hello world from %s\n", "liuhiayang");
            fflush(fdWrite);
        }
        fclose(fdWrite);
        exit(0);

    }else if(pid > 0){
        //父进程逻辑
        close(fd[1]);
        while(true){
            char buffer[1024]{};
            int nRead = read(fd[0], buffer, 1024);
            if(nRead != 0){
                printf("readContent: %s\n", buffer);
            }else{
                printf("end of read ...\n");
                break;
            }
        }
        close(fd[0]);
    }

    int status;
    waitpid(pid, &status, 0);
    print_process_exit_status(status);
}



int main(int argc, char* argv[]){

    // fifo(argc, argv);

    pipe_test();

    return 0;
}

