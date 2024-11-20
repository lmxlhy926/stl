
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void sig_handler(int signo){
    printf("pid: %d, %s\n", getpid(), strsignal(signo));
}

void test(){
    // 注册信号处理函数
    if(signal(SIGQUIT, sig_handler) == SIG_ERR){
        perror("signal");
        exit(-1);
    }

    // 阻塞信号
    sigset_t newset, oldset;
    sigemptyset(&newset);
    sigaddset(&newset, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newset, &oldset) == -1){
        perror("sigprocmask");
        exit(-1);
    }

    printf("wait 1 ...\n");
    sleep(5);

    // 查看阻塞信号
    sigset_t pendingset;
    if(sigpending(&pendingset) == 0){
        if(sigismember(&pendingset, SIGQUIT)){
            printf("sigquit has been caught ...\n");
        }
    }

    printf("---------\n");

    // 暂时设置新信号屏蔽字
    sigsuspend(&oldset);

    printf("wait 2 ...\n");
    sleep(5);

    // 恢复信号屏蔽字
    sigprocmask(SIG_SETMASK, &oldset, nullptr);
}


int quitflag1 = 0;
sigset_t mask;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void* thr_fn(void *arg){
    int err, signo;
    for(; ;){
        //-------调用sigwait之前信号要先被阻塞，否则这里有时间窗口-------
        err = sigwait(&mask, &signo);   //线程阻塞等待信号，直到被信号唤醒。
        if(err != 0){
            printf("sigwait error: %s\n", strerror(err));
            exit(-1);
        }
       
        //线程结束阻塞，开始执行后续逻辑
        switch(signo){
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGTSTP:
                printf("\nsigtstp\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&lock);  //线程中的共享数据需要加锁保护
                quitflag1 = 1;
                printf("quitflag == 1\n");
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&waitloc);
                return(0);
            default:
                printf("unexcepted signal %s\n", strsignal(signo));
                break;
        }
    }
}


/**
 * 线程的阻塞信号和进程的阻塞信号：
 *      进程收到一个信号后，会随机发送给任意一个线程。因此sigpending获取到的线程的未决信号集合,
 * 包含明确发送给该线程但被阻塞的信号以及发送给该进程但被阻塞的信号。
 *      sigpending获取到的是线程的未决信号集合。
*/
void sig_multiThread_process(){
    sigset_t oldmask;

    sigfillset(&mask);
    if(pthread_sigmask(SIG_BLOCK, &mask, &oldmask) != 0){   //设置主线程信号屏蔽字
        perror("pthread_sigmask");
        exit(-1);
    }

    //新创建的线程继承调用线程的信号屏蔽字
    pthread_t tid;
    int err = pthread_create(&tid, nullptr, thr_fn, nullptr);
    if(err != 0){
        printf("pthread_create error: %s\n", strerror(err));
        exit(-1);
    }

    printf("wait for 5 seconds....\n");
    sleep(5);
   
    pthread_mutex_lock(&lock);
    while(quitflag1 == 0){
        pthread_cond_wait(&waitloc, &lock); //线程等待被唤醒，满足条件则执行后续逻辑，否则继续陷入等待
    }
    pthread_mutex_unlock(&lock);
   
    //恢复线程信号屏蔽字
    // if(pthread_sigmask(SIG_SETMASK, &oldmask, nullptr) != 0){
    //     perror("pthread_sigmask");
    //     exit(-1);
    // }

    exit(0);
}


int main(int argc, char* argv[])
{
    sig_multiThread_process();

    return 0;
}






















