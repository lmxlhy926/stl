
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

/**
 * UNIX系统的进程控制，包括创建新进程、执行程序和进程终止。
 * 
 * 进程标识：
 *      每个进程都有一个非负整型表示的唯一进程ID。
 *      ID为0进程通常是调度进程，常常被称为交换进程，是内核的一部分；
 *      进程ID1通常是init进程，在自举过程结束时由内核调用。init进程绝对不对终止，是所有孤儿进程的父进程。
*/

/**
 * fork:
 *      子进程是父进程的副本。子进程获得父进程数据空间、堆和栈的副本。注意，这是子进程所拥有的副本。
 *      父进程和子进程并不共享这些存储空间部分。父进程和子进程共享正文段。
 *      父子进程之间遵循读时共享写时复制的原则，目的是节省内存开销，逻辑上各个进程的变量之间是独立的
 * 
 * fork之后
 *      父子相同处：.data .text 堆 栈 
 *                 环境变量、控制终端、当前工作目录、文件描述符
 * 
 *      父子不同处：1.进程id  2.fork返回值  3.父进程Id  4.进程运行时间  
 *                 5.子进程不继承父进程设置的文件锁
 *                 5.子进程的未处理闹钟被清除
 *                 6.子进程的未处理信号集设置为空集
 * 
 *      注：相同的意思是值相同,不代表物理存储空间相同
 * 
*/

/**
 * 父子进程的执行顺序：
 *      一般来说，fork之后是父进程先执行还是子进程先执行是不确定的，这取决于内核所用的调度算法。
 *      如果要求父进程和子进程之间相互同步，则要求某种形式的进程间通信。
 *      fork之后可以使用信号使父进程和子进程同步。
 * 
 * fork时如何处理标准IO库的缓冲区内容：
 *      标准输出行缓冲：
 *          如果标准输出连接到终端设备，则它是行缓冲的；否则它是全缓冲的。
 *          当以交互方式运行该程序时，只得到该printf输出的行一次，其原因是标准输出缓冲区由换行符冲洗。
 *      标准输出全缓冲：
 *          但是将标准输出重定向到一个文件时，却得到printf输出行两次。其原因是，在fork之前调用了printf一次，但当调用fork时，
 *          该行数据仍在缓冲区中，然后在将父进程数据空间复制到子进程中时，该缓冲区数据也被复制到子进程中，此时父进程和子进程各自有
 *          了带该行内容的缓冲区。
 *      
 *      即取决于fork时，标准IO库的缓冲区内容是否被冲洗。如果已被冲洗，则没得复制；否则，复制到子进程的数据空间中。
 * 
 * 父子进程之间的文件共享：
 *      如果父进程和子进程写同一描述符指向的文件，但又没有任何形式的同步，那么它们的输出就会相互混合，
 *      这里假定所用的描述符是在fork之前打开的。但这不是常用的操作模式。
 *      
 *      父进程等待子进程完成：
 *          父进程无需对其描述符做任何处理。当子进程终止后，它曾进行过读、写操作的任一共享描述符的文件偏移量已做了相应更新。
 *      父进程和子进程各自执行不同的程序段
 *          fork之后，父进程和子进程各自关闭它们不需要使用的文件描述符，这样就不会干扰对方使用的文件描述符。
 * 
*/

//fork后父子进程的数据空间彼此独立，不会相互影响；
int globalVar = 6;  //.data区域
string out =  "a write to stdout\n";     
void forkTest(){
    int var;    //栈变量
    pid_t pid;

    var = 88;
    write(STDOUT_FILENO, out.data(), out.length());     //无缓冲写
    printf("before fork\n");    //标准库带缓冲写；行缓冲或全缓冲，取决于标准输出对应的文件。

    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){  //子进程
        globalVar++;
        var++;
    }else{  //父进程
        sleep(2);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), globalVar, var);
    exit(0);
}

/**
 *函数exit
 *      进程有5种正常终止和3种异常终止方式
 *      不管进程如何终止，最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等。
 * 
 *      常见3种正常终止方式：
 *          1. 在main函数内执行return语句。等效于调用exit。
 *          2. 调用exit函数。此函数由ISO C定义，其操作包括调用各种终止处理程序(终止处理程序在调用atexit函数时登记)，
 *             然后关闭所有标准IO流等。
 *          3. 调用_Exit函数。ISO C定义_Exit，其目的是为进程提供一种无需运行终止处理程序或信号处理程序而终止的方法。
 *             在UNIX系统中,_exit函数和_Exit函数是同义的，并不冲洗标准IO流。_exit函数由exit函数调用，它处理UNIX系统特定细节。
 *             exit(3)是标准C库中的一个函数，而_exit(2)则是一个系统调用。
 * 
 *      常见的异常终止：
 *          1. 调用abort。它产生SIGABRT信号，这是下一种异常终止的一种特例。
 *          2. 当进程接收到某些信号时。信号可由进程自身(如调用abort函数)、其它进程或内核产生。
 *             若进程引用地址空间之外的存储单元、或者除以0，内核就会为该进程产生相应的信号。
 *      
 *      终止进程如何通知其父进程自己是如何终止的？
 *          对于4个终止函数(return、exit、_exit、_Exit)，将其退出状态作为参数传送给函数。在异常终止情况，内核(不是进程本身)产生一个
 *          指示其异常终止原因的终止状态。这里使用了"退出状态"和"终止状态"两个术语，以表示有所区别。在最后调用_exit时，内核将
 *          退出状态转换成终止状态。如果子进程正常终止，则父进程可以获得子进程的退出状态。
 * 
 *      如果父进程在子进程之前终止，又将如何？
 *          对于父进程已经终止的所有进程，它们的父进程都改变为init进程。我们称这些进程由init进程收养。
 *          init被编写成无论何时只要有一个子进程终止，init就会调用一个wait函数取得其终止状态。
 *          这样就防止了在系统中塞满僵死进程。
 * 
 *      如果子进程在父进程之前终止，父进程如何获取子进程的终止状态？
 *          内核为每个终止子进程保存了一定量的信息，所以当终止进程的父进程调用wait或waitpid时，可以得到这些信息。
 *          这些信息至少包括进程ID、该进程的终止状态以及该进程使用的CPU时间总量
 *      
 * 
*/


/**
 * 函数wait和waitpid
 *      当一个进程正常或异常终止时，内核就向其父进程发送SIGCHLD信号。因为子进程终止是个异步事件，所以这种信号
 *      也是内核向父进程发的异步通知。父进程可以选择忽略该信号，或者提供一个该信号发生时即被调用执行的函数，即
 *      信号处理程序。对于这种信号的系统默认动作时忽略它。
 * 
 *      调用wait或waitpid的进程可能会发生什么：
 *          * 如果其所有子进程都还在运行，则阻塞。
 *          * 如果一个子进程已终止，正等待父进程获取其终止状态，则取得该子进程的终止状态并立即返回。
 *          * 如果它没有任何子进程，则立即出错返回。
 *          如果进程由于接收到SIGCHLD信号而调用wait，我们期望wait会立即返回。但如果在随机时间点调用wait，则进程可能会阻塞。
 *          如果status不是一个空指针，则存放终止进程的终止状态。如果不关心终止状态，则可将该参数指定为空指针。
 * 
 * pid_t wait(int *status);
 *      成功：返回清理掉的子进程ID； 失败：-1 (没有子进程)
 *      阻塞等待，直到有子进程结束；如果没有子进程则返回-1。
 *
 * pid_t waitpid(pid_t pid, int *status, in options);
 *      成功： 返回清理掉的子进程ID；    失败： -1(无子进程)；   0：options为WNOHANG, 子进程正在运行中
 *      pid：指定要回收状态的进程的ID
 *      opitons：可以指定不阻塞；可以指定关心处于停止状态的进程；可以指定关心停止后又继续的进程；    
 * 
 *      参数：
 *          pid：
 *              >0：回收指定 ID 的子进程
 *              -1：回收任意子进程（相当于 wait）
 *          status:
 *               接收进程返回状态
 *          options: 或者是0，或者是以下常量按位或运算的结果。
 *              0:          阻塞等待
 *              WNOHANG:    不阻塞
 *              WUNTRACED:  进程处于停止状态，并且其状态自停止以来还未报告过，则返回其状态。
 *              WCONTINUED: 子进程在停止后已经继续，但其状态尚未报告，则返回其状态
 *          
 *
 *      判断进程返回状态的宏函数
 *          WIFEXITED(status)   为非 0 → 进程正常结束
 *          WEXITSTATUS(status) 如上宏为真, 获取进程退出状态 (子进程传送给exit或_exit参数的低8位)
 *
 *          WIFSIGNALED(status) 为非 0 → 进程异常终止
 *          WTERMSIG(status)    如上宏为真， 使用此宏 → 取得使进程终止的那个信号的编号。
 *
 *          WIFSTOPPED(status)  为非 0 → 进程处于暂停状态
 *          WSTOPSIG(status)    如上宏为真， 使用此宏 → 取得使进程暂停的那个信号的编号。
 * 
 *          WIFCONTINUED(status) 非0 → 作业控制暂停后已经继续的子进程返回了状态
 * 
 *      对于wait，其唯一的出错是调用进程没有子进程(函数调用被一个信号中断时，也可能返回另一种出错)。
 *      对于waitpid，如果指定的进程或进程组不存在，或者参数pid指定的进程不是调用进程的子进程，都可能出错。
 */

void pr_exit(int status){
    if(WIFEXITED(status)){  //正常退出，打印退出状态
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

    }else if(WIFSIGNALED(status)){  //异常退出，打印造成退出的信号
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
        #ifdef WCOREDUMP
            WCOREDUMP(status) ? " (core file generated)" : "");
        #else
            "");
        #endif

    }else if(WIFSTOPPED(status)){   //进程暂停，打印造成暂停的信号
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));

    }else if(WIFCONTINUED(status)){
        printf("child continued....\n");
    }
}


void doulbeFork(){
    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){ //创建的第一个子进程
        if((pid = fork()) < 0){
            printf("fork error\n");
            exit(-1);
        }else if(pid > 0){  //第一个子进程退出
            exit(0);
        }else if (pid == 0){    //第一个子进程创建的子进程
            sleep(2);
            printf("second child, parent pid = %d\n", getppid());
            exit(0);
        }
    }

    int status;
    if(waitpid(pid, &status, 0) != pid){    //回收创建的子进程
        printf("waitpid error\n");
        exit(-1);
    }else{
        printf("first child, pid = %d\n", pid);
        pr_exit(status);
        exit(0);
    }
}



void fpt_wait(){
    for(int index = 0; index < 5; index++){
        std::cout << "-->parent start to fork...." << std::endl;
        int pid = fork();
        if(pid == -1){  
            perror("fork error");
            exit(-1);
        }else if(pid == 0){ //子进程
            cout << "i am " << index << " child, my pid == " << getpid() << " my parentpid == " << getppid() << endl;
            sleep(3);
            exit(index);
        }
    }

    cout << "i am parent, my pid == " << getpid() << endl;
    while(true){
        int status;
        int handlePid = waitpid(-1, &status, 0);
        if(handlePid > 0){
            if(WIFEXITED(status)){
                cout << "child exit with status " << WEXITSTATUS(status) << endl;
            }else if (WIFSIGNALED(status)){
                cout << "child is killed by " << WTERMSIG(status) << endl;
            }else if(WIFSTOPPED(status)){
                cout << "child is stopped by " << WSTOPSIG(status) << endl;
            }
        }else if(handlePid == -1){
            std::cout << "parent complete...." << std::endl;
            break;
        }
    }
}






/*
 * 如何在一个进程中启动其它的程序？
 * 需要的参数：程序的名称(绝对路径、相对路径), 程序的参数(列表、数组)
 *
 * 可执行程序名（相对位置/绝对位置）, PATH环境变量, 整个环境变量environ
 * 参数形式：list vector
 * 环境变量：PATH environ
 *
 * int execl(const char *path, const char *arg, ...);
 * int execlp(const char *file, const char *arg, ...);
 * int execle(const char *path, const char *arg, ..., char *const envp[]);
 *
 * vector path environ
 * int execv(const char *path, char *const argv[]);
 * int execvp(const char *file, char *const argv[]);
 * int execve(const char *path, char *const argv[], char *const envp[]);
 *
 */
char* const lsArgument[] = { (char*)"ls", (char*)"-l", nullptr};
int fptExec(int option)
{
    pid_t pid = fork();     //通知内核创建一个新进程
    if(pid == 0){   //子进程执行单元
        cout << "i am child process, my pid == " << getpid() << endl;
        switch(option){
            case 0:
                execl("/usr/bin/pwd", "pwd", nullptr);  //绝对路径
                perror("error");
                break;
            case 1:
                execlp("ls", "ls", "-l", nullptr);  //相对路径      
                perror("error");
                break;
            case 2:
                execle("/usr/bin/ls", "ls", "-l", nullptr, environ);    //绝对路径 + 环境变量数组
                perror("error");
                break;
            case 3:
                execv("/bin/ls", lsArgument);
                perror("error");
                break;
            case 4:
                execvp("ls", lsArgument);
                perror("error");
                break;
            case 5:
                execve("/bin/ls", lsArgument, environ);
                perror("error");
                break;
            default:
                cout << "wrong option number" << endl;
        }

    }else if(pid > 0){  //父进程执行单元
        cout << "i am parent, pid = " << getpid() << endl;
        int status;
        int waitedPid = wait(&status);  //父进程回收子进程
        if(waitedPid > 0){
            if(WIFEXITED(status)){  //正常退出
                std::cout << "child is exit with status " << WEXITSTATUS(status) << std::endl;
            }else if(WIFSIGNALED(status)){  //接收到信号退出
                std::cout << "child is signaled by " << WTERMSIG(status) << std::endl;
            }else if(WIFSTOPPED(status)){   //接收到停止信号退出
                std::cout << "child is stopped by " << WSTOPSIG(status) << std::endl;
            }
        }
    }
    return 0;
}








int main(int argc, char* argv[]){
   system("ls;pwd > a.txt");

    return 0;
}







