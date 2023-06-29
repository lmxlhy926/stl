#include <cstdio>
#include <unistd.h>

// 声明外部函数
int add(int a, int b);
int sub(int a, int b);
extern "C" void show(int option, int result);

// 定义本地函数
int mul(int a, int b){
    return a * b;
}

// 声明外部全局变量
extern int Global_a, Global_b;

// 全局变量包含函数地址
typedef int(*addFuncType)(int, int);
addFuncType addFuncPtr = add;


int main(int argc, char* argv[]){
    printf("------%s\n", argv[0]);
    int local_a = 1, local_b = 2;
    show(1, mul(local_a, local_b));
    show(1, local_a + local_b);
    int result = add(local_a, local_b);
    result = sub(result, 4);
    show(2, result);
    show(3, result);
    return 0;
}



