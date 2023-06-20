
#include <cstdio>
#include <cstdlib>
#include <unistd.h>


static void myExit1(){
    printf("-->myExit handler1....\n");
}

static void myExit2(){
    printf("-->myExit handler2....\n");
}

void enter(){
    printf("-->enter start...\n");
    exit(0);
    printf("-->enter end...\n");
}

void exitTest(){
    atexit(myExit1);
    atexit(myExit2);
    atexit(myExit2);

    printf("main is done...\n");
    abort();
    enter();
}

//打印环境变量数组
int printEnviron(){
    for(int i = 0; *(environ + i) != nullptr; ++i){
        printf("%s\n", *(environ + i));
    }
    return 0;
}

int main(int argc, char* argv[]){
    printEnviron();
    
    return 0;
}













