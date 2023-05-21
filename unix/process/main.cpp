
#include <cstdio>
#include <unistd.h>
#include <cstdlib>



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



int main(int argc, char* argv[]){
    forkTest();


    return 0;
}







