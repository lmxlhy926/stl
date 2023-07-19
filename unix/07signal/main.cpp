
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

//信号处理函数，打印用户自定义信号
void sig_usr(int signo){
    if(signo == SIGUSR1){
        printf("pid = %d, received SIGUSR1\n", getpid());
    }else if(signo == SIGUSR2){
        printf("pid = %d, received SIGUSR2\n", getpid());
    }
}

/**
 * 为SIGUSR1注册信号处理函数
 * 为SIGUSR2注册信号处理函数
 * 循环调用pause，等待被信号唤醒
 * 让进程在后台运行，然后调用 kill -USR1 <PID>; kill -USR2 <PID>; kill <PID>
 * 
 * 当执行一个程序时，所有信号的状态都是系统默认或忽略。exec函数将原先设置为要捕捉的信号都更改为默认动作，
 * 其它信号状态则不变。一个进程原先要捕捉的信号，当其执行一个新程序后，就不能再捕捉了，因为捕捉函数的地址
 * 很可能在所执行的新程序文件中已无意义。
 * 
 * 当一个进程调用fork时，其子进程继承父进程的信号处理方式。因为子进程在开始时复制了父进程内存映像，所以
 * 信号捕捉函数的地址在子进程中是有意义的。
*/
void sigusr_test(){
    if(signal(SIGUSR1, sig_usr) == SIG_ERR){
        perror("can't catch SIGUSR1");
        exit(-1);
    }
    if(signal(SIGUSR2, sig_usr) == SIG_ERR){
        perror("can't catch SIGUSR2");
    }

    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);
    }

    for(; ;){
        pause();
    }
}


static void my_alarm(int signo){
    struct passwd* rootptr;
    printf("in signal handler\n");
    if((rootptr = getpwnam("root")) == nullptr){
        perror("getpwnam(root) error");
    }else{
        printf("getpwnam: %s\n", rootptr->pw_name);
    }
    alarm(1);
}

static void my_alarm_test(){
    signal(SIGALRM, my_alarm);
    alarm(1);
    while(true){
        sleep(10);
        printf("sleep wake up.....\n");
    }
    struct passwd* ptr;
    for(; ;){
        if((ptr = getpwnam("lhy")) == nullptr){
            perror("getpwnam error");
        }else{
            printf("getpwnam: %s\n", ptr->pw_name);
        }
    }
}


static void sig_alarm(int signo){
    return;
}

unsigned int sleep1(unsigned int seconds){
    if(signal(SIGALRM, sig_alarm) == SIG_ERR){
        return seconds;
    }
    alarm(seconds);
    pause();
    return(alarm(0));
}


void pr_mask(const char* str){
    sigset_t sigset;
    int errno_save = errno;

    //取得当前进程的信号屏蔽字
    if(sigprocmask(0, nullptr, &sigset) == -1){
        perror("sigprocmask error");
        exit(-1);
    }else{
        //打印该进程的屏蔽的部分信号
        printf("%s", str);
        if(sigismember(&sigset, SIGINT)){
            printf(" SIGINT");
        }
        if(sigismember(&sigset, SIGQUIT)){
            printf(" SIGQUIT");
        }
        if(sigismember(&sigset, SIGUSR1)){
            printf(" SIGUSR1");
        }
        if(sigismember(&sigset, SIGALRM)){
            printf(" SIGALRM");
        }
        printf("\n");
    }
}


void sig_quit(int signo){
    printf("caught SIGQUIT\n");
    //恢复SIGQUIT信号的handler为默认
    if(signal(SIGQUIT, SIG_DFL) == SIG_ERR){
        perror("can't reset SIGQUIT");
        exit(-1);
    }
}

void sigpending_test(){
    sigset_t newmask, oldmask, pendmask;

    //注册SIGQUIT信号处理函数
    //收到信号后，恢复信号的默认处理函数
    if(signal(SIGQUIT, sig_quit) == SIG_ERR){
        perror("cant't catch SIGQUIT");
        exit(-1);
    }

    //将SIGQUIT加入屏蔽信号集，并保存当前信号集合，用于后面恢复
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        perror("SIG_BLOCK error");
        exit(-1);
    }

    sleep(5);

    //取得当前产生但是被阻塞的信号，判断SIGQUIT是否被阻塞
    if(sigpending(&pendmask) < 0){
        perror("sigpending error");
        exit(-1);
    }
    if(sigismember(&pendmask, SIGQUIT)){
        printf("\nSIGQUIT pending\n");
    }

    //恢复当前进程的屏蔽信号集合 （解除SIGQUIT信号的阻塞）
    //如果有信号递送，则在sigprocmask返回前，至少执行其中一个信号处理函数
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        perror("SIG_SETMASK error");
        exit(-1);
    }
    printf("SIGQUIT unblocked\n");

    //此时不再阻塞SIGQUIT信号，默认操作是结束进程
    sleep(5);
    exit(0);
}


void sigQuitHandler(int signo){
    printf("sigHandler start....\n");
    sleep(5);
    printf("sigHandler end....\n");
}

void sigIntHandler(int signo){
    printf("sigIntHandler start....\n");
    printf("sigIntHandler end....\n");
}

using Sigfunc = void(int);
Sigfunc * signal_own(int signo, Sigfunc* func){
    //创建设置sigaction, 保存sigaction
    struct sigaction act, oact;

    //设置信号处理函数，执行信号处理函数时需要屏蔽的信号，以及信号可选标志(处理和信号关联的相关特性)
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);  //首先清空信号集，以完成初始化
    act.sa_flags = 0;
    if(signo == SIGALRM){
        #ifdef SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;   //不自动启动由该信号中断的系统调用
        #endif
    }else{
        act.sa_flags |= SA_RESTART; //自动启动由该信号中断的系统调用
    }

    //设置和指定信号相关联的处理动作
    if(sigaction(signo, &act, &oact) < 0){
        return SIG_ERR; //出错则返回SIG_ERR
    }   
    return oact.sa_handler; //否则返回之前设置的信号处理函数
}

void sigaction_test(){
    signal_own(SIGINT, sigIntHandler);
    
    struct sigaction act;
    act.sa_handler = sigQuitHandler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);    //执行SIGQUIT的信号处理函数时，屏蔽SIGINT信号
    act.sa_flags = 0;
    if(sigaction(SIGQUIT, &act, nullptr) < 0){
        perror("sigaction");
        exit(-1);
    }
    sleep(10);
}


int main(int argc, char* argv[]){
    sigaction_test();
    return 0;
}

