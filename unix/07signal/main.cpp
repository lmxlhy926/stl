
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

std::vector<int> sigvec{
        SIGINT,	
        SIGILL,	
        SIGABRT,	
        SIGFPE,	
        SIGSEGV,	
        SIGTERM,	
        SIGHUP,	
        SIGQUIT,	
        SIGTRAP,
        SIGKILL,	
        SIGSYS,	
        SIGPIPE,	
        SIGALRM,	
        SIGURG,	
        SIGSTOP,	
        SIGTSTP,	
        SIGCONT,	
        SIGCHLD,	
        SIGTTIN,	
        SIGTTOU,	
        SIGPOLL,	
        SIGXCPU,	
        SIGXFSZ,	
        SIGVTALRM,
        SIGPROF,	
        SIGUSR1,	
        SIGUSR2,	
        SIGWINCH,	
        SIGIO,		
        SIGIOT,	
        SIGCLD	
};


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
 * 当执行一个程序时，所有信号的状态都是系统默认或忽略。
 * 
 * exec族函数将原先设置为要捕捉的信号都更改为默认动作，其它信号状态则不变。
 * 一个进程原先要捕捉的信号，当其执行一个新程序后，就不能再捕捉了，因为捕捉函数的地址很可能在所执行的新程序文件中已无意义。
 * 
 * 当一个进程调用fork时，其子进程继承父进程的信号处理方式。
 * 因为子进程在开始时复制了父进程内存映像，所以信号捕捉函数的地址在子进程中是有意义的。
*/
void sigusr_test(){
    //设置信号处理函数
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

    //fork后，子进程继承父进程的信号处理方式
    for(; ;){
        std::cout << "start to pause, <" << getpid() << "> ..." << std::endl; 
        pause();
        std::cout << "end pause, <" << getpid() << "> ..." << std::endl; 
    }
}


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


//解析并打印回收的进程状态信息
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

    }else if(WIFCONTINUED(status)){   //进程恢复
        printf("child continued....\n");
    }
}


//打印当前进程的信号屏蔽字
int pr_mask(const char* str){
    //取得当前进程的信号屏蔽字集合
    sigset_t blockSet;
    if(sigprocmask(SIG_BLOCK, nullptr, &blockSet) == -1){
        perror("sigprocmask error");
        exit(-1);
    }

    //打印该进程的屏蔽的部分信号
    printf("%s", str);
    for(auto& signo : sigvec){
        if(sigismember(&blockSet, signo)){
            printf(" %s", strsignal(signo));
        }
    }
    printf("\n");
    return 0;
}


void sig_quit(int signo){
    printf("caught SIGQUIT\n");
    //恢复SIGQUIT信号的handler为默认
    if(signal(SIGQUIT, SIG_DFL) == SIG_ERR){
        perror("can't reset SIGQUIT");
        exit(-1);
    }
}


/**
 *  sigpending：返回当前进程的阻塞信号集合
*/
void sigpending_test(){
    std::cout << "pid: " << getpid() << std::endl;

    //注册SIGQUIT信号处理函数
    if(signal(SIGQUIT, sig_quit) == SIG_ERR){
        perror("cant't catch SIGQUIT");
        exit(-1);
    }

    //将SIGQUIT加入屏蔽信号集，并保存当前信号集合，用于后面恢复
    sigset_t newmask, oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        perror("SIG_BLOCK error");
        exit(-1);
    }

    std::cout << "now start to signal a SIGQUIT ..." << std::endl;
    sleep(10);

    //取得当前产生但是被阻塞的信号，判断SIGQUIT是否被阻塞
    sigset_t pendmask;
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
    std::cout << "now start to signal a SIGQUIT ..." << std::endl;
    sleep(10);
    exit(0);
}


/**
 * 由sigaction实现的signal:
 *      * 出错返回SIG_ERR，否则返回之前设置的信号处理函数
 *      * 由该信号中断的系统调用，默认重启
*/
using Sigfunc = void(int);
Sigfunc * signal_own(int signo, Sigfunc* func){
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);  //首先清空信号集，以完成初始化
    act.sa_flags = 0;
    if(signo == SIGALRM){
        #ifdef SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;   //不自动启动由SIGALRM信号中断的系统调用
        #endif

    }else{
        act.sa_flags |= SA_RESTART; //自动启动由该信号中断的系统调用
    }

    if(sigaction(signo, &act, &oact) < 0){
        return SIG_ERR; //出错则返回SIG_ERR
    }   

    return oact.sa_handler; //否则返回之前设置的信号处理函数
}


void sigQuitHandler(int signo){
    printf("sigQuitHandler start....\n");
    sleep(5);
    sleep(2);
    printf("sigQuitHandler end....\n");
}

void sigIntHandler(int signo){
    printf("sigIntHandler start....\n");
    printf("sigIntHandler end....\n");
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


static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;   //sig_atomic_t：整型，不会被信号处理程序打断

static void sig_alrm(int){
    //打印信号处理屏蔽字
    pr_mask("in sig_alrm: ");
}

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


static void sig_init(int signo){
    pr_mask("\nin sig_init: ");
}

void sigsuspend_test(){
    sigset_t newmask, oldmask, waitmask;
    pr_mask("program start: ");
    if(signal_own(SIGINT, sig_init) == SIG_ERR){
        perror("signal(SIGINT) error");
        exit(-1);
    }
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);

    //block SIGINT and save current signal mask
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        perror("sigprocmask error");
        exit(-1);
    }
    pr_mask("in critial region: ");

    //pause, allowing all signals except SIGUSR1.
    //在一个原子操作中设置信号屏蔽字，然后使进程休眠
    //在捕捉到一个信号或发生了一个会终止该进程的信号之前，该进程被挂起。
    //如果捕捉到一个信号而且从该信号处理程序返回，则sigsuspend返回，并且该进程的信号屏蔽字设置为调用sigsuspend之前的值。
    if(sigsuspend(&waitmask) != -1){
        perror("sigsuspend error");
        exit(-1);
    }
    pr_mask("aftern return from sigsuspend: ");

    //reset signal block which unblocks SIGINT
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        perror("sigprocmask error");
        exit(-1);
    }

    pr_mask("program exit: ");
    exit(0);
}


static volatile sig_atomic_t quitflag;

static void sig_init1(int signo){
    pr_mask("in sig_init1: ");
    if(signo == SIGINT)
        printf("\ninterrupt\n");
    else if(signo == SIGQUIT)
        quitflag = 1;
}

void sigsuspend_test1(){
    sigset_t newmask, oldmask, zeromask;
    //设置SIGINT、SIGQUIT的信号处理函数
    if(signal_own(SIGINT, sig_init1) == SIG_ERR){
        perror("signal(SIGINT) error");
        exit(-1);
    }
    if(signal_own(SIGQUIT, sig_init1) == SIG_ERR){
        perror("signal(SIGQUIT) error");
        exit(-1);
    }

    //构造信号集合
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    pr_mask("main starting: ");
    //阻塞SIGQUIT信号
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        perror("sigprocmask error");
        exit(-1);
    }
    pr_mask("after SIG_BLOCK: ");

    while(quitflag == 0){
        //在一个原子操作中完成：1. 设置新的信号屏蔽字； 2 阻塞等待信号发声
        //被唤醒号，恢复进程的信号屏蔽字
        sigsuspend(&zeromask);
        pr_mask("after sigsuspend: ");
    }
        

    quitflag = 0;

    //恢复信号
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        perror("SIG_SETMASK error");
        exit(-1);
    }
    pr_mask("main ending: ");

    exit(0);
}

/**
 * ISO C要求若捕捉到此信号而且相应信号处理程序返回，abort仍不会返回到其调用者。
 * 
 * POSIX.1也说明abort并不理会进程对此信号的阻塞和忽略。
 * 
 * 让进程捕捉SIGABRT的意图是：在进程终止之前由其执行所需的清理操作。如果进程并不在信号处理程序中终止自己，POSIX.1声明
 * 当信号处理程序返回时，abort终止该进程。
 * 
 * 
*/
void abort_own(){
    struct sigaction action;

    //获取之前的设置,，如果SIGABRT被阻塞，则恢复为默认
    sigaction(SIGABRT, nullptr, &action);
    if(action.sa_handler == SIG_IGN){
        action.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &action, nullptr);
    }

    //如果SIGABRT是默认操作，则刷新标准库缓冲区
    if(action.sa_handler == SIG_DFL){
        fflush(nullptr);
    }

    //阻塞除SIGABRT之外的所有信号
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGABRT);
    sigprocmask(SIG_SETMASK, &mask, nullptr);
    
    //向调用进程自身发送信号
    /**
     * 如果调用kill使其为调用者产生信号，并且如果该信号是不被阻塞的，则在Kill返回前该信号(或某个未决、未阻塞的信号)
     * 就被传送给了该进程。我们阻塞了SIGABRT外的所有信号，这样就可知如果对Kill的调用返回了，则该进程一定已捕捉到该信号，
     * 并且也从该信号处理程序返回。
    */
    kill(getpid(), SIGABRT);
    printf("kill here....\n");

    //对于捕获SIGABRT的会返回的handler，控制流会到达这里
    //设置SIGABRT信号handler为默认，再次发送，终止进程
    fflush(nullptr);
    action.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &action, nullptr);
    sigprocmask(SIGABRT, &mask, nullptr);
    kill(getpid(), SIGABRT);

    exit(1);
}

void abortHandler(int signo){
    printf("catch SIGABRT...\n");
}

void abort_test(){
    signal_own(SIGABRT, abortHandler);
    abort_own();
}



int system_nosignal(const char *cmdstring){
    pid_t pid;
    int status = -1;

    if(cmdstring == nullptr){
        return 1;
    }

    if((pid = fork()) < 0){
        status = -1;
    }else if(pid == 0){
        execl("/usr/bin/bash", "sh", "-c", cmdstring, nullptr);
        _exit(127);
    }else{
        while(waitpid(pid, &status, 0) < 0){
            if(errno != EINTR){
                status = -1;
                break;
            }
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











