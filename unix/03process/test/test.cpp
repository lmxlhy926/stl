
/**
 * 
 * 进程：
 *      1. 创建进程：fork
 *                  父进程
 *                  子进程
 *          父进程如果提前结束、则子进程被init进程收养
 *          返回子进程的进程号
 * 
 *      2. 回收进程：waitpid
 *                      * 退出状态
 *                      * 终止信号
 *                      * 停止信号
 *                      * 进程继续运行
 * 
 *      3. 进程加载新的程序运行
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(-1);
    }


    return 0;
}








