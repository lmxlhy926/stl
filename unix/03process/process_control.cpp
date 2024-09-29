
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;


//tag---------------------------创建进程---------------------------
/**
 * UNIX系统的进程控制，包括创建新进程、执行程序和进程终止。
 * 进程标识：
 *      每个进程都有一个非负整型表示的唯一进程ID。
 *      ID为0进程通常是调度进程，常常被称为交换进程，是内核的一部分；
 *      进程ID为1通常是init进程，在自举过程结束时由内核调用。init进程绝对不会终止，是所有孤儿进程的父进程。
*/

/**
 * fork:
 *      子进程是父进程的副本。子进程获得父进程数据空间、堆和栈的副本。注意，这是子进程所拥有的独立副本。
 *      父进程和子进程并不共享这些存储空间部分。父进程和子进程共享正文段。正文段是只读的。
 *      父子进程之间遵循读时共享写时复制的原则，目的是节省内存开销，逻辑上各个进程的变量之间是独立的
 * 
 *      fork失败的2个主要原因是：
 *          1. 系统中已经有了太多的进程
 *          2. 该实际用户ID的进程总数超过了系统限制。CHILD_MAX规定了每个实际用户ID在任一时刻可拥有的最大进程数。
 *      
 *      fork的2种用法：
 *          1. 一个父进程希望复制自己，使父进程和子进程同时执行不同的代码段。这在网络服务进程中是常见的——父进程等待客户端的服务请求。
 *             当这种请求到达时，父进程调用fork, 使子进程处理此请求。父进程则继续等待下一个服务请求。
 *          2. 一个进程要执行一个不同的程序。这对shell是常见的情况。在这种情况下，子进程从fork返回后立即调用exec。
 * 
 *          将fork和exec分开，使得子进程在fork和exec之间可以更改自己的属性，如I/O重定向、用户ID、信号安排等。
 * 
 * fork之后
 *      父子相同处：.data .text 堆 栈 
 *                 环境变量、控制终端、当前工作目录、文件描述符
 * 
 *      父子不同处：1.进程id  2.fork返回值  3.父进程Id  4.进程运行时间  
 *                 5.子进程不继承父进程设置的文件锁
 *                 5.子进程的未处理闹钟被清除
 *                 6.子进程的未处理信号集设置为空集
 * 
 *      注：相同的意思是值相同,不代表物理存储空间相同。每个进程的逻辑物理空间是完全独立的。
 * 
 * 
 * 父子进程的执行顺序：
 *      一般来说，fork之后是父进程先执行还是子进程先执行是不确定的，这取决于内核所用的调度算法。
 *      如果要求父进程和子进程之间相互同步，则要求某种形式的进程间通信。
 *      fork之后可以使用信号使父进程和子进程同步。
 * 
 * 
 * 父子进程之间的文件共享：
 *      在重定向父进程的标准输出时，子进程的标准输出也被重定向。父进程和子进程每个相同的打开描述符共享一个文件表项。父进程和子进程共享同一个文件偏移量。
 *      如果父进程和子进程写同一描述符指向的文件，但又没有任何形式的同步，那么它们的输出就会相互混合，
 *      这里假定所用的描述符是在fork之前打开的。但这不是常用的操作模式。
 * 
 *      fork之后处理文件描述符的2种常见情况
 *          父进程等待子进程完成：
 *              父进程无需对其描述符做任何处理。当子进程终止后，它曾进行过读、写操作的任一共享描述符的文件偏移量已做了相应更新。
 *          父进程和子进程各自执行不同的程序段
 *              fork之后，父进程和子进程各自关闭它们不需要使用的文件描述符，这样就不会干扰对方使用的文件描述符。
 * 
 * 
 * 数据空间：.data区域，栈空间，堆空间
 * 父子进程拥有独立的物理地址空间，fork后其虚拟地址空间相同，但是写时复制机制会造成相同虚拟地址指向的物理空间是不同的。
 * 进程地址空间独立，不会相互影响。
 * 
 * fork时标准IO库的缓冲区内容处理：
 *      取决于fork前，标准IO库的缓冲区内容是否被冲洗。
 * 
 *      标准输出行缓冲：
 *          如果标准输出连接到终端设备，则它是行缓冲的；否则它是全缓冲的。
 *          当以交互方式运行该程序时，fork之前printf的内容只输出一次，其原因是标准输出缓冲区由换行符冲洗。
 * 
 *      标准输出全缓冲：
 *          但是将标准输出重定向到一个文件时，fork之前printf的内容输出两次。其原因是，在fork之前调用了printf，但当调用fork时，
 *          该行数据仍在缓冲区中，然后在将父进程数据空间复制到子进程中时，该缓冲区数据也被复制到子进程中，此时父进程和子进程各自有
 *          了带该行内容的缓冲区。
 *      
 *      printf输出机制：
 *          (1) 将内容存入printf的缓冲区中，依据缓冲刷新机制，调用write系统调用将缓冲内容输出到设备驱动程序，设备驱动再将内容输出到终端。
*/



/**
 * @brief 
 *      1. 进程层级在内核之上
 *      2. 非阻塞IO的内容都是立即写入内核缓冲区的
 *      3. printf的不同表现，取决于在fork之前，标准库缓冲区的内容有没有写入内核
 *      4. 不同进程的相同虚拟地址，指向的是不同的物理空间；（父子进程，没有发生写时复制时，指向相同的物理空间）
 */
int globalData = 1;  //.data区域
void parent_child_writeOnCoyp(){
    int stackVariable = 1;    //栈变量
    int* intPtr = reinterpret_cast<int *>(malloc(sizeof(int)));     //申请堆地址空间
    *intPtr = 1;
    printf("pid = %d, globalData = %d, stackVariable = %d, intPtr = %p, *intPtr = %d\n", getpid(), globalData, stackVariable, intPtr, *intPtr);    

    write(STDOUT_FILENO, "helloworld\n", 11);     //无缓冲写
    printf("before fork\n");    //标准库带缓冲写；行缓冲或全缓冲，取决于标准输出对应的文件。
    fflush(stdout); //刷新缓冲。否则fork后printf输出内容会输出2次。

    pid_t pid;
    if((pid = fork()) < 0){ //创建新进程
        perror("fork error");
        exit(-1);
    }else if(pid == 0){ //子进程
        globalData++;       //修改.data区域变量
        stackVariable++;    //修改栈变量
        *intPtr = 2;        //修改堆变量
        printf("pid = %d, globalData = %d, stackVariable = %d, intPtr = %p, *intPtr = %d\n", getpid(), globalData, stackVariable, intPtr, *intPtr);
    }else{  //父进程
        sleep(1);
        printf("pid = %d, globalData = %d, stackVariable = %d, intPtr = %p, *intPtr = %d\n", getpid(), globalData, stackVariable, intPtr, *intPtr);
    }

    exit(0);
}




//tag---------------------------退出进程---------------------------
/**
 *进程终止
 *      进程有5种正常终止和3种异常终止方式
 *      不管进程如何终止，最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等。
 *      即只要进程终止，则其占用的一切资源都会被释放。
 * 
 *      常见4种正常终止方式：
 *          1. 在main函数内执行return语句。等效于调用exit。
 *          2. 调用exit函数。
 *                  exit(3)是标准C库中的一个函数。
 *                  此函数由ISO C定义，其操作包括调用各种终止处理程序(终止处理程序在调用atexit函数时登记)，然后关闭所有标准IO流等。
 *          3. 调用_Exit/_exit函数。_exit(2)则是一个系统调用。
 *                  ISO C定义_Exit，其目的是为进程提供一种无需运行终止处理程序或信号处理程序而终止的方法。  
 *                  在UNIX系统中,_exit函数和_Exit函数是同义的，并不冲洗标准IO流。_exit函数由exit函数调用，它处理UNIX系统特定细节。
 *          4. 进程的最后一个线程结束。
 * 
 *      常见的异常终止：
 *          1. 调用abort。它产生SIGABRT信号，这是下一种异常终止的一种特例。
 *          2. 当进程接收到某些信号时。信号可由进程自身(如调用abort函数)、其它进程或内核产生。例如若进程引用地址空间之外的存储单元、或者除以0，内核就会为该进程产生相应的信号。
 *     
 * 
 *      终止进程如何通知其父进程自己是如何终止的？
 *          对于3个终止函数(exit、_exit、_Exit)，将其退出状态作为参数传送给函数。在异常终止情况，内核(不是进程本身)产生一个
 *          指示其异常终止原因的终止状态。这里使用了"退出状态"和"终止状态"两个术语，以表示有所区别。在最后调用_exit时，内核将
 *          退出状态转换成终止状态。如果子进程正常终止，则父进程可以获得子进程的退出状态。
 * 
 *      如果父进程在子进程之前终止，又将如何？
 *          对于父进程已经终止的所有进程，它们的父进程都改变为init进程。我们称这些进程由init进程收养。
 *          init被编写成无论何时只要有一个子进程终止，init就会调用一个wait函数取得其终止状态。这样就防止了在系统中塞满僵死进程。         
 * 
 *      如果子进程在父进程之前终止，父进程如何获取子进程的终止状态？
 *          内核为每个终止子进程保存了一定量的信息，所以当终止进程的父进程调用wait或waitpid时，可以得到这些信息。
 *          这些信息至少包括进程ID、该进程的终止状态以及该进程使用的CPU时间总量
 *          在UNIX术语中，一个已经终止、但是其父进程尚未对其进行善后处理(获取终止子进程的有关信息、释放它仍占用的资源)的进程被称为僵死进程(zombie)。
 * 
 * 退出函数
 *   #include <stdlib.h>
 *      void exit(int status);
 *      void _Exit(int status);
 *   #include <unistd.h>
 *      void _exit(int status);
 * 
 *      使用不同头文件的原因是exit和_Exit是由ISO C说明的，而_exit是由POSIX.1说明的。
 *      由于历史原因，exit函数总是执行一个标准I/O库的清理关闭操作；对所有打开流调用fclose函数。
 *      main函数返回一个整型值与用该值调用exit是等价的。在main函数中： return(0) == exit(0)
 *  
 *  atexit函数：
 *      按照ISO C规定，一个进程可以登记多至32个函数，这些函数将由exit自动调用。这些函数称为终止处理程序(exit handler)。
 *      exit调用这些函数的顺序与它们登记时候的顺序相反。同一个函数如果登记多次，也会被调用多次。
 *      根据ISO C 和 POSIX.1，exit首先调用各种终止处理程序，然后关闭所有打开流。
 *      POSIX.1扩展了ISO C标准，它说明，如若程序调用exec函数族中的任一函数，则将清除所有已安装的终止处理程序。
 * 
 *      注意，内核使程序执行的唯一方法是调用一个exec函数。进程自愿终止的唯一方法是显示或隐式地(通过调用exit)调用_exit或_Exit。
 *  进程也可非自愿地由一个信号使其终止。
*/

static void atExitHandler1(){
    printf("-->atExitHandler1....\n");
}

static void atExitHandler2(){
    printf("-->atExitHandler2....\n");
}


/**
 * 终止处理程序是由exit来调用执行的
 * 无论exit在何处调用，只要exit被调用，则exit就会执行终止处理程序.
 * 如果进程是因为接收到某个信号而异常终止，则不会执行exit函数。
 * 
*/
void atexitTest(){
    atexit(atExitHandler1);
    atexit(atExitHandler2);
    atexit(atExitHandler2);     //按照注册的相反顺序进行调用，单个函数多次注册会多次调用。
    printf("-->commonFunc start...");
    abort();    //进程接收到信号而异常终止，不执行exit函数。
    _exit(0);   //不执行终止处理程序，不刷新I/O流
    exit(0);   //调用终止处理程序，刷新标准I/O流(fflush(nullptr))。然后调用_exit(0);
}


/**
 * 无缓冲的输出系统API，会直接将内容发送到设备驱动程序进行输出。
 * printf是先将输出内容存储到缓冲区，然后调用系统API将内容输出。缓冲区的缓冲规则一般是无缓冲、行缓冲、全缓冲。
 * 由于缓冲机制的存在，在进程正常结束时会刷新I/O缓冲区。
 * 
*/
void ioFlush(){
    for(int i = 0; i < 10; ++i){
         printf("hello world\n");   //重定向标准输出到文件，标准输出变为全缓冲，即只有缓冲区满才会flush内容到文件中。
         usleep(100 * 1000);
    }
    for(int i = 0; i < 10; ++i){
        write(STDOUT_FILENO, "HELLO ", 6);  //调用系统API的无缓冲输出，输出内容会立即交给内核进行输出。
        usleep(100 * 1000);
    }
    abort();    //异常终止，不会刷新标准I/O缓冲区
    exit(0);    //exit会自动对所有打开的流调用fclose函数，流中缓存的内容会刷新到文件中。
}






//tag---------------------------获取退出进程状态---------------------------
/**
 * 获取进程终止状态：函数wait和waitpid
 *      当一个进程正常或异常终止时，内核就向其父进程发送SIGCHLD信号。因为子进程终止是个异步事件，所以这种信号
 *      也是内核向父进程发的异步通知。父进程可以选择忽略该信号，或者提供一个该信号发生时即被调用执行的函数，即
 *      信号处理程序。对于这种信号系统默认动作是忽略它。
 * 
 *      调用wait或waitpid的进程可能会发生什么：
 *          * 如果其所有子进程都还在运行，则阻塞。
 *          * 如果一个子进程已终止，正等待父进程获取其终止状态，则取得该子进程的终止状态并立即返回。
 *          * 如果它没有任何子进程，则立即出错返回。
 * 
 *      如果进程由于接收到SIGCHLD信号而调用wait，我们期望wait会立即返回。但如果在随机时间点调用wait，则进程可能会阻塞。 
 *      如果status不是一个空指针，则存放终止进程的终止状态。如果不关心终止状态，则可将该参数指定为空指针。
 * 
 *      wait和waitpid返回的整型状态字是由实现定义的。其中某些位表示退出状态(正常返回)，其它位则指示信号编号(异常返回)，有一位指示是否产生了
 *      core文件等。有4个互斥的宏可用来取得进程终止的原因，哪一个宏为真，就可选用其它宏来取得退出状态、信号编号等。
 *      
 * pid_t wait(int *status);
 *      成功：返回清理掉的子进程ID； 失败：-1 (没有子进程)
 *      阻塞等待，直到有子进程结束；如果没有子进程则返回-1。
 *      对于wait，其唯一的出错是调用进程没有子进程(函数调用被一个信号中断时，也可能返回另一种出错)。
 *
 * pid_t waitpid(pid_t pid, int *status, in options);
 *      成功： 返回清理掉的子进程ID；    失败： -1(无子进程)；   0：options为WNOHANG, 子进程正在运行中
 *      pid：指定要回收状态的进程的ID
 *      opitons：可以指定不阻塞；可以指定关心处于停止状态的进程；可以指定关心停止后又继续的进程；
 *      对于waitpid，如果指定的进程或进程组不存在，或者参数pid指定的进程不是调用进程的子进程，都可能出错。    
 * 
 *      参数：
 *          pid：
 *              >0：回收指定ID的子进程
 *              -1：回收任意子进程（相当于 wait）
 *          status:
 *               接收进程返回状态
 *          options: 或者是0，或者是以下常量按位或运算的结果。
 *              0:          阻塞等待
 *              WNOHANG:    不阻塞
 *              WUNTRACED:  进程处于停止状态，并且其状态自停止以来还未报告过，则返回其状态。
 *              WCONTINUED: 子进程在停止后已经继续，但其状态尚未报告，则返回其状态
 *          
 *
 *      判断进程返回状态的宏函数
 *          WIFEXITED(status)   为非 0 → 进程正常结束
 *          WEXITSTATUS(status) 如上宏为真, 获取进程退出状态 (子进程传送给exit或_exit参数的低8位)
 *
 *          WIFSIGNALED(status) 为非 0 → 进程异常终止
 *          WTERMSIG(status)    如上宏为真， 使用此宏 → 取得使进程终止的那个信号的编号。
 *
 *          WIFSTOPPED(status)  为非 0 → 进程处于暂停状态
 *          WSTOPSIG(status)    如上宏为真， 使用此宏 → 取得使进程暂停的那个信号的编号。
 * 
 *          WIFCONTINUED(status) 非0 → 作业控制暂停后已经继续的子进程返回了状态
 */

//打印回收的子进程的终止状态
void pr_exit(int status){
    if(WIFEXITED(status)){  //正常退出，打印退出状态
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

    }else if(WIFSIGNALED(status)){  //异常退出，打印造成退出的信号
        printf("abnormal termination, signal<%s> %s\n", strsignal(WTERMSIG(status)),
        #ifdef WCOREDUMP
            WCOREDUMP(status) ? "(core file generated)" : "");
        #else
            "");
        #endif

    }else if(WIFSTOPPED(status)){   //进程暂停，打印造成暂停的信号
        printf("child stopped, signal<> = %s\n", strsignal(WSTOPSIG(status)));

    }else if(WIFCONTINUED(status)){
        printf("child continued....\n");
    }
}


/**
 * 父进程创建多个子进程，子进程执行自己的逻辑
 * 父进程阻塞等待，按序回收子进程
*/
void multifork_waitpid(){
    std::vector<int> waitPidVec;
    for(int count = 0; count < 5; count++){
        int pid = fork();   //只有在父进程的逻辑里调用fork，所有的fork创建的进程都是父进程的子进程
        if(pid == -1){   
            perror("fork error, parent process exit....");  
            exit(-1);

        }else if(pid == 0){ //子进程
            cout << "i am " << count << " child, my pid == " << getpid() << " my parentpid == " << getppid() << endl;
            sleep(count);
            if(count < 3){
                abort();    //异常终止
            }else{
                exit(count);    //正常终止
            }

        }else if(pid > 0){  //父进程
            waitPidVec.push_back(pid);
        }
    }

    cout << "i am parent, my pid == " << getpid() << endl;
    int cpid, status;
    for(auto pid : waitPidVec){     //按序回收子进程
        cpid = waitpid(pid, &status, 0);
        if(cpid > 0){
            std::cout << cpid << " : " << std::endl;
            pr_exit(status);
        }else if(cpid == -1){
            std::cout << "parent complete...." << std::endl;
            return;
        }
    }
}



/**
 * 父进程创建一个子进程，子进程再创建自己的子进程
 * 如果子进程在自己的子进程之前结束，则其子进程成为孤儿进程，被init进程收养，由init进程负责回收
 * 父进程只能回收自己创建的子进程，孤儿进程由init进程负责回收
*/
void process_adoption(){
    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){ //子进程
        if((pid = fork()) < 0){
            printf("fork error\n");
            exit(-1);
        }else if (pid == 0){ //子进程的子进程
            sleep(2);
            printf("child child exit, pid = %d, ppid = %d\n", getpid(), getppid());
            exit(0);
        } 
        sleep(1);
        printf("child exit: pid = %d\n", getpid());
        exit(10);
    } 

    int status;
    if(waitpid(pid, &status, 0) != pid){ //回收创建的子进程
        printf("waitpid error\n");
        exit(-1);
    }
    printf("child is collected, pid = %d\n", pid);
    pr_exit(status);
    exit(0);
}



/**
 * 竞争条件：进程之间的同步
 *      如果在fork之后的某种逻辑显式或隐式地依赖于在fork之后是父进程先运行还是子进程先运行，此即产生了竞争。
 *      采用sleep来同步进程时序是不可靠的，如果系统的负载很重，sleep不能保证期望的时序。
 *      为了避免竞争条件和轮询，在多个进程之间需要有某种形式的信号发送和接收的办法。类似线程之间的同步。
*/


//tag---------------------------创建进程中加载新可执行文件---------------------------
/**
 * 如何在一个进程中运行新的程序？
 * 需指定的参数：
 *      可执行文件名称：程序的绝对路径或程序名(借助PATH环境变量)
 *      命令行参数：argv[0] .... argv[n] nullptr
 *      环境表指针：environ或定制的环境表。 新进程会复制指定的环境表，形成自己专有的环境表。
 *
 * exec函数族
 *      int execl(const char *path, const char *arg, ...);
 *      int execlp(const char *file, const char *arg, ...);
 *      int execle(const char *path, const char *arg, ..., char *const envp[]);
 * 
 *      int execv(const char *path, char *const argv[]);
 *      int execvp(const char *file, char *const argv[]);
 *      int execve(const char *path, char *const argv[], char *const envp[]);
 * 
 *      execl后，重新加载进程的虚拟地址空间，跳到新程序的入口地址进行执行，执行完毕后进程退出。
 *      execl失败才会返回。基本不会返回。
 *
 * 
 *      当进程调用一种exec函数时，该进程执行的程序完全替换为新程序，而新程序则从其main函数开始执行。因为调用exec并不创建新进程，
 * 所以前后的进程ID并未改变。exec只是用磁盘上的一个新程序替换了当前进程的正文段、数据段、堆段和栈段。
 * 
 * UNIX系统进程控制原语：
 *      fork：创建新进程
 *      exec：加载执行新程序
 *      exit：退出进程
 *      wait：回收内核保存的进程终止状态信息
 * 
 *      如果exec找到了一个可执行文件，但是该文件不是由链接编辑器产生的机器可执行文件，则就认为该文件是一个shell脚本，于是试着调用/bin/sh，
 * 并以该filename作为shell输入。 
 * 
 * 每个进程拥有独立的环境变量
 *      以e结尾的3个函数(execle, execve, fexecve)可以传递一个指向环境字符串指针数组的指针。其它4个函数则使用调用进程中的environ变量
 * 为新程序复制现有的环境。即父进程可以更改当前环境和后面生成的子进程的环境，但是不能影响自己的父进程的环境。通常，一个进程允许将其环境
 * 传播给其子进程，但有时也有这种情况，进程想要为子进程指定某一个确定的环境。例如，在初始化一个新登录的shell时，login程序通常创建一个只
 * 定义少数几个变量的特殊环境，而在我们登录时，可以通过shell启动文件，将其它变量加载到环境中。
 * 
 *      进程中每个打开描述符都有一个执行时关闭标志。若设置了此标志，则在执行exec时关闭该描述符；否则该描述符仍打开。除非特地用fcntl设置
 * 了该执行时关闭标志，否则系统的默认操作是在exec后仍保持这种描述符打开。
 * 
 *      在exec前后实际用户ID和实际组ID保持不变，而有效ID是否改变则取决于所执行程序文件的设置用户ID位和设置组ID位是否设置。如果新程序的
 * 设置用户ID位已设置，则有效用户ID变成程序文件所有者的ID；否则有效用户ID不变。对组ID的处理方式与此相同。
 * 
 *      可执行程序的第一个参数(新程序中的argv[0])设置为路径名的文件名分量。某些shell将此参数设置为完全的路径名。这只是一个惯例。我们可
 * 将argv[0]设置为任何字符串。
 */


/**
 * 父进程创建一个子进程，子进程加载新程序执行，父进程回收子进程的退出状态。
*/
char* const lsArgument[] = { (char*)"ls", (char*)"-l", nullptr};
int fork_exec(int option){
    pid_t pid = fork();     //通知内核创建一个新进程
    if(pid == 0){           //子进程执行单元
        cout << "i am child process, my pid == " << getpid() << endl;
        switch(option){
            case 0:
                execl("/usr/bin/pwd", "pwd", nullptr);  //绝对路径 + 默认环境变量数组environ
                perror("error");
                break;
            case 1:
                execlp("ls", "ls", "-l", nullptr);  //相对路径 + 默认环境变量数组environ      
                perror("error");
                break;
            case 2:
                execle("/usr/bin/ls", "ls", "-l", nullptr, environ);    //绝对路径 + 指定环境变量数组
                perror("error");
                break;
            case 3:
                execv("/bin/ls", lsArgument);   //绝对路径 + 默认环境变量数组environ
                perror("error");
                break;
            case 4:
                execvp("ls", lsArgument);   //相对路径 + 默认环境变量数组environ
                perror("error");
                break;
            case 5:
                execve("/bin/ls", lsArgument, environ); //绝对路径 + 指定环境变量数组
                perror("error");
                break;
            default:
                cout << "wrong option number" << endl;
        }
    }

    cout << "i am parent, pid = " << getpid() << endl;
    int status, cpid;
    cpid = wait(&status);  //父进程回收子进程
    if(cpid > 0){  
        printf("%d is collected....\n", cpid); 
        pr_exit(status);    
    }
    std::cout << "parent exit, no child process needed to adopt...." << std::endl;
    return 0;
}


/**
 *  (1) 每个进程拥有独立的进程环境表，每个进程的environ指向本进程的环境表
 *  (2) 进程环境表的复制发生在exec执行时，因此更改环境表只能影响后续创建的子进程，不能影响已创建的子进程和自己的父进程的环境表。
*/
int exec_environ_copy(){
    string parentPathBefore(getenv("PATH"));
    std::cout << "parentPathBefore: " << parentPathBefore << std::endl;

    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){
        char* env_init[] = {(char*)"USER=unknown", (char*)"PATH=/TMP", nullptr};    //构造环境表
        //加载新程序时，会将环境表复制到子进程环境表空间，并使子进程的environ指向此环境表。
        if(execle("/home/lhy/ownproject/stl/out/bin/echoall", "echoall first", "myarg1", "my arg2", nullptr, env_init) == -1){
            printf("execle error\n");
            exit(-1);
        }
    }

    sleep(1);
    wait(nullptr);
    string parentPathAfter(getenv("PATH"));
    std::cout << "parentPathAfter: " << parentPathAfter << std::endl;

    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){
        string path(getenv("PATH"));
        path.append(":/home/lhy/ownproject/stl/out/bin");
        setenv("PATH", path.c_str(), 1);
        //默认传递父进程的environ，复制父进程的环境表
        if(execlp("echoall", "echoall second", "arg1", nullptr)  == -1){
            printf("execle error\n");
            exit(-1);
        }
    }
    exit(0);
}



//tag---------------------------解释器文件---------------------------
/**
 * exec解释器文件时的命令行参数
 *      exec函数族可以加载处理解释器文件，但是要求该解释器文件具有执行权限。
 * 
 *      所有现今的UNIX系统都支持解释器文件。这种文件是文本文件，其起始行的形式：#! pathname [optional-argument]; pathname通常是绝对路径名，
 *      对它不进行什么特殊处理(不使用PATH进行路径搜索)。对这种文件的识别是由内核作为exec系统调用处理的一部分来完成的。内核使调用exec函数的进程
 *      实际执行的并不是该解释器文件，而是在该解释器文件第一行中pathname所指定的程序。一定要将解释器文件和解释器区分开。  
 * 
 *      当内核exec解释器时，argv[0]是该解释器的pathname，argv[1]是解释器文件中的可选参数，其余参数是execl解释器文件时指定的参数。
*/
void interpret(){
    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){
        if(execl("/home/lhy/ownproject/stl/unix/05process/testinterp", "estinterp", "arg1", "arg2", nullptr) < 0){
            printf("execl error\n");
            exit(-1);
        }
    }

    wait(nullptr);
    exit(0);
}


//tag---------------------------system函数---------------------------
/**
 * 在UNIX中，system总是可用的。
 * int system(const char* cmdstring);
 * system在其实现中调用了fork、exec和waitpid，因此具有3种返回值：
 *      1. fork失败或者waitpid返回除EINTR之外的出错，则system返回-1，并且设置errno以指示错误类型。
 *      2. 如果exec失败(表示不能执行shell)，其返回值如同shell执行了exit(127).
 *      3. 否则所有3个函数都执行成功，那么system的返回值是shell的终止状态。
 * 
 * 
 * system函数的一个实现
 * 调用进程创建子进程，子进程加载shell解释器去执行具体的任务
 * 如果调用进程执行fork或者waitpid时发生错误，则返回-1。否则返回回收到的子进程的终止状态。
*/
int system_implement(const char* cmdstring){
    pid_t pid;
    int status;

    //返回非0值，表示支持system函数
    if(cmdstring == nullptr){
        return 1;
    }

    if((pid = fork()) < 0){
        perror("fork error");
        status = -1;
    }else if(pid == 0){ //子进程中加载shell程序，执行命令行指令
        execl("/usr/bin/bash", "bash", "-c", cmdstring, nullptr);
        perror("execl error");
        _exit(127);    
    }  

    /**
     * 等待回收子进程
     * 故system一定是在命令执行完毕后返回
    */
    while(waitpid(pid, &status, 0) < 0){    //waitpid被中断后会再次调用
        if(errno != EINTR){
            status = -1;
            break;
        }
    }
    
    /**
     * fork或者waitpid失败时，返回-1
     * execl失败时，返回的子进程的退出状态是127
     * 都成功时，返回回收的子进程的终止状态
    */
    return status;
}



//tag---------------------------用户ID、组ID---------------------------
/**
 * 用户ID、组ID
 *      在UNIX系统中，特权以及访问控制(如能否读写一个特定文件)，是基于用户ID和组ID的。一般而言，在设计应用时,
 *总是试图使用最小特权模型。依照此模型，我们的程序应当只具有为完成给定任务所需的最小特权。这降低了由恶意用户试
 *图哄骗我们的程序以未预料的方式使用特权造成的安全性风险。
 *
 * int setuid(uid_t uid);
 *      若进程具有超级用户特权，则setuid函数将实际用户ID、有效用户ID以及保存的设置用户ID设置为uid。
 *      若进程没有超级用户特权，但是uid等于实际用户ID或保存的设置用户ID，则setuid只将有效用户ID设置为uid。
 *      若对程序文件设置了设置用户ID位时，exec函数才设置有效用户ID，保存的设置用户ID是由exec复制有效用户ID而得到的。
 *     
 * int seteuid(uid_t uid);
 *      特权用户：可将有效用户ID设置为uid。（这区别于setuid函数，它更改所有3个用户ID）
 *      普通用户：将有效用户ID设置为其实际用户ID或其保存的设置用户ID。
 * 
 *fork()创建子进程，子进程继承父进程的实际用户ID、有效用户ID、保存的设置用户ID。 
*/

// 打印进程的用户ID、组ID、保存的设置用户ID(saved set-user-ID)
void printUserId(const char* message){
    uid_t realUserId, effectiveUserId, saveSetUserId;
    getresuid(&realUserId, &effectiveUserId, &saveSetUserId);
    printf("%s > realUserId: %u, effectiveUserId: %u, saveSetUserId: %u\n", message, realUserId, effectiveUserId, saveSetUserId);
}

// 获取进程的用户ID、组ID、保存的设置用户ID(saved set-user-ID)
uid_t getsuid(){
    uid_t realUserId, effectiveUserId, saveSetUserId;
    getresuid(&realUserId, &effectiveUserId, &saveSetUserId);
    return saveSetUserId;
}

/**
 * 执行时先将可执行程序的文件所有者更改为root，执行权限设置为所有人可执行。置位设置用户ID位
 * 以普通用户账号运行程序
*/
void processid(){
    /**
     * ruid:    普通用户
     * euid:    可执行程序的所有者
     * suid:    可执行程序的所有者
     * exece加载时设置
    */
    printUserId("1--");

    /**
     * 将有效用户ID设回为实际用户ID，降低用户特权，保存的设置用户ID不变
     * ruid:    普通用户
     * euid:    普通用户
     * suid:    可执行程序的所有者
    */
    seteuid(getuid());  //设置为实际用户ID
    printUserId("2--");

    /**
     * 将有效用户ID恢复为保存的设置用户ID，回复程序特权
     * ruid:    普通用户
     * euid:    可执行程序的所有者
     * suid:    可执行程序的所有者
    */
    seteuid(getsuid());     //设置为保存的设置用户ID
    printUserId("3--");

    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 == 0){
        //子进程继承父进程的实际用户ID、有效用户ID、设置的保存用户ID
        printUserId("child 1--");
        exit(0);
    }
    sleep(1);

    /**
     * 进程的有效用户ID为root对应的ID，此时setuid会将实际用户ID、有效用户ID、设置的保存用户ID都设为uid。
    */
    setuid(getuid());
    printUserId("parent--");
    pid2 = fork();
    if(pid2 == 0){
        printUserId("child 2--");
        exit(0);
    }
    sleep(1);

    pid_t wid;
    while(true){
        if((wid = wait(nullptr)) < 0){
            break;
        }
    }
}



int main(int argc, char* argv[]){

    ioFlush();

    return 0;
}


