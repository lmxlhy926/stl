
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
 *      （1）信号集合
 *      （2）向进程发送信号，向线程发送信号
 *       (3) 屏蔽线程信号集。新创建的线程继承调用线程的屏蔽信号集合。
 *      （4）返回线程阻塞的信号集合。（产生但是被阻塞的信号集合）
 *      （4）设置信号响应函数。
 *      （5）阻塞等待指定的信号集合中的信号发生。
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
 * kill和raise
 *      kill函数将信号发送给进程或进程组。raise函数则允许进程向自身发送信号。
 * 
 * 
 * pthread_kill
 *      int pthread_kill(pthread_t thread, int signo);
 *      向指定的线程发送信号
 * 
 * 
 * sigprocmask
 *      sigprocmask是仅为单线程进程定义的。处理多线程进程中信号的屏蔽使用pthread_sigmask。
 *      可以检测或更改，或同时进行检测和更改进程的信号屏蔽字。
 *      int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
 *      how:
 *          SIG_BLOCK   : set包含了希望阻塞的附加信号
 *          SIG_UNBLOCK ：set包含了希望解除阻塞的信号
 *          SIG_SETMASK ：set指向该进程新的信号屏蔽字
 * 
 * 
 * pthread_sigmask
 *      int pthread_sigmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
 *          pthread_sigmask工作在线程中，而且失败时返回错误码。
 * 
 * 
 * sigaction
 *      int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);
 *          sigaction函数的功能是检查或修改或检查并修改与指定信号相关联的处理动作。
 *          此函数取代了UNIX早期版本使用的signal函数。用sigaction函数可以实现signal函数。
 * 
 * 
 * sigpending：线程阻塞信号集合
 *      int sigpending(sigset_t *set);
 *          sigpending() returns the set of signals that are pending for delivery to the calling thread.
 * 
 *          If a signal is both blocked and has a disposition of "ignored", it is not added to the mask of pending
 *          signals when generated.
 * 
 *          The set of signals that is pending for a thread is the union of the set of signals  that  is  pending  for 
 *          that thread and the set of signals that is pending for the process as a whole; see signal(7).
 * 
 *
 * sigsuspend
 *      int sigsuspend(const sigset_t *sigmask);    在一个原子操作中先恢复信号屏蔽字，然后使进程休眠。
 * 
 *      sigsuspend()  temporarily  replaces  the signal mask of the calling thread with the mask given by mask and
 * then suspends the thread until delivery of a signal whose action is to invoke a signal handler or to  terminate a process.
 *      
 *      If the signal terminates the process, then sigsuspend() does not return.  If the signal is caught, then
 * sigsuspend() returns after the signal handler returns, and the signal mask is restored to the state before
 * the call to sigsuspend().
 * 
 *      Normally, sigsuspend() is used in conjunction with sigprocmask(2) in order to prevent delivery of a signal
 * during the execution of a critical code section.  The caller first blocks the signals with sigprocmask(2).
 * When the critical code has completed, the caller then waits for the signals by calling sigsuspend() with
 * the signal mask that was returned by sigprocmask(2) (in the oldset argument).
 * 
 * 
 * int sigwait(const sigset_t *restrict set, int *restrict signop);
 *      set指定了线程等待的信号集。返回时signop指向触发返回的信号编号。
 * 
 *      The  sigwait()  function suspends execution of the calling thread until one of the signals specified
 *   in the signal set set becomes pending.  The function accepts the signal (removes it from the pending
 *   list of signals), and returns the signal number in sig
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
*/



#include <iostream>
#include <vector>
#include <signal.h>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <pwd.h>
#include <sys/wait.h>

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


//打印当前线程的信号屏蔽字
int pr_mask(const char* str){
    int err;
    sigset_t blockSet;
    if((err = pthread_sigmask(SIG_BLOCK, nullptr, &blockSet)) != 0){   //取得当前线程的信号屏蔽字集合
        printf("pthread_sigmask error: %s\n", strerror(err));
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
    fflush(stdout);
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
        act.sa_flags |= SA_RESTART; //自动启动由该信号中断的其它系统调用
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

    /**
     * 阻塞SIGQUIT
     * 将SIGQUIT加入屏蔽信号集，并保存当前信号集合，用于后面恢复
    */
    sigset_t newmask, oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1){
        perror("SIG_BLOCK error");
        exit(-1);
    }

/*------------在SIGQUIT被阻塞情况下，执行代码逻辑------------*/
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
 /*------------在SIGQUIT被阻塞情况下，执行代码逻辑------------*/

    /**
     * 恢复当前进程的屏蔽信号集合 （解除SIGQUIT信号的阻塞）
     * 如果有信号递送，则在sigprocmask返回前，至少执行其中一个信号处理函数
    */
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) == -1){
        perror("SIG_SETMASK error");
        exit(-1);
    }
   
    //此时不再阻塞SIGQUIT信号，默认操作是结束进程
    std::cout << "SIGQUIT unblocked..." << std::endl;
    exit(0);
}


void sigsuspend_handler1(int signo){
    printf("sigsuspend_handler1, pid = %d, received %s\n", getpid(), strsignal(signo));
    sleep(2);
    printf("sigsuspend_handler1 end....\n");
}

void sigsuspend_handler2(int signo){
    printf("sigsuspend_handler2, pid = %d, received %s\n", getpid(), strsignal(signo));
    sleep(3);
    printf("sigsuspend_handler2 end....\n");
}

/**
 * 注册信号处理函数
 * 阻塞信号，进行关键数据处理
 * 设置新的信号屏蔽字并等待信号
 * 恢复最初的信号屏蔽字
*/
void sigsuspend_test1(){
    sigset_t newmask, oldmask;
    pr_mask("program start: ");                             //打印进程屏蔽信号集合
    
    /**
     * 注册信号处理函数
    */
    if(signal(SIGINT, sigsuspend_handler1) == SIG_ERR){      //注册信号处理函数
        perror("signal(SIGINT) error");
        exit(-1);
    }
    if(signal(SIGQUIT, sigsuspend_handler2) == SIG_ERR){     //注册信号处理函数
        perror("signal(SIGINT) error");
        exit(-1);
    }

    /**
     * 阻塞SIGINT、SIGQUIT
    */
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1){   //阻塞SIGINT，SIGQUIT
        perror("SIG_BLOCK error");
        exit(-1);
    }


    pr_mask("in critical region: ");
    sleep(10);  //执行期间阻塞SIGINT,SIGQUIT
    std::cout << "critical region code complete..." << std::endl;


    /**
     * 暂时恢复信号集合，并等待信号发生
     * 设置新的信号屏蔽字并等待，这是一个原子操作。并且在结束返回后，自动恢复原先的信号屏蔽字。
     * 以下代码不是原子操作，有时间窗口
     *      sigprocmask();  //恢复信号屏蔽字
     *      ------信号在这里发生，会导致无法唤醒pause，即信号丢失------
     *      pause()         //阻塞
    */
    if(sigsuspend(&oldmask) != -1){    //设置新的信号屏蔽字并阻塞等待
        perror("sigsuspend error");
        exit(-1);
    }
    pr_mask("after return from sigsuspend: ");  //返回时恢复信号屏蔽字


    /**
     * 恢复进程开始时的信号屏蔽字
    */
    if(sigprocmask(SIG_SETMASK, &oldmask, nullptr) == -1){  
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

void sigsuspend_handler3(int signo){
    printf("pid = %d, received %s\n", getpid(), strsignal(signo));
    if(signo == SIGQUIT){
        quitflag = 1;
    }
}

/**
 * 等待一个信号处理程序设置一个全局变量
*/
void sigsuspend_test2(){
    /**
     * 注册信号响应函数
    */
    if(signal(SIGINT, sigsuspend_handler3) == SIG_ERR){    
        perror("signal(SIGINT) error"); 
        exit(-1);
    }
    if(signal(SIGQUIT, sigsuspend_handler3) == SIG_ERR){    
        perror("signal(SIGQUIT) error");
        exit(-1);
    }

    //进入判断逻辑前，首先阻塞该信号
    sigset_t newmask, oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1){   //阻塞SIGQUIT
        perror("sigprocmask error");
        exit(-1);
    }

    /**
     * 这里首先阻塞了SIGQUIT信号，这样在执行sigsuspend()之前SIGQUIT一定不会被递送给进程
     * 如果不首先阻塞该信号的话，在判断语句和sigsuspend()之间留有时间窗口
     * sigsuspend是原子操作，这样保证进程收到信号后，一定会唤醒sigsuspend而不会进入休眠。
    */
    consumeTime();  //执行逻辑
    printf("wait for SIGQUIT...\n");
    while(quitflag == 0){
        //----------不阻塞信号，这里会有时间窗口----------
        sigsuspend(&oldmask);
    }
 
    /**
     * 恢复信号屏蔽字
    */
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
 *      阻塞：阻塞特定信号集合  
 *      等待：调用sigsuspend进行等待，直到全局变量被修改
 *      唤醒：向指定进程发送信号
 */


/**
 * 初始化操作：设置信号捕捉函数，屏蔽指定信号集合
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

//阻塞等待被唤醒
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
    /**
     * 设置信号屏蔽集合，设置信号处理函数
     * 子进程继承父进程的信号屏蔽字以及信号处理函数
    */
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


void printBlockedSet(const char* str, sigset_t set){
    printf("%s", str);
    for(auto signo : SigSetAll){
        if(sigismember(&set, signo)){
            printf("%s ", strsignal(signo));
        }
    }
    printf("\n");
    fflush(stdout);
}



/**
 * 每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有线程共享的。这意味着单个线程可以阻止某些信号，
 * 但当某个线程修改了与某个给定信号相关的处理行为后，所有的线程都必须共享这个处理行为的改变。即对进程来说
 * 信号的行为是唯一且确定的，每个线程都可以修改信号的行为，信号的行为由最近一次的修改行为决定。
 * 
 * 进程中的信号是递送到单个线程的。如果一个信号与硬件故障相关，那么该信号一般会被发送到引起该事件的线程中去，
 * 而其它的信号则被发送到任意一个线程。
 * 
 * sigprocmask是针对单线程进程的，其行为在多线程进程中并没有定义，线程必须使用pthread_sigmask。
 * 
 * 
 * int sigwait(const sigset_t *restrict set, int *restrict signop);
 *      set指定了线程等待的信号集。返回时signop指向触发返回的信号编号。
 *      如果信号集中的某个信号在sigwait调用的时候处于挂起状态，那么sigwait将无阻塞的返回。在返回之前，sigwait
 * 将从进程中移除那些处于挂起等待状态的信号。sigwait函数会原子的取消信号集的阻塞状态，直到有新的信号被递送。在
 * 返回之前，sigwait将恢复线程的信号屏蔽字。如果信号在sigwait被调用的时候没有被阻塞，那么在线程完成对sigwait的
 * 调用之前会出现一个时间窗，在这个时间窗中，信号就可以被发送给线程。为了避免错误行为发生，线程在调用sigwait之前，
 * 必须阻塞那些它正在等待的信号。
 *      使用sigwait的好处在于它可以简化信号处理，允许把异步产生的信号用同步的方式处理。为了防止信号中断线程，可以
 * 把信号加到每个线程的信号屏蔽字中。然后可以安排专用线程处理信号。这些专用线程可以进行函数调用，不需要担心在信号
 * 处理程序中调用哪些函数是安全的，因为这些函数调用来自正常的线程上下文，而非会中断线程正常执行的传统信号处理程序。
 *      如果多个线程在sigwait的调用中因等待同一个信号而阻塞，那么在信号递送的时候，就只有一个线程可以从sigwait中
 * 返回。如果一个信号被捕获，而且一个线程正在sigwait调用中等待同一信号，那么这时将由操作系统实现来决定以何种方式递
 * 送信号。操作系统实现可以让sigwait返回，也可以激活信号处理程序，单这2种情况不会同时发生。
 * 
*/

int quitflag1 = 0;
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

/**
 * (1) 确定要响应的信号。
 *（2）在其它线程中屏蔽上述信号，开启一个专用线程处理信号。调用sigwait等待信号前，线程要先屏蔽该信号，否则会有时间窗。
 * (3) 信号唤醒sigwait后，线程恢复其原先的屏蔽信号集，然后开始执行线程后续逻辑
 * 
 * 对比：
 *      (1) 传统的信号处理程序中，必须调用异步安全的函数，因为传统信号处理程序会中断其它线程的正常执行，造成函数的重入。
 * 很多函数是不可重入的，因此造成错误。
 *      (2) 采用sigwait的方式时，其它的线程屏蔽要响应的信号。专用线程中开始时也屏蔽响应信号，然后调用sigwait原子操作
 * 解除信号屏蔽并等待。信号递送时唤醒sigwait,然后执行线程后续逻辑。在此过程中，没有函数被中断，因此只要调用线程安全
 * 的函数即可。
 * 
 * sigsuspend和sigwait的区别：
 *      sigsuspend中指定线程要屏蔽的信号集合，其可被未指定的信号唤醒。但是不知道是被哪个信号唤醒。且在返回前执行
 * 未决的未被阻塞的所有的信号的处理函数。
 *      sigwait中指定线程要等待的信号的集合，被唤醒后，知道是被哪个信号唤醒的。
 * 
 * 注：
 *      通过阻塞信号以及在专用线程中处理信号，可以解决信号中断带来的问题。
*/


/**
 * 在专用线程里用sigwait等待的信号，就不再使用sigaction指定函数进行捕获。
 * 否则具体是由线程来处理该信号，还是由信号处理函数来处理该信号，就由操作系统来决定。
*/
void* thr_fn(void *arg){
    int err, signo;
    for(; ;){
        /**
         * sigwait每返回一次，线程阻塞信号集合中就有一个信号被清除
        */
        sigset_t pendmask;
        if(sigpending(&pendmask) == -1){
            perror("sigpending error");
            exit(-1);
        }
        printBlockedSet("sigpending: ", pendmask);

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
    int err;
    sigset_t oldmask;
    pthread_t tid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);
    if((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0){   //设置主线程信号屏蔽字
        printf("pthread_sigmask error: %s\n", strerror(err));
        exit(-1);
    }

    printf("wait for 5 seconds....\n");
    sleep(5);

    //新创建的线程继承调用线程的信号屏蔽字
    err = pthread_create(&tid, nullptr, thr_fn, nullptr);
    if(err != 0){
        printf("pthread_create error: %s\n", strerror(err));
        exit(-1);
    }
   
    pthread_mutex_lock(&lock);
    while(quitflag1 == 0){
        pthread_cond_wait(&waitloc, &lock); //线程等待被唤醒，满足条件则执行后续逻辑，否则继续陷入等待
    }
    pthread_mutex_unlock(&lock);
   
    //恢复线程信号屏蔽字
    if((err = pthread_sigmask(SIG_SETMASK, &oldmask, nullptr)) != 0){
        printf("pthread_sigmask error: %s\n", strerror(err));
        exit(-1);
    }

    exit(0);
}


void sigAbort_handler(int signo){
    std::cout << "start to wait for 5 seconds ..." << std::endl;
    sleep(5);
    std::cout << "wait complete ..." << std::endl;
}

/**
 * abort：SIGABRT
 *      ISO C要求若捕捉到此信号而且相应信号处理程序返回，abort仍不会返回到其调用者。
 *      POSIX.1也说明abort并不理会进程对此信号的阻塞和忽略。
 *      让进程捕捉SIGABRT的意图是：在进程终止之前由其执行所需的清理操作。如果进程并不在信号处理程序中终止自己，POSIX.1声明
 *      当信号处理程序返回时，abort终止该进程。
*/
void abort_own(){
    struct sigaction action;

    //获取之前的设置,，如果SIGABRT被忽略，则恢复为默认。（忽略进程对SIGABRT的忽略）
    sigaction(SIGABRT, nullptr, &action);
    if(action.sa_handler == SIG_IGN){
        action.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &action, nullptr);
    }

    //如果SIGABRT是默认操作，则刷新标准库缓冲区
    if(action.sa_handler == SIG_DFL){
        fflush(nullptr);
    }

    //阻塞除SIGABRT之外的所有信号（忽略进程对SIGABRT的阻塞）
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
    printf("return from kill ...\n");

    /**
     * 对于捕获SIGABRT但是返回的handler，控制流会到达这里
     * 设置SIGABRT信号handler为默认，再次发送，终止进程
    */
    fflush(nullptr);
    action.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &action, nullptr);
    sigprocmask(SIGABRT, &mask, nullptr);   /* just in case ... */
    kill(getpid(), SIGABRT);

    exit(1);    /* this should never be executed ... */
}


void abort_test(){
    struct sigaction newSigAction, oldSigAction;
    newSigAction.sa_handler = sigAbort_handler;
    sigemptyset(&newSigAction.sa_mask);
    newSigAction.sa_flags = 0;
    sigaction(SIGABRT, &newSigAction, &oldSigAction);
    abort_own();
    while(true){
        sleep(10);
    }
}


void waitpid_handler(int signo){
    int saveErrno = errno;
    printf("waitpid_handler, pid = %d, received %s\n", getpid(), strsignal(signo));
    if(signo == SIGCHLD){
        int status;
        pid_t pid;
        if(pid = wait(&status) != -1){
            pr_exit(status);
        }else{
            perror("waitpid_handler wait error: ");
        }
    }
    errno = saveErrno;
}


/**
 * 没有捕获信号的system函数的一个实现版本
 *      当system运行另一程序时，不应使父、子进程两者都捕捉终端产生的2个信号：中断和退出。这2个信号只应该发送
 * 给正在运行的程序：子进程。因为由system执行的命令可能是交互式命令，以及因为system的调用者在程序执行时放弃了
 * 控制，等待该执行程序的结束，所以system的调用者就不应该接收这2个终端产生的信号。
 *      但是若父进程正捕捉SIGCHLD信号，那么正在执行system函数时，应当阻塞对父进程递送SIGCHLD信号。实际上，这
 * 就是POSIX.1所说明的。否则，当system创建的子进程结束时，system的调用者可能错误的认为，它自己的一个子进程结束
 * 了。于是，调用者将会调用一种wait函数以获得子进程的终止状态，这样就阻止了system函数获得子进程的终止状态，并将
 * 其作为它的返回值。
*/
int system_nosignal(const char *cmdstring){
    if(cmdstring == nullptr){
        return 1;
    }
    pid_t pid;
    int status = -1;
    if((pid = fork()) < 0){
        status = -1;
    }else if(pid == 0){
        std::cout << "child process: " << getpid() << std::endl;
        execl("/usr/bin/bash", "sh", "-c", cmdstring, nullptr);
        _exit(127);
    }

    sleep(20);
    while(waitpid(pid, &status, 0) < 0){
        perror("system_nosignal waitpid error");
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

    /* 父进程忽略终端产生的SIGINT、SIGINT， 父进程屏蔽SIGCHLD */
    struct sigaction ignore, saveintr, savequit;
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;

    if(sigaction(SIGINT, &ignore, &saveintr) < 0){
        return -1;
    }
    if(sigaction(SIGQUIT, &ignore, &savequit) < 0){
        return -1;
    }

    sigset_t chldmask, savemask;
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
        sigaction(SIGINT, &saveintr, nullptr);          //恢复信号的处理方式
        sigaction(SIGQUIT, &savequit, nullptr);
        sigprocmask(SIG_SETMASK, &savemask, nullptr);   //恢复之前的阻塞信合集合
        execl("/usr/bin/bash", "bash", "-c", cmdstring, nullptr);
        _exit(127);
    }
    //父进程等待回收子进程
    while(waitpid(pid, &status, 0) < 0){
        if(errno != EINTR){
            status = -1;
            break;
        }
    }
    
    if(sigaction(SIGINT, &saveintr, nullptr) < 0)
        return -1;
    if(sigaction(SIGQUIT, &savequit, nullptr) < 0)
        return -1;
    if(sigprocmask(SIG_SETMASK, &savemask, nullptr) < 0)
        return -1;

    return status;
}

void system_test(){
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
    signal(SIGCHLD, waitpid_handler);
    int status = system_signal("/usr/bin/ed");
    std::cout << "status: " << status << std::endl;
    exit(0);
}


/**
 * SIGTSTP SIGCONT
 * 仅当某个进程向该进程发送一个SIGCONT信号时，该进程才继续。我们不捕捉SIGCONT信号。该信号的默认配是继续运行停止的进程。
 * 仅当SIGTSTP信号的配置是SIG_DFL才安排捕捉该信号，其理由是：当此程序由不支持作业控制的shell启动时，
 * 此信号的配置应当设置为SIG_IGN。只有作业控制shell才应将这3个信号重新设置为SIG_DFL。
*/

void sigtstop_test(){
    std::cout << "pid: " << getpid() << std::endl;
    while(true){
        std::cout << "hello world" << std::endl;
        sleep(1);
    }
    
    exit(0);
}

/**
 * 打印信号
*/
void signal_print_test(){
    psignal(SIGABRT, "print");
    char* ptr = strsignal(SIGABRT);
    printf("strsignal: %s\n", ptr);
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
    newact.sa_handler = sig_handler;  //信号处理函数
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
 * sleep是由alarm实现的，同时使用alarm和sleep时不会受影响。
*/
void sleep_test(){
    std::cout << "alarm in 3 seconds ... " << std::endl;
    signal(SIGALRM, sig_handler);
    alarm(2);
    sleep(1);   //1秒后唤醒
    std::cout << "wake up 1 ..." << std::endl;
    sleep(100); //再过1秒后被闹钟唤醒
     std::cout << "wake up 2 ..." << std::endl;
}


/**
 * 更精确的延时
*/
void nanosleep_test(){
    struct timespec sleeptime, unsleeptime{};
    sleeptime.tv_sec = 15;  //秒
    sleeptime.tv_nsec = 1000 * 1000 * 1000;  //纳秒
    nanosleep(&sleeptime, &unsleeptime);
    std::cout << "unsleeptime.tv_sec: " << unsleeptime.tv_sec
              << " , unsleeptime.tv_nsec: " << unsleeptime.tv_nsec << std::endl;
}


int main(int argc, char* argv[]){
    sleep_test();

    return 0;
}


