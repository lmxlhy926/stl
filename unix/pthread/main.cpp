
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

/*
posix线程库

线程作用：一个线程绑定一个函数，可以为函数设定执行逻辑顺序，使函数按照特定的逻辑顺序执行。

线程函数：
	类型：pthread_t pid;						//线程标识符类型
    标识: pthread_t pthread_self(void)		//获取本线程的线程标识符ID
	创建：int pthread_create(pthread_t *thread, const pthread_attr_t *attr,  void *(*start_routine) (void *), void *arg);							 
	退出：void pthread_exit(void *retval);					    //自己以返回值退出
	脱离：int pthread_detach(pthread_t thread);					//自己脱离，后台执行，自动回收
	杀死：int pthread_cancel(pthread_t thread);					//其它线程杀死该线程；
	回收：int pthread_join(pthread_t thread, void **retval);	//其它线程回收该线程，阻塞等待


	return          ：返回到调用函数
	pthread_exit()  ：结束调用该函数的线程
	exit            ：退出进程
*/

static int globalVal = 0;   //线程共享全局变量

typedef struct software{
    char title[20];
	int version;
}software_t;

typedef struct ThreadArgs{
    int args;
}ThreadArgs_t;


void *thread_func1(void *arg){
	int i = reinterpret_cast<ThreadArgs_t*>(arg)->args;
    free(arg);  //释放传入的参数指针对象
	printf("%dth thread\n", i);
	if(0 == i){
		globalVal = 100;
		printf("	globalVal == %d\n", globalVal);
        int* intPtr = new int;
        *intPtr = 100;
		pthread_exit(intPtr);    //退出线程，并返回一个指针，该指针可被pthread_join接收
	}else if(1 == i){
		globalVal = 200;
		printf("	globalVal == %d\n", globalVal);
        int* intPtr = new int;
        *intPtr = 200;
		pthread_exit(intPtr);
	}else{
		printf("	globalVal == %d\n", globalVal);
		software_t* software = (software_t *)malloc(sizeof(software_t));
        strncpy(software->title, "title", 5);
        software->version = 1;
		pthread_exit(software);
	}
	pthread_exit(nullptr);
}


//普通线程例程
void *thread_func2(void *arg){
    for(int i = 0; i < 10; ++i){
        printf("thread_func2---%d\n", i);
        usleep(500 * 1000);
    }
    pthread_exit(nullptr);
}


void *thread_func3(void *arg){
    long long int count = 0;
	while(true)
	{
		printf("cout: %lld\n", count);
        count++;
		//pthread_testcancel();	//向程序中手动加入检测点，程序到了检测点会检测该线程是否被杀死。
	}
    int* intPtr = new int;
    *intPtr = 100;
	pthread_exit(intPtr);
}


/**
 *  创建3个线程，将返回的线程ID存入数组
 *  依次回收线程，打印线程的返回值
 *  注意：
 *      1. 线程传参时要注意作用域，线程接收的参数是指针，要保证线程执行期间指针指向的对象一直有效。
 *      2. 回收线程要注意释放已结束线程返回的指针对象
*/
int test1(void){
	pthread_t tid[3];
	void *retval[3]{};

	for(int i = 0; i < 3; i++){
        ThreadArgs_t* args = new ThreadArgs_t;
        args->args = i;
		int ret = pthread_create(&tid[i], nullptr, thread_func1, args);	//成功返回0， 失败返回错误号
		if(ret != 0)
		{
			fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
			return 0;
		}
	}
	
	sleep(1);
	printf("\n");
	
	for(int i = 0; i < 3; i++){
		int err = pthread_join(tid[i], &retval[i]);	    //回收线程，若该线程还没执行完则阻塞等待
		printf("pthread_join : %s\n", strerror(err));
		if(0 == i || 1 == i){
            printf("ret == %d\n", *reinterpret_cast<int *>(retval[i]));  //打印回收函数接收的线程的返回值
        }else{
            software_t* softwarePtr = reinterpret_cast<software_t *>(retval[i]);
			printf("softwareTitle = %s softwareVersion = %d\n", softwarePtr->title, softwarePtr->version);
		}
        free(retval[i]);  //释放线程返回的指针指向的对象
	}

	pthread_exit(nullptr);
}


int test2(){
	pthread_t tid;
	pthread_create(&tid, nullptr, thread_func2, nullptr);
	pthread_detach(tid);	                //指定线程和主线程脱离，指定线程结束后自动释放占用的所有资源，不需要主线程再去回收资源。
    int err = pthread_join(tid, nullptr);	//指定线程已经和主控线程断开关系，调用该函数会直接返回错误号，不会阻塞。
	printf("err = %d : %s\n", err, strerror(err));
	return 0;
}


int test3(void){   //线程取消
	pthread_t tid;
	pthread_create(&tid, nullptr, thread_func3, nullptr);
    usleep(1);
    printf("-----------cancel start------------\n");
	pthread_cancel(tid);    //杀死线程，指定线程到了检查点才会检测是否发出此命令。
     printf("-----------cancel end------------\n");
	int err = pthread_join(tid, nullptr);
    printf("err = %d : %s\n", err, strerror(err));
	printf("end of test3\n");
	return 0;
}


int main(int argc, char* argv[]){
    test3();
    sleep(2);
    return 0;
}

