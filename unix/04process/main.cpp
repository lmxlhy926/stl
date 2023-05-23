
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

/*
 * 本质：char *environ[] = {"HOME=/home/itcat", "SHELL=/bin/bash", ....}
 * 所有的环境变量都存在于environ这个数组中,下面的三个函数都是针对与该数组进行操作。
 *
 * 三个函数定义于<stdlib.h>中
 * char *getenv(const char *name);
 * 成功： 返回环境变量的值； 失败： NULL (name 不存在)
 *
 * int setenv(const char *name, const char *value, int overwrite);
 * 成功： 0； 失败： -1
 * 参数 overwrite 取值：
 *      0： 不覆盖。 (该参数常用于设置新环境变量， 如： ABC = haha-day-night)
 *      1： 覆盖原环境变量
 *
 * int unsetenv(const char *name);
 * 成功： 0； 失败： -1
 * 注意事项： name 不存在仍返回 0(成功)， 当 name 命名为"ABC="时则会出错。
 *
 */


//打印环境变量数组
int printEnviron(){
    for(int i = 0; environ[i] != nullptr; ++i){
        printf("%s\n", environ[i]);
    }
    return 0;
}

//获取环境变量的值
char * getEnvTest(const char *name)
{
    char *p = nullptr;
    for(int i = 0; environ[i] != nullptr; ++i){
        p = strstr(environ[i], "=");    //定位到'='字符
        int len = p - environ[i];       //变量名长度
        if(strncmp(name, environ[i], len) == 0){    //比较变量名长度
            return p+1; //返回值的地址地址
        }
    }
    return nullptr;
}

//设置环境变量
void setUnsetEnv(const char* name, const char* setValue){
    char *value;
    if((value = getEnvTest(name)) != nullptr){
        printf("value = %s\n", value);
    }else{
        printf("%s is not set...\n", name);
    }

    setenv(name, setValue, 0);
    printEnviron();
}


/*
 * 如何在一个进程中启动其它的程序？
 * 需要的参数：程序的名称(绝对路径、相对路径), 程序的参数(列表、数组)
 *
 * 可执行程序名（相对位置/绝对位置）, PATH环境变量, 整个环境变量environ
 * 参数形式：list vector
 * 环境变量：PATH environ
 *
 * int execl(const char *path, const char *arg, ...);
 * int execlp(const char *file, const char *arg, ...);
 * int execle(const char *path, const char *arg, ..., char *const envp[]);
 *
 * vector path environ
 * int execv(const char *path, char *const argv[]);
 * int execvp(const char *file, char *const argv[]);
 * int execve(const char *path, char *const argv[], char *const envp[]);
 *
 */

char* const lsArgument[] = {(char*)"ls", (char*)"-l", nullptr};
int fptExec(int option)
{
    pid_t pid = fork();
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
        int status;
        int waitedPid = wait(&status);
        if(waitedPid > 0){
            if(WIFEXITED(status)){
                std::cout << "child is exit with status " << WEXITSTATUS(status) << std::endl;
            }else if(WIFSIGNALED(status)){
                std::cout << "child is signaled by " << WTERMSIG(status) << std::endl;
            }else if(WIFSTOPPED(status)){
                std::cout << "child is stopped by " << WSTOPSIG(status) << std::endl;
            }
        }else{
            printf("------>parent process end-----\n");
        }
    }
    return 0;
}


/*
 * fork之后
 * 父子相同处：.data .text 堆 栈 环境变量 用户id 宿主目录 进程工作目录 信号处理方式
 * 父子不同处：1.进程id  2.fork返回值  3.父进程Id  4.进程运行时间  5.闹钟（定时器）  6.未决信号集
 * 注：相同的意思是值相同,不代表物理存储空间相同
 * 
 * 父子进程之间遵循读时共享写时复制的原则，目的是节省内存开销，逻辑上各个进程的变量之间是独立的。
 */
int globalVar = 6;  //数据段变量
char buf[] = "a write to stdout\n";     

void forkTest(){
    int var;    //栈变量
    pid_t pid;

    var = 88;
    write(STDOUT_FILENO, buf, sizeof(buf) - 1);
    printf("before fork\n");

    if((pid = fork()) < 0){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){  //子进程
        globalVar++;
        var++;
    }else{  //父进程
        sleep(2);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), globalVar, var);
    exit(0);
}


/*
 * pid_t wait(int *status);
 * 成功：返回清理掉的子进程ID； 失败：-1 (没有子进程)
 *
 * pid_t waitpid(pid_t pid, int *status, in options);
 * 成功： 返回清理掉的子进程ID；    失败： -1(无子进程)；   0：options为WNOHANG, 子进程正在运行中
 * 参数：
 *      pid：
 *          > 0：回收指定 ID 的子进程
 *           -1：回收任意子进程（相当于 wait）
 *      status:
 *          接收进程返回状态
 *      options:
 *          0: 阻塞等待
 *          WNOHANG: 不阻塞
 *
 * 判断进程返回状态的宏函数
 *      WIFEXITED(status) 为非 0 → 进程正常结束
 *      WEXITSTATUS(status) 如上宏为真， 使用此宏 → 获取进程退出状态 (exit 的参数)
 *
 *      WIFSIGNALED(status) 为非 0 → 进程异常终止
 *      WTERMSIG(status) 如上宏为真， 使用此宏 → 取得使进程终止的那个信号的编号。
 *
 *      WIFSTOPPED(status) 为非 0 → 进程处于暂停状态
 *      WSTOPSIG(status) 如上宏为真， 使用此宏 → 取得使进程暂停的那个信号的编号。
 */
void fpt_wait(){
    for(int index = 0; index < 5; index++){
        std::cout << "-->parent start to fork...." << std::endl;
        int pid = fork();
        if(pid == -1){  
            perror("fork error");
            exit(-1);
        }else if(pid == 0){ //子进程
            cout << "i am " << index << " child, my pid == " << getpid() << " my parentpid == " << getppid() << endl;
            sleep(3);
            exit(index);
        }
    }

    cout << "i am parent, my pid == " << getpid() << endl;
    while(true){
        int status;
        int handlePid = waitpid(-1, &status, 0);
        if(handlePid > 0){
            if(WIFEXITED(status)){
                cout << "child exit with status " << WEXITSTATUS(status) << endl;
            }else if (WIFSIGNALED(status)){
                cout << "child is killed by " << WTERMSIG(status) << endl;
            }else if(WIFSTOPPED(status)){
                cout << "child is stopped by " << WSTOPSIG(status) << endl;
            }
        }else if(handlePid == -1){
            std::cout << "parent complete...." << std::endl;
            break;
        }
    }
}



int main(int argc, char* argv[]){
    fptExec(3);


    return 0;
}







