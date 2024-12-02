
#include <cstdio>
#include <cstring>
#include <thread>
#include <pthread.h>
#include <unistd.h>

void* thread_func1(void* arg){
    int* ptr = (int*)arg;
    printf("received int: %d\n", *ptr);


    for(int i = 0; i < 3; ++i){ 
        printf("thread_func1 %lu.....\n", pthread_self());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // intPtr的值是一个地址值
    int* intPtr = (int*)malloc(sizeof(int));
    // 改变地址值指向的内容
    *intPtr = 100;
    // 返回intPtr包含的地址值
    pthread_exit(intPtr);
}

void* thread_func2(void* arg){
    for(int i = 0; i < 5; ++i){ 
        printf("thread_func2 %lu.....\n", pthread_self());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    int* intPtr = (int*)malloc(sizeof(int));
    *intPtr = 200;
    pthread_exit(intPtr);
}


int main(int argc, char* argv[]){
   int a = 100;

    pthread_t tid1, tid2;
    pthread_create(&tid1, nullptr, thread_func1, &a);
    pthread_create(&tid2, nullptr, thread_func2, nullptr);

    int *int1, *int2;
    pthread_join(tid1, (void**)&int1);
    printf("pthread_join tid1: %d\n", *int1);
    free(int1);

    pthread_join(tid2, (void**)&int2);
    printf("pthread_join tid2: %d\n", *int2);
    free(int2);

    pthread_exit(nullptr);
    return 0;
}











