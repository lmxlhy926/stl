

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
 *      阻塞信号：记录信号是否发生，但是不执行信号对应的处理动作
 * 
 *      sigsuspend，sigwait：暂时恢复原来的屏蔽字，捕捉到信号后，恢复原来的屏蔽字；即一次捕捉一个信号
 * 
 *      多个信号同时发生时，由内核决定先执行哪个信号处理函数
 * 
 * 
 * 信号相关的常见原语：
 *      * 注册信号处理函数：signal、sigaction
 * 
 *      * 创建一个信号集合：sigset_t
 * 
 *      * 设置信号屏蔽字/获取信号屏蔽字: sigprocmask、pthread_sigmask
 * 
 *      * 执行关键函数：这里函数不会被阻塞的信号集合打断
 * 
 *      * 查询发生的阻塞信号集合：即该信号已经发生，但是被阻塞; sigpending
 * 
 *      * 调用sigsuspend、sigwait，处理信号
 * 
 *      * 像进程或线程发送信号; kill raise
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>

typedef void (*sighandler_t)(int signo);

static sighandler_t signal_restart(int signo, sighandler_t sighandler){
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


void sigtest(){
    if(signal_restart(SIGQUIT, printSigno) == SIG_ERR){
        perror("signal");
        exit(-1);
    }

    if(signal_restart(SIGINT, printSigno) == SIG_ERR){
        perror("signal");
        exit(-1);
    }

    sigset_t newsigset, oldsigset;
    sigemptyset(&newsigset);
    sigaddset(&newsigset, SIGQUIT);
    sigaddset(&newsigset, SIGINT);

    // 阻塞信号集合
    sigprocmask(SIG_BLOCK, &newsigset, &oldsigset);

    sleep(5);

    sigset_t pendingset;
    sigpending(&pendingset);
    if(sigismember(&pendingset, SIGQUIT)){
        printf("sigpending: sigquit\n");
    }
    if(sigismember(&pendingset, SIGINT)){
        printf("sigpending: sigint\n");
    }

    // sigsuspend(&oldsigset);
    int signo;
    sigwait(&newsigset, &signo);
    printf("after sigsuspend ....\n");

    sigpending(&pendingset);
    if(sigismember(&pendingset, SIGQUIT)){
        printf("sigpending: sigquit\n");
    }
    if(sigismember(&pendingset, SIGINT)){
        printf("sigpending: sigint\n");
    }


    sleep(10);
    sigprocmask(SIG_SETMASK, &oldsigset, nullptr);

    return;
}


int main(int argc, char* argv[])
{
    sigtest();

    return 0;
}







