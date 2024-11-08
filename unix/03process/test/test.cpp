

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


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
 * 
 *      创建进程
 *          fork:
 *              通知内核，创建一个新进程；
 *              读时共享、写时复制。进程的地址空间是完全独立的；
 * 
 *      加载程序
 *          exec：
 *              更新进程维护文件内容，加载新的程序
 * 
 *      退出进程
 *          exit
 *              退出进程
 * 
 *      回收进程
 *          wait
 *              回收进程；
 *              获取进程退出状态
 */


void printExitStatus(int pid, int status){
    // 调用return、exit正常退出进程
    // 收到信号后导致的退出
    // 停止信号导致的进程暂停
    // 暂停的进程重新启动
    if(WIFEXITED(status)){
        printf("pid<%d> quit by exit, status %d\n", pid, WEXITSTATUS(status));

    }else if(WIFSIGNALED(status)){
        printf("pid<%d> quit by signal, signal %s\n", pid, strsignal(WTERMSIG(status)));

    }else if(WIFSTOPPED(status)){
        printf("pid<%d> stop by %s\n", pid, strsignal(WSTOPSIG(status)));

    }else if(WIFCONTINUED(status)){
        printf("pid<%d> continued\n", pid);
    }
}


void test1(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(-1);

    }else if(pid == 0){
        printf("this is child process <%d> ...\n", getpid());
        kill(getpid(), SIGSTOP);
        kill(getpid(), SIGKILL);
    }else if(pid > 0){
        printf("this is parent process <%d>...\n", getpid());
    }
   
   while(true){
        int status;
        pid_t wait_result_pid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
        if(wait_result_pid > 0){
            printExitStatus(wait_result_pid, status);

        }else if(wait_result_pid == -1){
            printf("no child need to be collected\n");
            break;
        }
        kill(pid, SIGCONT);
   }
}


void execl_test(){
    char* buf[3] = {(char*)"key=value", (char*)"key2=value2", nullptr};
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){
        execle("/home/lhy/ownproject/stl/unix/03process/test_process", "test_process", nullptr, buf);
    }
}


int main(int argc, char* argv[])
{
    
    return 0;
}








