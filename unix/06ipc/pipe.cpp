
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;


/*
 * linux环境下，进程地址空间相互独立，每个进程各自有不同的用户地址空间。
 * 任何一个进程的全局变量在另一个进程中都看不到，所以进程和进程之间不能相互访问。
 * 交换数据必须通过内核，在内核中开辟一块缓冲区，进程1把数据从用户空间拷贝到内核缓冲区，
 * 进程2再从内核缓冲区把数据读走，内核提供的这种机制称为进程间通信。
 *
 * 常用的进程间通信方式
 *      管道（使用最简单）
 *      信号（开销最小）
 *      共享映射区（无血缘关系）
 *      本地套接字（最稳定)
 */

/*
 * 管道
 * 管道是一种最基本的IPC机制，作用于有血缘关系的进程之间，完成数据传递。
 *  1.本质是一个伪文件（实际为内核缓冲区）
 *  2.由2个文件描述符引用，一个表示读端，一个表示写端。
 *  3.规定数据从管道的写端流入管道，从读端流出。
 *
 * 局限性
 *  数据自己读不能自己写
 *  数据一旦被读走，便不再在管道中存在，不可反复读取
 *  采用的是半双工通信方式，一条管道的数据只能有一个流动方向
 *  只能在有公共祖先的进程间使用管道
 */


/*
 * int pipe(int pipefd[2]); 成功:0； 失败:-1； 设置errno.
 *  函数调用成功后返回r/w两个文件描述符。无需open，但是需要手动close。fd[0] --> read; fd[1] --> write.
 *
 * 使用方式：
 *  1. 父进程调用pipe函数创建管道，得到2个文件描述符fd[0],fd[1],指向管道的读端和写端。
 *  2. 父进程调用fork()创建子进程，那么子进程也有2个文件描述符指向同一管道。
 *  3. 一端关闭读端，一端关闭写端。数据从写端流入，读端流出，实现了通信。
 *
 * 管道的读写行为：
 *      读管道：
 *          1.管道中有数据, read返回实际读到的字节数。
 *          2.管道中无数据：
 *              .管道写端被全部关闭, read返回0(好像读到文件结尾)
 *              .写端没有全部关闭, read阻塞等待
*        写管道：
 *          1.管道读端全部被关闭, 进程异常终止(也可捕捉SIGPIPE信号，使进程不终止)
 *          2.管道读端没有全部关闭：
 *              管道已满, write阻塞。
 *              管道未满, write将数据写入，并返回实际写入的字节数
 *
 * 管道的缺点：
 *      1. 只能单向通信, 双向通信需建立两个管道。
 *      2. 只能用于父子、兄弟进程间的通信。该问题后来使用fifo有名管道解决。
 */



/*
 * 1. 父进程创建一个管道
 * 2. 创建子进程, 子进程持有管道描述符
 * 3. 父子进程各自关闭管道的一端, 数据在管道之间单向流动.
 */
void ipc_pipe(void){
    int fd[2];  //fd[0]存储读描述符, fd[1]存储写描述符。
    int ret = pipe(fd);
    if(ret == -1){  //管道创建失败
        perror("pipe");
        exit(-1);
    }

    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){ //子进程写
        close(fd[0]);   //关闭读描述符
        string str = "a test string from child\n";
        write(fd[1], str.c_str(), str.length());    //将数据写入fd[1]
        close(fd[1]);
        exit(0);
    }else if(pid > 0){ //父进程读取
        close(fd[1]);   //关闭写描述符
        char readBuffer[1024];
        int readlength = read(fd[0], readBuffer, sizeof(readBuffer)); //将数据从fd[0]读出
        write(STDOUT_FILENO, readBuffer, readlength);
        while(true){
            int status;
            if(waitpid(-1, &status, 0) > 0){
                if(WIFEXITED(status)){
                    std::cout << "child exits with status " << WEXITSTATUS(status) << std::endl;
                }else if(WIFSIGNALED(status)){
                    std::cout << "child is signaled by " << WTERMSIG(status) << std::endl;
                }else if(WIFSTOPPED(status)){
                    std::cout << "child is stopped by " << WSTOPSIG(status) << std::endl;
                }
            }else{
                std::cout << "no child needed to wait, parent process complete..." << std::endl;
                break;
            }
        }
        close(fd[0]);
    }
    return;
}



/*
 * 将STDIN_FILENO指向管道的读端
 * 将STDOUT_FILENO指向管道的写端
 * 这样本来对标准输入输出文件进行操作的函数, 就可以对管道进行操作.
 */
int ipc_pipe1(void)
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(-1);
    }else if (pid == 0) {  //child
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);		    //STDIN_FILENO指向管道的读端
        execlp("wc", "wc", "-l", nullptr);	//wc命令默认从STDIN_FILENO指向的文件读取数据, 现在就是从管道读取数据.
    }else if(pid > 0){
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);		    //STDOUT_FILENO指向管道的写端
        sleep(2);
        execlp("ls", "ls", "-l", nullptr);	//ls的结果默认是输出到STDOUT_FILENO指向的文件, 现在STDOUT_FILENO指向管道,因此数据被写入到管道中
    }
    perror("execlp error");
    return -1;
}



/*
 * 2个子进程写管道, 父进程读管道
 */
int ipc_pipe2(void){
    int fd[2], i;
    pipe(fd);   //fork之前创建管道，通过返回的文件描述符可以访问内核提供的缓存，由于子进程是父进程的副本，所以子进程也拥有管道文件描述符。
    for(i = 0; i < 2; i++){
        pid_t pid = fork();
        if(pid == -1){
            perror("fork error");
            exit(-1);
        }else if(pid == 0){
            break;
        }
    }

    if(i == 0){ //子进程1
        close(fd[0]);   //管道只能单向通信
        sleep(1);
        write(fd[1], "hello", strlen("hello"));
        exit(1);
    }else if (i == 1){  //子进程2
        close(fd[0]);
        sleep(2);
        write(fd[1], "world", strlen("world"));
        exit(2);
    }else if(i == 2){
        close(fd[1]);
        while(true){   //当子进程结束后管道的写端都会关闭, 此时父进程再读管道, read返回0.
            char buf[1024]{};
            int length = read(fd[0], buf, 1024);
            if(length > 0){
                std::cout << "readStr: " << string(buf, length) << std::endl;
            }else if(length == 0){
                std::cout << "opposite end is closed..." << std::endl;
                break;
            }else if(length < 0){
                perror("read error");
                exit(-1);
            }
        }

        while(true){
            int status;
            if(pid_t handledPid = waitpid(-1, &status, 0) > 0){
                if(WIFEXITED(status)){
                    std::cout << "child " << handledPid << " exits with status " << WEXITSTATUS(status) << std::endl;
                }else if(WIFSIGNALED(status)){
                    std::cout << "child " << handledPid << " is signaled by " << WTERMSIG(status) << std::endl;
                }else if(WIFSTOPPED(status)){
                    std::cout << "child " << handledPid << " is stopped by " << WSTOPSIG(status) << std::endl;
                }
            }else{
                std::cout << "no child needed to wait, parent process complete..." << std::endl;
                break;
            }
        }
        exit(0);
    }
    exit(-1);
}





int main(int argc, char* argv[]){
    ipc_pipe2();


    return 0;
}




