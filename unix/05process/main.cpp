
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

/**
 * UNIX系统的进程控制，包括创建新进程、执行程序和进程终止。
 * 
 * 进程标识：
 *      每个进程都有一个非负整型表示的唯一进程ID。
 *      ID为0进程通常是调度进程，常常被称为交换进程，是内核的一部分；
 *      进程ID1通常是init进程，在自举过程结束时由内核调用。init进程绝对不对终止，是所有孤儿进程的父进程。
*/

/**
 * fork:
 *      子进程是父进程的副本。子进程获得父进程数据空间、堆和栈的副本。注意，这是子进程所拥有的副本。
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
*/

/**
 * 父子进程的执行顺序：
 *      一般来说，fork之后是父进程先执行还是子进程先执行是不确定的，这取决于内核所用的调度算法。
 *      如果要求父进程和子进程之间相互同步，则要求某种形式的进程间通信。
 *      fork之后可以使用信号使父进程和子进程同步。
 * 
 * fork时如何处理标准IO库的缓冲区内容：
 *      取决于fork时，标准IO库的缓冲区内容是否被冲洗。如果已被冲洗，则没得复制；否则，复制到子进程的数据空间中。
 * 
 *      标准输出行缓冲：
 *          如果标准输出连接到终端设备，则它是行缓冲的；否则它是全缓冲的。
 *          当以交互方式运行该程序时，只得到该printf输出的行一次，其原因是标准输出缓冲区由换行符冲洗。
 *      标准输出全缓冲：
 *          但是将标准输出重定向到一个文件时，却得到printf输出行两次。其原因是，在fork之前调用了printf一次，但当调用fork时，
 *          该行数据仍在缓冲区中，然后在将父进程数据空间复制到子进程中时，该缓冲区数据也被复制到子进程中，此时父进程和子进程各自有
 *          了带该行内容的缓冲区。
 *      
 *      即取决于fork时，标准IO库的缓冲区内容是否被冲洗。如果已被冲洗，则没得复制；否则，复制到子进程的数据空间中。
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
*/

//fork后父子进程的数据空间彼此独立，不会相互影响；
int globalVar = 6;  //.data区域
string out =  "a write to stdout\n";     
void forkTest(){
    int var = 88;    //栈变量
    write(STDOUT_FILENO, out.data(), out.length());     //无缓冲写
    printf("before fork\n");    //标准库带缓冲写；行缓冲或全缓冲，取决于标准输出对应的文件。
    // fflush(stdout);

    pid_t pid;
    if((pid = fork()) < 0){ //创建新进程
        perror("fork error");
        exit(-1);
    }else if(pid == 0){  //子进程
        globalVar++;
        var++;
        sleep(1);
    }else{  //父进程
        sleep(3);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), globalVar, var);
    exit(0);
}


/**
 *进程终止
 *      进程有5种正常终止和3种异常终止方式
 *      不管进程如何终止，最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等。
 * 
 *      常见4种正常终止方式：
 *          1. 在main函数内执行return语句。等效于调用exit。
 *          2. 调用exit函数。此函数由ISO C定义，其操作包括调用各种终止处理程序(终止处理程序在调用atexit函数时登记)，然后关闭所有标准IO流等。
 *          3. 调用_Exit函数。ISO C定义_Exit，其目的是为进程提供一种无需运行终止处理程序或信号处理程序而终止的方法。
 *             在UNIX系统中,_exit函数和_Exit函数是同义的，并不冲洗标准IO流。_exit函数由exit函数调用，它处理UNIX系统特定细节。
 *             exit(3)是标准C库中的一个函数，而_exit(2)则是一个系统调用。
 *          4. 进程的最后一个线程结束。
 * 
 *      常见的异常终止：
 *          1. 调用abort。它产生SIGABRT信号，这是下一种异常终止的一种特例。
 *          2. 当进程接收到某些信号时。信号可由进程自身(如调用abort函数)、其它进程或内核产生。
 *             例如若进程引用地址空间之外的存储单元、或者除以0，内核就会为该进程产生相应的信号。
 *     
 *      不管进程如何终止，最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等。
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
*/

/**
 * 获取进程终止状态：函数wait和waitpid
 *      当一个进程正常或异常终止时，内核就向其父进程发送SIGCHLD信号。因为子进程终止是个异步事件，所以这种信号
 *      也是内核向父进程发的异步通知。父进程可以选择忽略该信号，或者提供一个该信号发生时即被调用执行的函数，即
 *      信号处理程序。对于这种信号系统默认动作时忽略它。
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
 *              >0：回收指定 ID 的子进程
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
 *      
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

    }else if(WIFCONTINUED(status)){
        printf("child continued....\n");
    }
}



/**
 * 父进程创建一个子进程，子进程再创建自己的子进程
 * 如果子进程在自己的子进程之前结束，则其子进程成为孤儿进程，被init进程收养，由init进程负责回收
 * 父进程只能回收自己创建的子进程
*/
void processAdoption(){
    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){ //创建的第一个子进程
        if((pid = fork()) < 0){
            printf("fork error\n");
            exit(-1);
        }else if(pid > 0){ //第一个子进程退出
            sleep(1);
            exit(10);
        }else if (pid == 0){ //第一个子进程创建的子进程
            sleep(2);
            printf("second child, parent pid = %d\n", getppid());
            exit(0);
        }

    }else if (pid > 0){ //父进程
        int status;
        if(waitpid(pid, &status, 0) != pid){ //回收创建的子进程
            printf("waitpid error\n");
            exit(-1);
        }else{
            printf("first child, pid = %d\n", pid);
            pr_exit(status);
            exit(0);
        }
    }
}


//一个父进程创建多个子进程，子进程执行完逻辑后退出，父进程回收子进程状态。
void fpt_wait(){
    for(int index = 0; index < 5; index++){
        int pid = fork();
        if(pid == -1){  
            perror("fork error, parent process exit....");
            exit(-1);
        }else if(pid == 0){ //子进程
            cout << "i am " << index << " child, my pid == " << getpid() << " my parentpid == " << getppid() << endl;
            sleep(3);
            if(index < 3){
                abort();
            }else{
                exit(index);
            }
        }
    }

    cout << "i am parent, my pid == " << getpid() << endl;
    int status, waitPid;
    while(true){
        waitPid = waitpid(-1, &status, 0);
        if(waitPid > 0){
            pr_exit(status);
        }else if(waitPid == -1){
            std::cout << "parent complete...." << std::endl;
            break;
        }
    }
}


/**
 * 竞争条件：进程之间的同步
 *      如果在fork之后的某种逻辑显式或隐式地依赖于在fork之后是父进程先运行还是子进程先运行，此即产生了竞争。
 *      采用sleep来同步进程时序是不可靠的，如果系统的负载很重，sleep不能保证期望的时序。
 *      为了避免竞争条件和轮询，在多个进程之间需要有某种形式的信号发送和接收的办法。类似线程之间的同步。
*/



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
 * 当进程调用一种exec函数时，该进程执行的程序完全替换为新程序，而新程序则从其main函数开始执行。因为调用exec并不创建新进程，
 * 所以前后的进程ID并未改变。exec只是用磁盘上的一个新程序替换了当前进程的正文段、数据段、堆段和栈段。
 * 
 * UNIX系统进程控制原语：
 *      fork：创建新进程
 *      exec：加载执行新程序
 *      exit：退出进程
 *      wait：回收内核保存的进程终止状态信息
 * 
 * 如果exec找到了一个可执行文件，但是该文件不是由链接编辑器产生的机器可执行文件，则就认为该文件是一个shell脚本，于是试着调用/bin/sh，
 * 并以该filename作为shell输入。 
 * 
 * 以e结尾的3个函数(execle, execve, fexecve)可以传递一个指向环境字符串指针数组的指针。其它4个函数则使用调用进程中的environ变量为新
 * 程序复制现有的环境。即父进程可以更改当前环境和后面生成的子进程的环境，但是不能影响自己的父进程的环境。通常，一个进程允许将其环境传播
 * 给其子进程，但有时也有这种情况，进程想要为子进程指定某一个确定的环境。例如，在初始化一个新登录的shell时，login程序通常创建一个只定义
 * 少数几个变量的特殊环境，而在我们登录时，可以通过shell启动文件，将其它变量加载到环境中。
 * 
 * 进程中每个打开描述符都有一个执行时关闭标志。若设置了此标志，则在执行exec时关闭该描述符；否则该描述符仍打开。除非特地用fcntl设置了该执行时
 * 关闭标志，否则系统的默认操作是在exec后仍保持这种描述符打开。
 * 
 * 在exec前后实际用户ID和实际组ID保持不变，而有效ID是否改变则取决于所执行程序文件的设置用户ID位和设置组ID位是否设置。如果新程序的设置用户ID位
 * 已设置，则有效用户ID变成程序文件所有者的ID；否则有效用户ID不变。对组ID的处理方式与此相同。
 * 
 * 
 */

// 父进程创建一个子进程，子进程加载新程序执行，父进程回收子进程的退出状态。
char* const lsArgument[] = { (char*)"ls", (char*)"-l", nullptr};
int forkExec(int option)
{
    pid_t pid = fork();     //通知内核创建一个新进程
    if(pid == 0){   //子进程执行单元
        cout << "i am child process, my pid == " << getpid() << endl;
        switch(option){
            case 0:
                execl("/usr/bin/pwd", "pwd", nullptr);  //绝对路径
                perror("error");
                break;
            case 1:
                execlp("ls", "ls", "-l", nullptr);  //相对路径      
                perror("error");
                break;
            case 2:
                execle("/usr/bin/ls", "ls", "-l", nullptr, environ);    //绝对路径 + 环境变量数组
                perror("error");
                break;
            case 3:
                execv("/bin/ls", lsArgument);
                perror("error");
                break;
            case 4:
                execvp("ls", lsArgument);
                perror("error");
                break;
            case 5:
                execve("/bin/ls", lsArgument, environ);
                perror("error");
                break;
            default:
                cout << "wrong option number" << endl;
        }

    }else if(pid > 0){  //父进程执行单元
        cout << "i am parent, pid = " << getpid() << endl;
        int status, waitPid;
        waitPid = wait(&status);  //父进程回收子进程
        if(waitPid > 0){
          pr_exit(status);
        }else{
            std::cout << "parent exit, no child process needed to adopt...." << std::endl;
        }
    }
    return 0;
}

//只能影响当前环境和后面生成的子进程的环境，不能影响前面生成的子进程的环境和父进程的环境
//因为环境表复制操作发生在exce执行时
int forkExec1(){
    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){
        //构造环境表
        char* env_init[] = {(char*)"USER=unknown", (char*)"PATH=/TMP", nullptr};
        //加载新程序时，会将环境表复制到子进程环境表空间，并使子进程的environ指向此环境表。
        if(execle("/home/lhy/ownproject/stl/out/bin/echoall", "echoall", "myarg1", "my arg2", nullptr, env_init) == -1){
            printf("execle error\n");
            exit(-1);
        }
    }

    if(waitpid(pid, nullptr, 0) < 0){
        printf("waitpid error\n");
        exit(-1);
    }

    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){
        string path(getenv("PATH"));
        path.append(":/home/lhy/ownproject/stl/out/bin");
        setenv("PATH", path.c_str(), 1);
        //默认传递父进程的environ，复制父进程的环境表
        if(execlp("echoall", "echoall", "only 1 arg", nullptr)  == -1){
            printf("execle error\n");
            exit(-1);
        }
    }

    exit(0);
}

// 打印进程的用户ID、组ID、保存的设置用户ID(saved set-user-ID)
void printUserId(const char* message){
    uid_t realUserId, effectiveUserId, saveSetUserId;
    getresuid(&realUserId, &effectiveUserId, &saveSetUserId);
    printf("%s > realUserId: %u, effectiveUserId: %u, saveSetUserId: %u\n", message, realUserId, effectiveUserId, saveSetUserId);
}

// 获取进程的用户ID、组ID、保存的设置用户ID(saved set-user-ID)
uid_t getUserId(const char* option){
    uid_t realUserId, effectiveUserId, saveSetUserId;
    getresuid(&realUserId, &effectiveUserId, &saveSetUserId);
    if(strcmp(option, "real") == 0){
        return realUserId;
    }else if(strcmp(option, "efftive") == 0){
        return effectiveUserId;
    }else if(strcmp(option, "save") == 0){
        return saveSetUserId;
    }
    return -1;
}

/**
 * 执行时先将可执行程序的文件所有者更改为root，执行权限设置为所有人可执行。置位设置用户ID位
 * 以普通用户账号运行程序
*/
void processid(){
    //实际用户id为普通用户，有效用户id为可执行程序的所有者，保存的设置用户ID位为可执行程序的所有者
    printUserId("1--");

    //设置有效用户ID为实际用户ID，降低程序的特权
    seteuid(getUserId("real"));
    printUserId("2--");

    //将有效用户ID恢复为保存的设置用户ID，回复程序特权
    seteuid(getUserId("save"));
    printUserId("3--");

    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(-1);
    }else if(pid == 0){
        //子进程继承父进程的实际用户ID、有效用户ID、设置的保存用户ID
        printUserId("child 1--");

        //子进程的有效用户ID为root对应的ID，此时setuid会将实际用户ID、有效用户ID、设置的保存用户ID都设为uid。
        setuid(getUserId("real"));
        printUserId("child 2--");


        exit(100);
    }

    int status;
    pid_t waitPid = waitpid(pid, &status, 0);
    if(waitPid == pid){
        pr_exit(status);
    }
}

int main(int argc, char* argv[]){
    processid();

    return 0;
}







