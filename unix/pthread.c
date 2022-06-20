#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
线程作用：一个线程绑定一个函数，可以为函数设定执行逻辑顺序，使函数按照特定的逻辑顺序执行。

线程函数：
	pthread_t pid;							//线程标识符类型
	ID：pthread_t pthread_self(void)		//获取本线程的线程标识符ID
	创建：int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
							 void *(*start_routine) (void *), void *arg);		//创建线程
							 
	退出：void pthread_exit(void *retval);					    //自己以返回值退出
	脱离：int pthread_detach(pthread_t thread);					//自己脱离，后台执行，自动回收
	杀死：int pthread_cancel(pthread_t thread);					//其它线程杀死该线程；
	回收：int pthread_join(pthread_t thread, void **retval);	//其它线程回收该线程，阻塞等待


	return：返回到调用函数
	pthread_exit()：结束调用该函数的线程
	exit：退出进程

*/



int var = 0; //线程共享全局变量

typedef struct book
{
	int booknum;
	char bookname[20];
}book_t;


int func1(void);
int func2(void);
int func3(void);


void *thread_func1(void *arg)
{
	int i = (int)arg;
	printf("%dth thread\n", i);

	if(0 == i)
	{
		var = 100;
		printf("	var == %d\n", var);
		pthread_exit((int *)100);	//退出线程，并返回一个指针，该指针可被pthread_join接收
	}
	else if(1 == i)
	{
		var = 200;
		printf("	var == %d\n", var);
		pthread_exit((int *)200);
	}
	else
	{
		printf("	var == %d\n", var);
		book_t *book = (book_t *)malloc(sizeof(book_t));
		book->booknum = 1;
		strcpy(book->bookname, "title");
		pthread_exit(book);
	}

	pthread_exit(NULL);
}

void *thread_func2(void *arg)
{
	int n = 3;
	while(n--)
		printf("count == %d\n",n);

	pthread_exit(NULL);
}

void *thread_func3(void *arg)
{
	while(1)
	{
		printf("oooi\n");
		pthread_testcancel();	//向程序中手动加入检测点，程序到了检测点会检测该线程是否被杀死。
		
	}
			
	pthread_exit(NULL);
}


int main(void)
{
	func3();

	return 0;
	
}




int func1(void)
{
	pthread_t tid[3];
	int ret;
	int err;
	void *retval[3] = {NULL};

	for(int i = 0; i < 3; i++)
	{
		ret = pthread_create(&tid[i], NULL, thread_func1, (void *)i);	//成功返回0， 失败返回错误号
		if(ret != 0)
		{
			fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
			return 0;
		}
	}
	
	sleep(1);
	printf("\n");
	
	for(int i = 0; i < 3; i++)
	{
		err = pthread_join(tid[i], &retval[i]);	//回收线程，若该线程还没执行完则阻塞等待
		printf("pthread_join : %s\n", strerror(err));
	
		if(0 == i || 1 == i)
			printf("ret == %d\n", (int)(retval[i]));  //打印回收函数接收的线程的返回值
		else
		{
			printf("booknum = %d bookname = %s\n",((book_t *)retval[i])->booknum, ((book_t *)retval[i])->bookname);
			free(retval[i]);  //释放线程中malloc的内存空间
		}
	
	}

	pthread_exit(NULL);

}


int func2(void)	//线程分离
{
	pthread_t tid;
	int err;

	pthread_create(&tid, NULL, thread_func2, NULL);
	pthread_detach(tid);	//指定线程和主线程脱离，指定线程结束后自动释放占用的所有资源，不需要主线程再去回收资源。

	while(1)
	{
		err = pthread_join(tid, NULL);	//指定线程已经和主控线程断开关系，调用该函数会直接返回错误号，不会阻塞。
		printf("err = %d : %s\n", err, strerror(err));
		sleep(1);
	}

	return 0;
}


int func3(void)	//线程取消
{
	pthread_t tid;

	pthread_create(&tid, NULL, thread_func3, NULL);
	usleep(1);
	
	pthread_cancel(tid);  //杀死线程，指定线程到了检查点才会检测是否发出此命令。
	pthread_join(tid, NULL);
	printf("end of func3\n");

	return 0;
}






