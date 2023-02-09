//
// Created by 78472 on 2023/2/8.
//


#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>


int searchDir(int argc, char* argv[]){
    DIR *dirPtr;    //指向目录
    struct dirent *direntryPtr; //指向目录项
    if(argc != 2){
        printf("usage: %s directory\n", argv[0]);
        exit(-1);
    }
    if((dirPtr = opendir(argv[1])) == nullptr){ //打开一个目录
        printf("cant open %s\n", argv[1]);
        exit(-1);
    }
    while((direntryPtr = readdir(dirPtr)) != nullptr){  //读取目录，得到目录项
        printf("%s\n", direntryPtr->d_name);    //输出目录项内容
    }
    closedir(dirPtr);   //关闭一个目录
    exit(0);
}

int readFile(){
#define BUFFSIZE 4096
    ssize_t n;  //实际读取到的字节数，也是要输出的字节数
    char buf[BUFFSIZE];     //数据缓存
    while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){     //从标准输入读取指定字节数的内容
        if(write(STDOUT_FILENO, buf, n) != n){  //将读取到的数据写入到标准输出
            printf("write error\n");
            exit(-1);
        }
    }
    if(n < 0){  //如果不是因为读取到文件末尾退出，而是因为读取错误退出，再次打印
        printf("read error\n");
    }
    exit(0);
}

int get(){
    int c;
    while((c = getc(stdin)) != EOF){    //从标准输入读取一个字符
        if(putc(c, stdout) == EOF){     //向标准输出写入一个字符
            printf("output error\n");
        }
    }
    if(ferror(stdin)){  //判断退出是否是因为读取错误导致的
        printf("input error\n");
    }
    exit(0);
}

int processid(){
    printf("hello world from process ID %ld\n", static_cast<long>(getpid()));   //打印当前进程ID
    exit(0);
}


int basicShell(){
    char buf[1024];
    pid_t pid;  //进程ID
    printf("%% ");
    while(fgets(buf, 1024, stdin) != nullptr){  //从标准输入读取命令
        if(buf[strlen(buf) -1] == '\n'){
            buf[strlen(buf) -1] = 0;
        }

        if((pid = fork()) < 0){     //fork()函数出错
            printf("fork error\n");
            exit(-1);
        }else if(pid == 0){     //子进程
            execlp(buf, buf, nullptr);  //加载新程序执行
            printf("couldnt execute: %s\n", buf);
            exit(-1);
        }

        int status;
        if(waitpid(pid, &status, 0) < 0){   //回收子进程
            printf("waitpid error\n");
        }
        printf("%% ");
    }
    exit(0);
}

int main(int argc, char* argv[]){
    basicShell();

    return 0;
}