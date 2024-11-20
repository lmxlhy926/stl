

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROCESS_TEST1 "/home/lhy/ownproject/stl/software/query_config_ble_synergy_sites/cmakeBuild/X86_64/unix/03process/process_test1"

/**
 * 进程：
 *      内核之上的抽象
 * 
 *      进程的内存结构：
 *          正文段
 *          数据段
 *          stack
 *          heap
 *          环境字符串、环境变量表；每个进程的环境变量是完全独立的，exec时指定环境变量表，然后复制到自己的进程地址空间中；
 */


/**
 * wait函数回收进程的几种状态：
 *      进程内调用return、exit导致的退出
 *      进程收到信号后导致的退出
 *      STOP信号导致的进程暂停
 *      CONTINUE信号导致
 */
void printWaitStatus(int pid, int status){
    if(WIFEXITED(status)){
        printf("<%d> exit status: %d\n", pid, WEXITSTATUS(status));
    }else if(WIFSIGNALED(status)){
        printf("<%d> exit by : %s\n", pid, strsignal(WTERMSIG(status)));
    }else if(WIFSTOPPED(status)){
        printf("<%d> stop by : %s\n", pid, strsignal(WSTOPSIG(status)));
    }else if(WIFCONTINUED(status)){
        printf("<%d> continued\n", pid);
    }
}

/**
 * 进程原语：
 *      fork:   通知内核，创建一个新进程；读时共享、写时复制。进程的地址空间是完全独立的；
 *      exec:   更新进程维护文件内容，加载新的程序
 *      exit:   退出进程
 *      wait:   回收进程；获取进程退出状态
 */
void process_example(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(-1);

    }else if(pid == 0){ // 子进程
        printf("child process: %d\n", getpid());
        char* environBuf[3] = {(char*)"hello=world", (char*)"name=valeue", nullptr};
        kill(getpid(), SIGSTOP);
        kill(getpid(), SIGKILL);
        execle(PROCESS_TEST1, "process_test1", nullptr, environBuf);
    }
    printf("parent process: %d\n", getpid());
    int status;
    while(true){
        pid_t pidWait = waitpid(-1, &status, WUNTRACED | WCONTINUED);
        if(pidWait > 0){
            printWaitStatus(pidWait, status);
            kill(pidWait, SIGCONT);

        }else if(pidWait == -1){
            printf("<%d> had no child\n", getpid());
            break;
        }
    }
}


int main(int argc, char* argv[])
{
   process_example();
    return 0;
}








