#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int globalInt = 1000;

void test1(){
    int stackInt = 10;
    int* intPtr = reinterpret_cast<int*>(malloc(sizeof(int)));
    *intPtr = 100;
    printf("pid: %d, statcInt: %d, intPtr: %p, *intPtr: %d, globalInt: %d\n", getpid(), stackInt, intPtr, *intPtr, globalInt);

    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(-1);
    }else if(pid == 0){
        printf("in child process ....\n");
        stackInt++;
        *intPtr = *intPtr + 1;
        globalInt++;
        printf("pid: %d, statcInt: %d, intPtr: %p, *intPtr: %d, globalInt: %d\n", getpid(), stackInt, intPtr, *intPtr, globalInt);
        exit(1);
    }else if(pid > 0){
        sleep(1);
         printf("in parent process ....\n");
         printf("pid: %d, statcInt: %d, intPtr: %p, *intPtr: %d, globalInt: %d\n", getpid(), stackInt, intPtr, *intPtr, globalInt);
    }
}

static void atExitHandler(){
    printf("at exit handler 1 ...\n");
}

int main(int argc, char* argv[]){
    atexit(atExitHandler);

    fprintf(stdout, "hello world");

    // _exit(0);
    abort();
    // exit(0);

    
    return 0;
}



















