
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/wait.h>

using namespace std;



static void my_alarm(int signo){
    printf("in signal handler\n");
    //获取登录名对应的账号的信息
    struct passwd* rootptr = getpwnam("lhy");
    if(rootptr == nullptr){
        perror("getpwnam(root) error");
    }else{
        printf("username: %s\n", rootptr->pw_name);
        printf("passwd: %s\n", rootptr->pw_passwd);
        printf("uid: %u\n", rootptr->pw_uid);
        printf("gid: %u\n", rootptr->pw_gid);
        printf("home directory: %s\n", rootptr->pw_dir);
        printf("shell program: %s\n", rootptr->pw_shell);
    }
    alarm(3);   //在指定的时间内产生一个闹钟信号
}


static void my_alarm_test(){
    //设置SIGALRM信号处理函数
    if(signal(SIGALRM, my_alarm) == SIG_ERR){   //捕捉闹钟信号
        perror("signal SIGALRM...");
        exit(-1);
    }
    //设置闹铃，一秒钟产生SIGALRM信号
    alarm(3);

    while(true){
        sleep(10);  //等待被信号处理中断
        printf("sleep wake up.....\n");
        std::cout << std::endl;
    }
}


static void sig_alarm(int signo){
    std::cout << "in sig_alrm ... " << std::endl;
    return;
}

unsigned int sleep_nosignal_test(){
    uint seconds = 10;
    std::cout << "pid: " << getpid() << std::endl;
    sleep(5);
    std::cout << "-------" << std::endl;
    if(signal(SIGALRM, sig_alarm) == SIG_ERR){
        return seconds;
    }
    alarm(seconds);
    //-----------------------------------//
    pause();     //等待闹铃信号
    std::cout << "alarm(0): " << alarm(0) << std::endl;
    return 0;
}




/**
 * 用alarm实现sleep
 *      * 阻塞SIGALRM，设定闹钟
 *      * 用sigsuspend进行等待
 *      * 恢复等待之前的进程的屏蔽信号集合和信号处理函数
 *      * 返回剩余的等待时间
*/
unsigned int sleep_signal(unsigned int seconds){
    //设置捕获到SIGALRM信号时的信号处理动作
    struct sigaction newact, oldact;
    newact.sa_handler = sig_alarm;  //信号处理函数
    sigemptyset(&newact.sa_mask);   //执行信号处理函数时，要额外屏蔽的信号集合
    newact.sa_flags = 0;            //设定信号相关特性
    sigaction(SIGALRM, &newact, &oldact);   //设置信号处理函数，并返回之前的信号处理函数

    //阻塞SIGALRM，记录此时的进程信号屏蔽集合
    sigset_t newmask, oldmask, suspmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask); //设置进程屏蔽信号集合

    //设置定时闹铃
    alarm(seconds);

    //阻塞等待信号发生，可以为之前系统未屏蔽的任何信号以及SIGALRM信号中断
    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM);
    sigsuspend(&suspmask);  //设置屏蔽信号集合并阻塞在这里，返回时恢复原屏蔽信号集合

    /**
     * 返回到这里意味着已经捕捉到某个信号。sigsuspend会恢复之前的进程屏蔽信号集合，SIGALRM现在被阻塞。
    */

    int unslept = alarm(0); //返回剩余的时间

    //恢复SIGALRM的信号处理程序，恢复之前的进程信号屏蔽字
    sigaction(SIGALRM, &oldact, nullptr);
    sigprocmask(SIG_SETMASK, &oldmask, nullptr);

    return unslept;
}


/**
 * kill -SIGUSR1 PID， 中断进程运行，测试sleep时间
*/
void sleep_test(){
    struct sigaction newaction, oldaction;
    newaction.sa_handler = sig_usr;
    sigemptyset(&newaction.sa_mask);
    newaction.sa_flags = 0;
    sigaction(SIGUSR1, &newaction, &oldaction);

    std::cout << "processid: " << getpid() << std::endl;
    int start = time(nullptr);
    int unslept = sleep_signal(20);
    int end = time(nullptr);

    std::cout << "unslept: " << unslept << " , end - start: " << end - start << std::endl;
}



void nanosleep_test(){
    struct sigaction newaction, oldaction;
    newaction.sa_handler = sig_usr;
    sigemptyset(&newaction.sa_mask);
    newaction.sa_flags = 0;
    sigaction(SIGUSR1, &newaction, &oldaction);
    std::cout << "pid: " << getpid() << std::endl;

    struct timespec sleeptime, unsleeptime{};
    sleeptime.tv_sec = 15;  //秒
    sleeptime.tv_nsec = 1000 * 1000 * 1000;  //纳秒
    nanosleep(&sleeptime, &unsleeptime);
    std::cout << "unsleeptime.tv_sec: " << unsleeptime.tv_sec
              << " , unsleeptime.tv_nsec: " << unsleeptime.tv_nsec << std::endl;
}


static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;   //sig_atomic_t：整型，不会被信号处理程序打断

static void sig_usr1(int signo){
    //主程序调用sigsetjmp之前，不响应信号；
    if(canjump == 0){
        return;
    }
    //打印进程信号屏蔽字
    pr_mask("starting sig_usr1: ");
    //给进程设置闹钟
    alarm(3);
    //工作5秒，这期间会触发闹钟信号处理函数
    time_t starttime = time(nullptr);
    for(; ;){
        if(time(nullptr) > starttime + 5)
            break;
    }
    //打印信号处理屏蔽字
    pr_mask("finishing sig_usr1: ");
    canjump = 0;
    //跳转到sigsetjmp处
    siglongjmp(jmpbuf, 1);
}


void sigsetjmp_siglongjump(){
    //设置SIGUSR1信号处理程序
    if(signal_own(SIGUSR1, sig_usr1) == SIG_ERR){
        perror("signal(SIGUSR1) error");
        exit(-1);
    }
    //设置SIGALRM信号处理程序
    if(signal_own(SIGALRM, sig_alrm) == SIG_ERR){
        perror("signal(SIGALRM) error");
        exit(-1);
    }

    //打印进程此时的信号屏蔽字
    pr_mask("starting main: ");

    //保存进程当前状态
    //如果savemask非0，则sigsetjmp在env中保存进程的当前信号屏蔽字
    //如果带非0savemask的sigsetjmp调用已经保存了env，则siglongjmp从其中恢复保存的信号屏蔽字
    if(sigsetjmp(jmpbuf, 1)){
        pr_mask("ending main: ");
        exit(0);
    }

    canjump = 1;
    for(; ;){
        pause();
    }
}







int system_nosignal(const char *cmdstring){
    if(cmdstring == nullptr){
        return 1;
    }
    pid_t pid;
    int status = -1;
    if((pid = fork()) < 0){
        status = -1;
    }else if(pid == 0){
        execl("/usr/bin/bash", "sh", "-c", cmdstring, nullptr);
        _exit(127);
    }
    while(waitpid(pid, &status, 0) < 0){
        if(errno != EINTR){
            status = -1;
            break;
        }
    }
    return status;
}


int system_signal(const char* cmdstring){
    if(cmdstring == nullptr){   //unix总是支持命令行处理
        return 1;
    }

    struct sigaction ignore, saveintr, savequit;
    sigset_t chldmask, savemask;
    //忽略SIGINT,SIGQUIT
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    if(sigaction(SIGINT, &ignore, &saveintr) < 0){
        return -1;
    }
    if(sigaction(SIGQUIT, &ignore, &savequit) < 0){
        return -1;
    }

    //阻塞SIGCHLD
    sigemptyset(&chldmask);
    sigaddset(&chldmask, SIGCHLD);
    if(sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0){
        return -1;
    }

    pid_t pid;
    int status;
    if((pid = fork()) < 0){
        status = -1;
    }else if(pid == 0){
        //恢复信号的处理方式
        sigaction(SIGINT, &saveintr, nullptr);
        sigaction(SIGQUIT, &savequit, nullptr);
        //恢复之前的阻塞信合集合
        sigprocmask(SIG_SETMASK, &savemask, nullptr);
        execl("/usr/bin/bash", "bash", "-c", cmdstring, nullptr);
        _exit(127);

    }else{
        //父进程等待回收子进程
        while(waitpid(pid, &status, 0) < 0){
            if(errno != EINTR){
                status = -1;
                break;
            }
        }
    }

    //父进程恢复信号处理方式
    if(sigaction(SIGINT, &saveintr, nullptr) < 0)
        return -1;
    if(sigaction(SIGQUIT, &savequit, nullptr) < 0)
        return -1;
    //父进程恢复进程屏蔽信号集合
    if(sigprocmask(SIG_SETMASK, &savemask, nullptr) < 0)
        return -1;

    return status;
}



static void sig_init_1(int signo){
    printf("caught SIGINT\n");
}

static void sig_chld_1(int signo){
    printf("caught SIGCHLD\n");
    int status;
    if(wait(&status) < 0){
        perror("wait");
    }
}

void system_nosignal_test(){
    if(signal_own(SIGINT, sig_init_1) == SIG_ERR){
        perror("signal(SIGINT) error");
        exit(-1);
    }
    if(signal_own(SIGCHLD, sig_chld_1) == SIG_ERR){
        perror("signal(SIGCHLD) error");
        exit(-1);
    }

    int status;
    if((status = system_signal("sleep 30")) < 0){
        perror("system error");
        exit(-1);
    }

    pr_exit(status);
    printf("---end---\n");
    exit(0);
}

void sig_tstp(int signo){
    //处理逻辑
    std::cout << "in sig_tstp" << std::endl;

    //进入SIGTSTP的信号处理程序后，自动屏蔽SIGTSTP信号。这里解除对SIGTSTP信号的屏蔽。
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, nullptr);

    //恢复SIGTSTP信号的默认处理方式：停止该进程
    signal(SIGTSTP, SIG_DFL);
    //向自己发送SIGTSTP信号来停止该进程
    kill(getpid(), SIGTSTP);

    /**
     * 仅当某个进程向该进程发送一个SIGCONT信号时，该进程才继续。我们不捕捉SIGCONT信号。该信号的默认配置
     * 是继续运行停止的进程，当此发生时，此程序如同从kill函数返回一样继续运行。
    */


    //恢复SIGTSTP信号处理程序
    signal(SIGTSTP, sig_tstp);
    std::cout << "sig_tstp end...." << std::endl;
}

void sigtstop_test(){
    /**
     * 仅当SIGTSTP信号的配置是SIG_DFL才安排捕捉该信号，其理由是：当此程序由不支持作业控制的shell启动时，
     * 此信号的配置应当设置为SIG_IGN。只有作业控制shell才应将这3个信号重新设置为SIG_DFL。
    */
    if(signal(SIGTSTP, SIG_IGN) == SIG_DFL){
        signal(SIGTSTP, sig_tstp);
    }

    while(true){
        std::cout << "hello world" << std::endl;
        sleep(1);
    }
    
    exit(0);
}


void signal_print_test(){
    psignal(SIGABRT, "print");
    char* ptr = strsignal(SIGABRT);
    printf("strsignal: %s\n", ptr);
}


void test(int a){
    std::cout << "---" << a << std::endl;
}
using funcType = void(int);

int main(int argc, char* argv[]){
    funcType *ft = test;
    


    

    return 0;
}











