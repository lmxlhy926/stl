
/**
 * 信号提供了一种处理异步事件的方法。POSIX.1对可靠信号例程进行了标准化。
 * 很多条件都可以产生信号
 *      * 当用户按某些终端键时，引发终端产生信号。
 *      * 硬件异常产生信号。除数为0、无效内存引用等。这些条件通常由硬件检测到，并通知内核。然后内核为该条件产生时正在运行的
 *        进程产生适当的信号。
 *      * 当检测到某种软件条件已经发生，并应将其通知有关进程时也产生信号。如SIGURG、SIGPIPE、SIGALRM。
 *      * 进程调用kill函数可将任意信号发送给另一个进程或进程组。
 *      * 用户可用Kill命令将信号发送给其它进程。
 * 
 * 信号是异步事件的经典实例。产生信号的事件对进程而言是随机出现的。在某个信号出现时，可以告诉内核按下列3种方式之一进行处理。
 *      （1）忽略此信号。SIGKILL和SIGSTOP决不能被忽略，它们向内核和超级用户提供了使进程终止或停止的可靠方法。
 *       (2) 捕捉信号。要通知内核在某种信号发生时，调用一个用户函数。
 *       (3) 执行系统默认动作。对大多数信号的系统默认动作时终止该进程。
 * 
 * 
 * 不可靠信号
 *      * 不可靠在这里指的是，信号可能会丢失；即一个信号发生了，但进程却可能一直不知道这一点。
 *      * 同时进程对信号的控制能力很差，它只能捕捉信号或忽略它，但是不能阻塞信号。
 *      * 早期版本的一个问题：在进程每次接收到信号并对其进行处理时，随即将该信号动作重置为默认值。会导致时间窗口出现。
 *      * 早期版本的另一个问题：在进程不希望某种信号发生时，它不能关闭该信号。进程能做的一切就是忽略该信号。
 * 
 * 
 * 中断的系统调用
 *      早期UNIX系统的一个特性是：如果进程在执行一个低速系统调用而阻塞期间捕捉到一个信号，则该系统调用就被中断
 * 不再继续执行。该系统调用返回出错，其errno设置为EINTR。在这里必须区分系统调用和函数。当捕捉到某个信号时
 * 被中断的是内核中执行的系统调用。
 *      低速系统调用是可能会使进程永远阻塞的一类系统调用。
 *      为了帮助应用程序使其不必处理被中断的系统调用，4.2BSD引进了某些被中断系统调用的自动重启动。
 * 
 * 
 * 可重入函数
 *      进程捕捉到信号并对其进行处理时，进程正在执行的正常指令序列就被信号处理程序临时中断，它首先执行该信号
 * 处理程序中的指令。如果从信号处理程序返回，则继续执行在捕捉到信号时进程正在执行的正常指令序列。但在信号处理
 * 程序中，不能判断捕捉到信号时进程执行到何处。Single UNIX Specification说明了在信号处理程序中保证调用安全的
 * 函数。这些函数是可重入的并被称为是异步信号安全的。除了可重入以外，在信号处理操作期间，它会阻塞任何会引起不
 * 一致的信号发送。
 * 
 * 不可重入函数的特点：
 *      (1) 已知它们使用静态数据结构
 *      (2) 它们调用malloc或free。
 *      (3) 它们是标准I/O函数。标准I/O库的很多实现都以不可重入方式使用全局数据结构。
 * 
 * 
 * 可靠信号术语和语义
 *      当造成信号的事件发生时，为进程产生一个信号(或向一个进程发送一个信号)。当一个信号产生时，内核通常在进程表
 * 中以某种形式设置一个标志。当对信号采取了这种动作时，我们说向进程递送了一个信号。在信号产生和递送之间的时间间隔
 * 内，称信号是未决的。进程可以选用"阻塞信号递送"，如果为进程产生了一个阻塞的信号，而且对该信号的动作是系统默认动作
 * 或捕捉该信号，则为该进程将此信号保持为未决状态，直到该进程为此信号解除了阻塞，或者将此信号的动作更改为忽略。进程
 * 调用sigpending函数来判定哪些信号是设置为阻塞并处于未决状态的。
 * 
 * 
*/


/**
 * 信号相关的处理函数概述：
 * 
 * kill和raise
 *      kill函数将信号发送给进程或进程组。raise函数则允许进程向自身发送信号。
 * 
 * 
 * sigset_t
 *      POSIX.1定义数据类型sigset_t以包含一个信号集，并且定义了下列5个处理信号集的函数。
 *      int sigemptyset(sigset_t *set);
 *      int sigfillset(sigset_t *set);
 *      int sigaddset(sigset_t *set, int signo);
 *      int sigdelset(sigset_t *set, int signo);
 *      int sigismember(const sigset_t *set, int signo);
 * 
 * 
 * sigprocmask
 *      可以检测或更改，或同时进行检测和更改进程的信号屏蔽字。
 *      sigprocmask是仅为单线程进程定义的。处理多线程进程中信号的屏蔽使用另一个函数
 *      int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
 *      how:
 *          SIG_BLOCK   : set包含了希望阻塞的附加信号
 *          SIG_UNBLOCK ：set包含了希望解除阻塞的信号
 *          SIG_SETMASK ：set指向该进程新的信号屏蔽字
 * 
 * 
 * sigpending
 *      sigpending函数返回一信号集，对于调用进程而言，其中的各信号是阻塞不能递送的，因而也一定是当前未决的。
 *      int sigpending(sigset_t *set);
 * 
 * 
 * sigaction
 *      sigaction函数的功能是检查或修改或检查并修改与指定信号相关联的处理动作。此函数取代了UNIX早期版本使用
 * 的signal函数。用sigaction函数可以实现signal函数。
 *      int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);
 * 
 * 
 * sigsuspend
 *      int sigsuspend(const sigset_t *sigmask);    在一个原子操作中先恢复信号屏蔽字，然后使进程休眠。
 *      进程的信号屏蔽字设置为由sigmask指向的值。在捕捉到一个信号或发生了一个会终止该进程的信号之前，该进程被挂起。
 * 如果捕捉到一个信号而且从该信号处理程序返回，则sigsuspend返回，并且该进程的信号屏蔽字设置为调用sigsuspend之前的值。
 *      
 *  
 * abort
 *      #include <stdlib.h>
 *      void abort(void);   
 *      此函数将SIGABRT信号发送给调用进程(进程不应忽略此信号)。ISO C规定，调用abort将向主机环境递送一个未成功终止的
 * 通知，其方法是调用raise(SIGABRT)函数。ISO C要求若捕捉到此信号而且相应信号处理程序返回，abort仍不会返回到其调用者。
 * 如果捕捉到此信号，则信号处理程序不能返回的唯一方法是它调用exit、_exit、_Exit、longjmp、或siglongjmp。让进程捕捉
 * SIGABRT的意图是：在进程终止之前由其执行所需的清理操作。如果进程并不在信号处理程序中终止自己，POSIX.1声明当信号处理
 * 程序返回时，abort终止该进程。ISO C针对此函数的规范将下列问题留由实现决定：是否要冲洗输出流以及是否要删除临时文件。
 * POSIX.1的要求则更进一步，它要求如果abort调用终止进程，则它对所有打开标准I/O流的效果应当与进程终止前对每个流调用
 * fclose相同。
 * 
 * 
 * system
*/



#include <iostream>
#include <vector>
#include <signal.h>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

using namespace std;

//信号集合
std::vector<int> SigSetAll{
	SIGINT,			/* Interactive attention signal.  */
	SIGILL,			/* Illegal instruction.  */
	SIGABRT,	    /* Abnormal termination.  */
	SIGFPE,			/* Erroneous arithmetic operation.  */
	SIGSEGV,		/* Invalid access to storage.  */
	SIGTERM,		/* Termination request.  */

/* Historical signals specified by POSIX. */
	SIGHUP,			/* Hangup.  */
	SIGQUIT,		/* Quit.  */
	SIGTRAP,		/* Trace/breakpoint trap.  */
	SIGKILL,		/* Killed.  */
    SIGBUS,			/* Bus error.  */
	SIGSYS,			/* Bad system call.  */
	SIGPIPE,		/* Broken pipe.  */
	SIGALRM,		/* Alarm clock.  */

/* New(er) POSIX signals (1003.1-2008, 1003.1-2013).  */
	SIGURG,			/* Urgent data is available at a socket.  */
	SIGSTOP,		/* Stop, unblockable.  */
	SIGTSTP,		/* Keyboard stop.  */
	SIGCONT,		/* Continue.  */
	SIGCHLD,		/* Child terminated or stopped.  */
	SIGTTIN,		/* Background read from control terminal.  */
	SIGTTOU,		/* Background write to control terminal.  */
	SIGPOLL,		/* Pollable event occurred (System V).  */
	SIGXCPU,		/* CPU time limit exceeded.  */
	SIGXFSZ,		/* File size limit exceeded.  */
	SIGVTALRM,		/* Virtual timer expired.  */
	SIGPROF,		/* Profiling timer expired.  */
	SIGUSR1,		/* User-defined signal 1.  */
	SIGUSR2,		/* User-defined signal 2.  */

/* Nonstandard signals found in all modern POSIX systems
   (including both BSD and Linux).  */
	SIGWINCH,		/* Window size change (4.3 BSD, Sun).  */
};


/**
 * 父进程调用wait或waitpid函数后调用此函数，通过解析status，可以判断回收子进程的状态。
 *      （1）正常退出
 *       (2) 信号造成的异常退出
 *       (3) 信号造成的进程暂停
 *       (4) 信号造成的进程恢复 
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

    }else if(WIFCONTINUED(status)){   //进程恢复
        printf("child continued....\n");
    }
}



//打印当前进程的信号屏蔽字
int pr_mask(const char* str){
    sigset_t blockSet;
    int errno_save = errno;
    if(sigprocmask(SIG_BLOCK, nullptr, &blockSet) == -1){   //取得当前进程的信号屏蔽字集合
        perror("sigprocmask error");
        exit(-1);
    }

    //打印该进程的屏蔽的部分信号
    printf("%s", str);
    for(auto signo : SigSetAll){
        if(sigismember(&blockSet, signo)){
            printf("%s ", strsignal(signo));
        }
    }
    printf("\n");
    errno = errno_save;
    return 0;
}


//信号处理函数，打印用户自定义信号
void sig_handler(int signo){
    printf("pid = %d, received %s\n", getpid(), strsignal(signo));
}


/**
 * 由sigaction实现的signal:
 *      * 出错返回SIG_ERR，否则返回之前设置的信号处理函数
 *      * 由该信号中断的系统调用，默认重启
*/
using Sigfunc = void(int);

Sigfunc * signal_restart(int signo, Sigfunc* func){
    struct sigaction act, oact;
    act.sa_handler = func;      //设置信号处理函数
    sigemptyset(&act.sa_mask);  //清空额外屏蔽信号集
    act.sa_flags = 0;           //设置额外标志
    if(signo == SIGALRM){
        #ifdef SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;   //不自动启动由SIGALRM信号中断的系统调用
        #endif
    }else{
        act.sa_flags |= SA_RESTART; //自动启动由该信号中断的系统调用
    }

    if(sigaction(signo, &act, &oact) == -1){
        return SIG_ERR; //出错则返回SIG_ERR
    }   
    return oact.sa_handler; //否则返回之前设置的信号处理函数
}


/**
 * 由sigaction实现的signal:
 *      * 出错返回SIG_ERR，否则返回之前设置的信号处理函数
 *      * 由该信号中断的系统调用，默认不重启动
*/
Sigfunc * signal_intr(int signo, Sigfunc* func){
    struct sigaction act, oact;
    act.sa_handler = func;          //设置信号处理函数
    sigemptyset(&act.sa_mask);      //清空额外屏蔽信号集
    act.sa_flags = 0;               //设置额外标志
#ifdef SA_INTERRUPT 
    act.sa_flags |= SA_INTERRUPT;   //默认不自动重启动由信号中断的系统调用
#endif
    if(sigaction(signo, &act, &oact) == -1){
        return SIG_ERR;             //出错则返回SIG_ERR
    }   
    return oact.sa_handler;         //否则返回之前设置的信号处理函数
}


/**
 *  sigpending：返回当前进程的阻塞信号集合
 *      配置信号处理函数
 *      阻塞信号
 *      恢复信号：先执行信号处理函数后从sigprocmask函数中返回
*/
void sigpending_test(){
    std::cout << "pid: " << getpid() << std::endl;

    //注册SIGQUIT信号处理函数
    if(signal(SIGQUIT, sig_handler) == SIG_ERR){
        perror("cant't catch SIGQUIT");
        exit(-1);
    }

    //将SIGQUIT加入屏蔽信号集，并保存当前信号集合，用于后面恢复
    sigset_t newmask, oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1){
        perror("SIG_BLOCK error");
        exit(-1);
    }

    std::cout << "SIGQUIT is blocked now, start to signal a SIGQUIT ..." << std::endl;
    sleep(10);

    //取得当前产生但是被阻塞的信号，判断SIGQUIT是否被阻塞
    sigset_t pendmask;
    if(sigpending(&pendmask) == -1){
        perror("sigpending error");
        exit(-1);
    }
    if(sigismember(&pendmask, SIGQUIT)){
        printf("\nSIGQUIT pending\n");
    }

    //恢复当前进程的屏蔽信号集合 （解除SIGQUIT信号的阻塞）
    //如果有信号递送，则在sigprocmask返回前，至少执行其中一个信号处理函数
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) == -1){
        perror("SIG_SETMASK error");
        exit(-1);
    }
   
    //此时不再阻塞SIGQUIT信号，默认操作是结束进程
    std::cout << "SIGQUIT unblocked..." << std::endl;
    exit(0);
}


void sigsuspend_handler(int signo){
    pr_mask("in sigsuspend handler: ");
}

/**
 * 注册信号处理函数
 * 阻塞信号，进行关键数据处理
 * 设置新的信号屏蔽字并等待信号
 * 恢复最初的信号屏蔽字
*/
void sigsuspend_test1(){
    sigset_t newmask, oldmask, waitmask;
    pr_mask("program start: ");                             //打印进程屏蔽信号集合
    if(signal(SIGINT, sigsuspend_handler) == SIG_ERR){      //注册信号处理函数
        perror("signal(SIGINT) error");
        exit(-1);
    }
    if(signal(SIGQUIT, sigsuspend_handler) == SIG_ERR){     //注册信号处理函数
        perror("signal(SIGINT) error");
        exit(-1);
    }

    //信号集合初始化并赋值
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigaddset(&newmask, SIGQUIT);
    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1){   //阻塞SIGINT，SIGQUIT
        perror("SIG_BLOCK error");
        exit(-1);
    }
    pr_mask("in critical region: ");

    sleep(10);  //执行期间阻塞SIGINT,SIGQUIT
    std::cout << "critical region code complete..." << std::endl;

    /**
     * 设置新的信号屏蔽字并等待，这是一个原子操作。并且在结束返回后，自动恢复原先的信号屏蔽字。
     * 以下代码不是原子操作，有时间窗口
     *      sigprocmask();
     *      ------信号在这里发生，会导致无法唤醒pause，即信号丢失------
     *      pause()
    */
    if(sigsuspend(&waitmask) != -1){    //设置新的信号屏蔽字并阻塞等待
        perror("sigsuspend error");
        exit(-1);
    }
    pr_mask("after return from sigsuspend: ");  //返回时恢复信号屏蔽字

    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) == -1){  //恢复进程开始时的信号屏蔽字
        perror("SIG_SETMASK error");
    }
    pr_mask("program exit: ");
    exit(0);
}

/**
 * 程序执行完必要操作后，调用sigsuspend()等待信号发生之前，首先要阻塞该信号。
 * 否则该信号有可能在sigsuspend之前发生，这样就会错过信号。
*/
void consumeTime(){
    time_t before = time(nullptr);
    volatile int k = 0;
    for(volatile int i = 0; i < 30000; ++i){
        for(volatile int j = 0; j < 100000; ++j){
            k = i + j;
        }   
    }
    time_t after = time(nullptr);
    printf("excute time: %ld\n", after - before);
}

volatile sig_atomic_t quitflag = 0;     //可以被原子修改，不会被信号处理程序中断

void sigsuspend_handler2(int signo){
    printf("pid = %d, received %s\n", getpid(), strsignal(signo));
    if(signo == SIGQUIT){
        quitflag = 1;
    }
}

/**
 * 等待一个信号处理程序设置一个全局变量
*/
void sigsuspend_test2(){
    sigset_t newmask, oldmask, zeromask;
    if(signal(SIGINT, sigsuspend_handler2) == SIG_ERR){     //注册SIGINT信号处理函数
        perror("signal(SIGINT) error"); 
        exit(-1);
    }
    if(signal(SIGQUIT, sigsuspend_handler2) == SIG_ERR){    //注册SIGQUIT信号处理函数
        perror("signal(SIGQUIT) error");
        exit(-1);
    }

    consumeTime();

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    sigemptyset(&zeromask);

    //进入判断逻辑前，首先阻塞该信号
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1){   //阻塞SIGQUIT
        perror("sigprocmask error");
        exit(-1);
    }
    /**
     * 这里首先阻塞了SIGQUIT信号，这样在执行sigsuspend()之前SIGQUIT一定不会被递送给进程
     * 如果不首先阻塞该信号的话，在判断语句和sigsuspend()之间留有时间窗口
     * sigsuspend是原子操作，这样保证进程收到信号后，一定会唤醒sigsuspend而不会进入休眠。
    */
    printf("wait for SIGQUIT...\n");
    while(quitflag == 0){
        //----------不阻塞信号，这里会有时间窗口----------
        sigsuspend(&zeromask);
    }

    quitflag = 0;
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) == -1){
        perror("SIG_SETMASK error");
        exit(-1);
    }

    exit(0);
}


/**
 * 进程之间的同步
 * 要求每个进程在执行完它的一套初始化操作后要通知对方，并且在继续运行之前，要等待另一方完成其初始化操作
*/

static volatile sig_atomic_t sigflag = 0;

static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo){
    sigflag = 1;
}

/**
 * 信号实现的一种进程间的同步机制:  
 *      阻塞：进程阻塞等待全局变量被修改
 *      唤醒：收到对方进程发送的信号后，在信号处理函数中修改全局变量，进程跳过阻塞，执行后续步骤
*/

void TELL_WAIT(){
    if(signal(SIGUSR1, sig_usr) == SIG_ERR){
        perror("signal(SIGUSR1) error");
        exit(-1);
    }
    if(signal(SIGUSR2, sig_usr) == SIG_ERR){
        perror("signal(SIGUSR2) error");
        exit(-1);
    }

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    sigemptyset(&zeromask);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        perror("SIG_BLOCK error");
        exit(-1);
    }
}

void TELL_PARENT(pid_t pid){
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(){
    while(sigflag == 0){
        sigsuspend(&zeromask);
    }
    sigflag = 0;
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        perror("SIG_SETMASK error");
        exit(-1);
    }
}

void TELL_CHILD(pid_t pid){
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(){
    while(sigflag == 0){
        sigsuspend(&zeromask);
    }
    sigflag = 0;
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        perror("SIG_SETMASK error");
        exit(-1);
    }
}

//标准输出取消缓冲，增加系统函数的调用次数
void printStrOnebyOne(const string& str){
    setbuf(stdout, nullptr);
    for(int i = 0; i < str.size(); ++i){
        putc(str[i], stdout);
    }
}

//父进程和子进程之间没有同步，存在竞争
void process_no_sync(){
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){ //子进程
        printStrOnebyOne("output from child\n");
    }else{  //父进程
        printStrOnebyOne("output from parent\n");
    }
    exit(0);
}

void process_sync(){
    pid_t pid;
    TELL_WAIT();
    if((pid = fork()) < 0){     //父子进程拥有相同的信号屏蔽字
        perror("fork error");
        exit(-1);
    }else if(pid == 0){
        printStrOnebyOne("output from child\n");
        TELL_PARENT(getppid());
    }else{
        WAIT_CHILD();
        printStrOnebyOne("output from parent\n");
    }
    exit(0);
}

/**
 * 每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有线程共享的。
 * 
 * 进程中的信号是递送到单个线程的。如果一个信号与硬件故障相关，那么该信号一般会被发送到引起该事件的线程中去，
 * 而其它的信号则被发送到任意一个线程。
 * 
 * sigprocmask是针对单线程进程的，其行为在多线程进程中并没有定义，线程必须使用pthread_sigmask。
 * 
 * 线程可以通过调用sigwait等待一个或多个信号的出现。
 * 
*/
int quitflag1 = 0;
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void* thr_fn(void *arg){
    sigset_t blockset;
    sigemptyset(&blockset);
    sigaddset(&blockset, SIGUSR1);
    sigaddset(&blockset, SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &blockset, nullptr);
    pr_mask("thr_fn: ");

    int err, signo;
    for(; ;){
        err = sigwait(&mask, &signo);   //线程阻塞等待信号，直到被信号唤醒。
        if(err != 0){
            printf("sigwait error: %s\n", strerror(err));
            exit(-1);
        }
        switch(signo){
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&lock);  //加锁安全操作。
                quitflag1 = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&waitloc);
                return(0);
            default:
                printf("unexcepted signal %s\n", strsignal(signo));
                exit(1);
        }
    }
}

void sig_multiThread_process(){
    int err;
    sigset_t oldmask;
    pthread_t tid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    pr_mask("start: ");
    
    if((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0){
        printf("pthread_sigmask error: %s\n", strerror(err));
        exit(-1);
    }
    sleep(1);
    pr_mask("after pthread_sigmask: ");

    err = pthread_create(&tid, nullptr, thr_fn, nullptr);
    if(err != 0){
        printf("pthread_create error: %s\n", strerror(err));
        exit(-1);
    }

    sleep(2);
    pr_mask("after pthread_create: ");


    pthread_mutex_lock(&lock);
    while(quitflag1 == 0){
        pthread_cond_wait(&waitloc, &lock); //线程等待被唤醒，满足条件则执行后续逻辑，否则继续陷入等待
    }
    pthread_mutex_unlock(&lock);


    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        perror("SIG_SETMASK error");
        exit(-1);
    }
    exit(0);
}



int main(int argc, char* argv[]){

   sig_multiThread_process();


    return 0;
}

