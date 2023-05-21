#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <string>
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

char * const argv[] = {const_cast<char *>("ls"), const_cast<char *>("-l"), nullptr};
int fptExec(int option)
{
    pid_t pid = fork();
    if(pid == 0){   //子进程执行单元
        cout << "i am child process, my pid == " << getpid() << endl;
        switch(option){
            case 0:
                execl("/bin/pwd", "pwd", nullptr);
                perror("error: ");
                break;
            case 1:
                execlp("ls", "ls", "-l", nullptr);
                perror("error: ");
                break;
            case 2:
                execle("/bin/ls", "ls", "-l", nullptr, environ);
                perror("error: ");
                break;
            case 3:
                execv("/bin/ls", argv);
                perror("error: ");
                break;
            case 4:
                execvp("ls", argv);
                perror("error: ");
                break;
            case 5:
                execve("/bin/ls", argv, environ);
                perror("error: ");
                break;
            default:
                cout << "wrong option number" << endl;
        }
        sleep(3);
        printf("------>child process end-----\n");
    }else if(pid > 0){  //父进程执行单元
        cout << "i am parent, pid = " << getpid() << endl;
        wait(nullptr);  //等待子进程结束
        printf("------>parent process end-----\n");
    }
    return 0;
}



int main(int argc, char* argv[]){
    int option = atoi(argv[1]);
    fptExec(option);

    return 0;
}






















