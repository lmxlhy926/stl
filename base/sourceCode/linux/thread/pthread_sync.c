#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


/*
主要内容：互斥量、读写锁、条件变量、信号量
操作过程：
	创建、初始化
	加1
	减1
	销毁释放

mutex: 0,1; 互斥量。
	pthread_mutex_t mutex;
	初始化：	int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
	加锁：	    int pthread_mutex_lock(pthread_mutex_t *mutex);		//阻塞
	加锁：	    int pthread_mutex_trylock(pthread_mutex_t *mutex);	//立即返回
	解锁：	    int pthread_mutex_unlock(pthread_mutex_t *mutex);	//解除阻塞
	销毁：	    int pthread_mutex_destroy(pthread_mutex_t *mutex);	//销毁


rwlock: 读共享，写独占。 读锁、写锁并行阻塞，写锁优先级高。
	pthread_rwlock_t rwlock;
	int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
	int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);		//读锁定
	int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);		//试着读锁定
	int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);		//写锁定
	int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);		//试着写锁定
	int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);		//解除锁定
	int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);		//销毁


condition: 和mutex结合
	//条件变量类型
	pthread_cond_t condition;
	//初始化条件变量
	int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
	//释放锁，挂起等待
	int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	//释放锁，挂起等待，最多等待一段时间
	int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
	//唤醒一个挂起的等待
	int pthread_cond_signal(pthread_cond_t *cond);
	//唤醒所有挂起的等待
	int pthread_cond_broadcast(pthread_cond_t *cond);
	//销毁条件变量
	int pthread_cond_destroy(pthread_cond_t *cond);


semaphore:
	sem_t sem;
	int sem_init(sem_t *sem, int pshared, unsigned int value);
	int sem_wait(sem_t *sem);
	int sem_trywait(sem_t *sem);	
	int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
	int sem_post(sem_t *sem);
	int sem_destroy(sem_t *sem);

*/

#define NUM 5

pthread_mutex_t mutex;	//创建互斥量
pthread_rwlock_t rwlock;
pthread_cond_t has_product;
sem_t empty_number, product_number;
int queue[NUM];
int counter = 0;


void *thread_func1(void *arg)
{
	srand(time(NULL));
	
	while(1)
	{
		pthread_mutex_lock(&mutex);  //加锁
			printf("HELLO");
			sleep(rand()%3);
			printf("WORLD\n");
		pthread_mutex_unlock(&mutex);  //解锁
		sleep(rand()%3);
	}
	
	pthread_exit(NULL);
	
}

void *thread_write(void *arg)
{
	int i = (int)arg;
	while(1)
	{
		pthread_rwlock_wrlock(&rwlock);	
			printf("%dth thread_write, counter == %d\n", i, ++counter);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}

	pthread_exit(NULL);
}

void *thread_read(void *arg)
{
	int i = (int)arg;
	while(1)
	{
		pthread_rwlock_rdlock(&rwlock);
			printf("%dth thread_read, counter == %d\n", i, counter);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *thread_producer(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		counter++;
		printf("thread_producer counter == %d\n", counter);
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&has_product);	//操作完数据释放锁后，执行一次唤醒。
		sleep(3);
	}

}

void *thread_consumer(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);		//锁定
	
		while(counter == 0) //不满足条件则阻塞且放弃获得的锁
		{
			pthread_cond_wait(&has_product, &mutex);  //当条件满足后，再次尝试获取锁，并进行条件判断。
		}
	
		if(counter > 0)
			counter--;
		printf("thread_consumer counter == %d\n", counter);
		pthread_mutex_unlock(&mutex);

		sleep(1);
	}

	pthread_exit(NULL);
}


//生产者往循环队列里放入数据，消费者从循环队列里读取数据。两者的顺序是一致的。
//空位，满位。 放入数据看空位，读取数据看满位。除了空位状态、满位状态、还有一种未完成状态(放入未完成、读取未完成)。
void *thread_sem_producer(void *arg)
{
//占取一个空位、操作状态、放入完成
	int i = 0;
	while(1)
	{
		sem_wait(&empty_number);	//空格-1
		queue[i] = rand() % 100 +1;
		printf("---produce---%d\n", queue[i]);
		sem_post(&product_number);	//产品+1
		
		i = (i +1) % NUM;
		sleep(rand() % 3);
	}

	pthread_exit(NULL);
}

void *thread_sem_consumer(void *arg)
{
//占取一个满位、操作状态、消费完成
	int i = 0;
	while(1)
	{
		sem_wait(&product_number);
		printf("***consume***%d\n", queue[i]);
		queue[i] = 0;
		sem_post(&empty_number);

		i = (i +1) % NUM;
		sleep(rand() % 1);
	}

	pthread_exit(NULL);
}

int func1(void);
int func2(void);
int func3(void);
int func4(void);


int main(void)
{
	func4();


	return 0;
}

int func1(void)
{
	int flag = 5;
	pthread_t tid;
	srand(time(NULL));	//设置随机数种子

	pthread_mutex_init(&mutex, NULL);  //初始化互斥量
	pthread_create(&tid, NULL, thread_func1, NULL);
	
	while(1)
	{
		pthread_mutex_lock(&mutex);
			printf("hello");
			sleep(rand()%3);
			printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand()%3);
	}

	pthread_cancel(tid);
	pthread_join(tid, NULL);

	pthread_mutex_destroy(&mutex);	//销毁互斥量

	return 0;
}


int func2(void)
{
	pthread_t tid[8];
	pthread_rwlock_init(&rwlock, NULL);

	for(int i = 0; i < 3; i++)
	{
		pthread_create(&tid[i], NULL, thread_write, (void *)i);
	}
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&tid[i+3], NULL, thread_read, (void *)i);
	}

	for(int i = 0; i < 8; i++)
	{
		pthread_join(tid[i], NULL);
	}

	
	pthread_rwlock_destroy(&rwlock);

	return 0;
	
}

int func3(void)
{
	pthread_t cid, pid;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&has_product, NULL);

	srand(time(NULL));

	pthread_create(&pid, NULL, thread_producer, NULL);
	pthread_create(&cid, NULL, thread_consumer, NULL);

	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	return 0;
}


int func4(void)
{
	pthread_t pid, cid;
	
	sem_init(&empty_number, 0, NUM);
	sem_init(&product_number, 0, 0);

	pthread_create(&pid, NULL, &thread_sem_producer, NULL);
	pthread_create(&cid, NULL, &thread_sem_consumer, NULL);

	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	sem_destroy(&empty_number);
	sem_destroy(&product_number);

	return 0;
}












