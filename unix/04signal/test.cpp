

/**
 * 信号：
 *      进程之间通过内核进行的短消息通信
 * 
 *      进程包含主线程和其它线程；每个线程都有自己的信号屏蔽字；信号对应的操作是固定的；     
 * 
 *      不进行设置的情况下，进程收到信号后，会执行默认的操作，如终止进程
 * 
 *      设置信号捕捉函数，在信号发生时执行；
 * 
 *      信号处理函数会打断正在执行的逻辑流，因此信号处理程序里必须调用可重入函数
 * 
 *      阻塞信号：记录信号是否发生，但是不执行信号对应的处理动作
 * 
 *      处理信号的2个函数：
 *              sigsuspend：设置为新的信号屏蔽字并阻塞等待，阻塞集合里有信号，执行信号处理函数后，恢复为原先的信号屏蔽字
 *              sigwait：如果阻塞集合里有指定的信号，则返回，并清除阻塞集合里的相应信号；
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
 *      * 像进程或线程发送信号; kill raise pthread_kill
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>
#include <pthread.h>


#if 0

typedef void (*sighandler_t)(int signo);


/**
 * struct sigaction的使用：
 *      * 信号回调函数
 *      * 执行信号回调函数时需要额外阻塞的信号集合
 *      * 和信号执行相关的其它属性
 * 
 * sigaction的使用：
 *      * 设置新的信号处理机制，返回旧的信号处理机制
 */
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


/**
 * 信号处理函数：
 *      * 如果使用sigsuspend，则需要信号处理函数是可冲入的
 *      * 使用sigwait，信号处理函数不需要是可重入的
 */
void printSigno(int signo){
    printf("received signal: %s\n", strsignal(signo));
}


/**
 * 信号处理的标准例程：
 *      * 设置信号处理机制
 *      * 设置信号屏蔽字
 *      * 执行关键代码
 *      * 处理已经发生的信号
 *      * 恢复信号屏蔽字
 * 
 *  由于使用的是sigsuspend，所以要求信号对应的信号处理函数是可重入的。
 */
void sigtest_sigsuspend(){
    /**
     * 首先对要处理的信号设置信号处理机制
     */
    if(signal_restart(SIGQUIT, printSigno) == SIG_ERR){
        perror("signal");
        exit(-1);
    }

    if(signal_restart(SIGINT, printSigno) == SIG_ERR){
        perror("signal");
        exit(-1);
    }

    // 构建信号集合
    sigset_t newsigset, oldsigset;
    sigemptyset(&newsigset);
    sigaddset(&newsigset, SIGQUIT);
    sigaddset(&newsigset, SIGINT);

    // 设置信号屏蔽字，阻塞指定的信号集合
    sigprocmask(SIG_BLOCK, &newsigset, &oldsigset);

    // 执行关键处理代码，在这期间捕捉到的信号会被阻塞，不会干扰代码的执行
    sleep(5);

    // 查看挂起的信号集合：指的是已发生但是被阻塞的信号
    sigset_t pendingset;
    sigpending(&pendingset);
    if(sigismember(&pendingset, SIGQUIT)){
        printf("sigpending: sigquit\n");
    }
    if(sigismember(&pendingset, SIGINT)){
        printf("sigpending: sigint\n");
    }

    /**
     * sigsuspend:
     *      * 暂时设置新的信号屏蔽字并休眠，这二者是一个原子操作
     *      * 触发进程终止或者信号处理函数调用的信号，会触发sigsuspend返回
     *      * 先执行信号处理函数，后sigsuspend返回，并且恢复原来的信号屏蔽字
     */
    sigsuspend(&oldsigset);
    printf("after sigsuspend ....\n");

    /**
     * 信号相关的处理动作执行后，信号会从发生集合中去除
     */
    sigpending(&pendingset);
    if(sigismember(&pendingset, SIGQUIT)){
        printf("sigpending: sigquit\n");
    }
    if(sigismember(&pendingset, SIGINT)){
        printf("sigpending: sigint\n");
    }

    sleep(5);
    // 恢复信号屏蔽字
    sigprocmask(SIG_SETMASK, &oldsigset, nullptr);

    return;
}

static int quitflag = 0;
sigset_t newset;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


/**
 * 独立的线程：捕捉信号并处理
 *      * 线程阻塞，直到有等待的信号发生
 *      * 处理发生的信号
 */
void* sighandle_thread(void* arg){
    while(true){
        /**
         * 如果有信号在阻塞集合中，则sigwait返回，并且从阻塞集合中清除该信号；
         */
        int signo;
        if(sigwait(&newset, &signo) != 0){
            perror("sigwait");
            exit(-1);
        }

        /**
         * 处理捕捉到的信号
         */
        switch(signo){
            case SIGINT:
                printf("received signal: %s\n", strsignal(SIGINT));
                break;
            case SIGQUIT:
                printf("received signal: %s\n", strsignal(SIGQUIT));
                pthread_mutex_lock(&lock);
                quitflag = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&cond);
                break;
            default:
                printf("unexpected signal: %s\n", strsignal(signo));
        }
    }
}


/**
 * 信号处理例程
 *      * 使用sigwait，在一个专门的线程里，处理所有信号；
 */
void sigtest_sigwait(){
    printf("process_id : %d\n", getpid());

    // 设置主线程的信号屏蔽字
    sigset_t oldset;
    sigfillset(&newset);
    if(pthread_sigmask(SIG_BLOCK, &newset, &oldset) != 0){
        perror("pthread_sigmask");
        exit(-1);
    }

    // 创建线程：被创建线程会继承创建它的线程的信号屏蔽字
    pthread_t tid;
    pthread_create(&tid, nullptr, sighandle_thread, nullptr);

    sleep(5);

    // 阻塞等待
    pthread_mutex_lock(&lock);
    while(quitflag == 0){
        pthread_cond_wait(&cond, &lock);
    }
    printf("quitflag: %d\n", quitflag);
    pthread_mutex_unlock(&lock);

    // 恢复信号屏蔽字
    if(pthread_sigmask(SIG_SETMASK, &oldset, nullptr) != 0){
        perror("pthread_sigmask");
        exit(-1);
    }

    exit(0);
}

#endif


typedef void (*sighandler_t)(int signo);

sighandler_t signal_restart(int signo, sighandler_t sighandler){
    struct sigaction newaction, oldaction;
    newaction.sa_handler = sighandler;
    sigemptyset(&newaction.sa_mask);
    newaction.sa_flags = 0;

    if(signo == SIGALRM){
        newaction.sa_flags |= SA_INTERRUPT;
    }else{
        newaction.sa_flags |= SA_RESTART;
    }

    if(sigaction(signo, &newaction, &oldaction) == -1){
        return SIG_ERR;
    }

    return oldaction.sa_handler;
}

void signal_handler(int signo){
    printf("received signal: %s\n", strsignal(signo));
}


void signal_test(){
    if(signal_restart(SIGINT, signal_handler) == SIG_ERR){
        perror("signal_restart");
        exit(-1);
    }
    if(signal_restart(SIGQUIT, signal_handler) == SIG_ERR){
        perror("signal_restart");
        exit(-1);
    }

    sigset_t newset, oldset;
    sigemptyset(&newset);
    sigaddset(&newset, SIGINT);
    sigaddset(&newset, SIGQUIT);
    
    if(pthread_sigmask(SIG_BLOCK, &newset, &oldset) != 0){
        perror("pthread_sigmask");
        exit(-1);
    }

    sleep(5);

    sigset_t pengdingset;
    sigpending(&pengdingset);
    if(sigismember(&pengdingset, SIGINT)){
        printf("pendingset: %s\n", strsignal(SIGINT));
    }
    if(sigismember(&pengdingset, SIGQUIT)){
        printf("pendingset: %s\n", strsignal(SIGQUIT));
    }

    printf("start sigsuspend ....\n");
    sigsuspend(&oldset);
    printf("after sigsuspend ....\n");

    pthread_sigmask(SIG_SETMASK, &oldset, nullptr);

    return;
}


int main(int argc, char* argv[])
{
    signal_test();

    return 0;
}







