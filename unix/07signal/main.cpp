
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
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


int main(int argc, char* argv[]){
    unsigned int left = sleep1(2);
    printf("sleep %d\n", left);

    return 0;
}


















