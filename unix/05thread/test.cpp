
#include <cstdio>
#include <cstring>
#include <thread>
#include <pthread.h>
#include <unistd.h>

/**
 * 线程：
 *      pthread_t
 *      pthread_self()
 *      pthread_create()
 *      pthread_detach()
 *      pthread_join()
 *      pthread_exit()
 * 
 *      线程属于进程
 *      线程包括主线程和其它线程
 *      线程共享进程的数据结构
 *      如果某个线程里调用exit，则会导致进程退出，从而所有线程都退出
 */


void* thread_func(void* arg){
    int count = *(int*)arg;
    for(int i = 0; i < count; ++i){
        printf("--------\n");
        sleep(1);
    }

    int* value = (int*)malloc(sizeof(int));
    *value = 100;
    pthread_exit(value);
}


void thread_test(){
    printf("main thread id : %lu\n", pthread_self());

    pthread_t threadId;
    int errcode;
    int* ptr = (int*)malloc(sizeof(int));
    *ptr = 5;
    errcode = pthread_create(&threadId, nullptr, thread_func, ptr);
    if(errcode != 0){
        perror("pthread_create");
        exit(-1);
    }

    int* value;
    pthread_join(threadId, (void**)&value);
    printf("join value: %d\n", *value);
    free(value);
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int container = 0;


void* thread_func1(void* arg){
    for(int i = 0; i < 10; ++i){
        pthread_mutex_lock(&mutex);
        printf("hello - 1\n");
        printf("hello - 2\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(nullptr);
}

void* thread_func2(void* arg){
    for(int i = 0; i < 10; ++i){
        pthread_mutex_lock(&mutex);
        printf("world - 1\n");
        printf("world - 2\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(nullptr);
}

void thread_test1(){
    pthread_t threadID1, threadID2;
    pthread_create(&threadID1, nullptr, thread_func1, nullptr);
    pthread_create(&threadID2, nullptr, thread_func2, nullptr);

    pthread_join(threadID1, nullptr);
    pthread_join(threadID2, nullptr);
}

void* consumer(void* arg){
    while(true){
        pthread_mutex_lock(&mutex);
        while(container == 0){
            pthread_cond_wait(&cond, &mutex);
        }
        printf("consume container: %d\n", container);
        container--;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(nullptr);
}

void* producter(void* arg){
    while(true){
        pthread_mutex_lock(&mutex);
        container++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        sleep(1);
    }
    pthread_exit(nullptr);
}



int main(int argc, char* argv[]){
    pthread_t threadId1, threadID2;

    pthread_create(&threadId1, nullptr, consumer, nullptr);
    pthread_create(&threadID2, nullptr, producter, nullptr);
   
    pthread_exit(nullptr);
}



