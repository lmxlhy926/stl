

/**
 * 信号：
 *      内核之间的一种通信手段
 * 
 *      进程收到信号后，会执行默认的操作，如终止进程
 * 
 *      设置信号捕捉函数，在信号发生时执行；
 * 
 *      信号处理函数会打断正在执行的逻辑流，因此信号处理程序里必须调用可重入函数
 * 
 *      信号集合
 * 
 *      阻塞信号：记录信号是否发生，但是不执行信号对应的处理动作
 * 
 *      sigsuspend，sigwait：暂时恢复原来的屏蔽字，捕捉到信号后，恢复原来的屏蔽字；即一次捕捉一个信号
 * 
 *      多个信号同时发生时，由内核决定先执行哪个信号处理函数
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal_restart(int signo, sighandler_t sighandler)
{
    struct sigaction newact, oldact;
    newact.sa_handler = sighandler;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;

    if(signo == SIGALRM){
        newact.sa_flags |= SA_INTERRUPT;
    }else{
        newact.sa_flags |= SA_RESTART;
    }

    if(sigaction(signo, &newact, &oldact) == -1){
        return SIG_ERR;
    }

    return oldact.sa_handler;
}



void printSigno(int signo){
    printf("received signal: %s\n", strsignal(signo));
}


void sig_test()
{
    // 设置信号处理函数
    signal_restart(SIGQUIT, printSigno);
    signal_restart(SIGINT, printSigno);

    // 阻塞指定信号
    sigset_t sigBlockSet, sigOldSet;
    sigemptyset(&sigBlockSet);
    sigaddset(&sigBlockSet, SIGQUIT);
    sigaddset(&sigBlockSet, SIGINT);
    sigprocmask(SIG_BLOCK, &sigBlockSet, &sigOldSet);

    // 执行关键代码
    sleep(5);

    // 接收阻塞信号，执行信号处理函数
    sigsuspend(&sigOldSet);



    // 恢复信号屏蔽字
    printf("end of sig_test ....\n");
}




int main(int argc, char* argv[])
{
    sig_test();

    return 0;
}











