
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <pthread.h>
#include <unistd.h>

using namespace std;

/*
posix线程库

线程作用：一个线程绑定一个函数，可以为函数设定执行逻辑顺序，使函数按照特定的逻辑顺序执行。

线程函数：
	类型：pthread_t pid;						//线程标识符类型
    标识: pthread_t pthread_self(void)			//在例程中，获取执行本例程的线程标识符ID
	创建：int pthread_create(pthread_t *thread, const pthread_attr_t *attr,  void *(*start_routine) (void *), void *arg);							 
	退出：void pthread_exit(void *retval);					    //在线程的例程中，例程以返回值退出
	脱离：int pthread_detach(pthread_t thread);					//使指定的线程在后台执行，自动回收
	杀死：int pthread_cancel(pthread_t thread);					//杀死指定的线程
	回收：int pthread_join(pthread_t thread, void **retval);	//回收指定的线程，阻塞等待，直到线程执行完毕


	return          ：返回到调用函数
	pthread_exit()  ：结束调用该函数的线程
	exit            ：退出进程

	使用流程：
		1. 在主线程中创建一个线程，使线程执行指定的例程；创建线程成功后，会返回线程的唯一标识符。
		2. 线程例程中执行逻辑，执行完毕后，返回一个指针。
		3. 在主线程中可做的操作
				1. 使创建的线程脱离运行，无需回收状态
				2. 阻塞回收指定的线程
				3. 杀死指定的线程

线程库的限制：
	1. 例程函数的格式是指定的，例程函数的参数只能接受一个指针，也只能返回一个指针
*/




static int globalVal = 0;   //线程共享全局变量

typedef struct software{
    char title[20];
	int version;
}software_t;

typedef struct ThreadArgs{
    int args;
}ThreadArgs_t;


//线程的传入参数、返回参数都是指针，注意释放指针对象
void *thread_procedure1(void *arg){
	int i = reinterpret_cast<ThreadArgs_t*>(arg)->args;
    free(arg);  //释放传入的参数指针对象
	printf("%dth thread  %lu\n", i, pthread_self());
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
void *thread_procedure2(void *arg){
    for(int i = 0; i < 10; ++i){
        printf("thread_func2---%d\n", i);
        usleep(50 * 1000);
    }
    pthread_exit(nullptr);
}


void *thread_procedure3(void *arg){
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
int threadTest(void){
	std::map<pthread_t, string> threadIdMap;	//存储线程标识符和返回值类型
	for(int i = 0; i < 3; i++){
		pthread_t threadId;
        ThreadArgs_t* args = new ThreadArgs_t;
        args->args = i;
		int ret;
		if((ret = pthread_create(&threadId, nullptr, thread_procedure1, args)) == 0){	//成功返回0， 失败返回错误号
			if(i == 0 || i == 1){
				threadIdMap.insert(std::make_pair(threadId, "int"));
			}else{
				threadIdMap.insert(std::make_pair(threadId, "struct"));
			}
		}else{
			fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
			return 0;
		}
	}
	
	sleep(1);
	printf("\n");
	
	for(auto& elem : threadIdMap){
		void* retPtr{nullptr};
		pthread_t threadId = elem.first;
		string threadRetType = elem.second;
		int err = pthread_join(threadId, &retPtr);	    //回收线程，若该线程还没执行完则阻塞等待
		printf("pthread_join : %s\n", strerror(err));

		if(elem.second == "int"){
            printf("ret == %d\n", *reinterpret_cast<int *>(retPtr));  //打印回收函数接收的线程的返回值
        }else{
            software_t* softwarePtr = reinterpret_cast<software_t *>(retPtr);
			printf("softwareTitle = %s softwareVersion = %d\n", softwarePtr->title, softwarePtr->version);
		}
		if(retPtr != nullptr)
        	free(retPtr);  //释放线程返回的指针指向的对象
	}
	pthread_exit(nullptr);
}


int threadTest1(){
	pthread_t tid;
	pthread_create(&tid, nullptr, thread_procedure2, nullptr);
	pthread_detach(tid);	                //指定线程和主线程脱离，指定线程结束后自动释放占用的所有资源，不需要主线程再去回收资源。
	sleep(1);
    int err = pthread_join(tid, nullptr);	//指定线程已经和主控线程断开关系，调用该函数会直接返回错误号，不会阻塞。
	printf("err = %d : %s\n", err, strerror(err));
	return 0;
}


int threadTest2(void){   //线程取消
	pthread_t tid;
	pthread_create(&tid, nullptr, thread_procedure3, nullptr);
    usleep(10);
    printf("-----------cancel start------------\n");
	pthread_cancel(tid);    //杀死线程，指定线程到了检查点才会检测是否发出此命令。
    printf("-----------cancel end------------\n");
	int err = pthread_join(tid, nullptr);
    printf("err = %d : %s\n", err, strerror(err));
	printf("end of procedure\n");
	return 0;
}



/*
主要内容：互斥量、读写锁、条件变量
mutex: 0,1; 互斥量。
	pthread_mutex_t mutex;
	初始化：	int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
	加锁：	    int pthread_mutex_lock(pthread_mutex_t *mutex);		//阻塞
	加锁：	    int pthread_mutex_trylock(pthread_mutex_t *mutex);	//立即返回
	解锁：	    int pthread_mutex_unlock(pthread_mutex_t *mutex);	//解除阻塞
	销毁：	    int pthread_mutex_destroy(pthread_mutex_t *mutex);	//销毁

	锁使用流程：
		1. 创建一个mutex变量（主线程、子线程都能访问锁变量）
		2. 主线程中初始化锁
		3. 在线程中：加锁--执行逻辑--解锁
		4. 执行完毕后在主线程中销毁锁


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

	condition + mutex的使用流程：
		1. 创建一个condition和mutex
		2. 初始化condition和mutex
		3. 消费者子线程
				* 加锁
				* 判断条件，不符合则释放锁，并休眠等待；被唤醒后，加锁并继续判断条件；
				* 在加锁条件下，执行保护逻辑
				* 解锁
		4. 生产者子线程
				* 加锁
				* 执行保护逻辑
				* 解锁
				* 发起唤醒通知
*/

pthread_mutex_t mutex;

void *thread_mutex_procedure1(void *arg){
	for(int i = 0; i < 10; ++i){
		pthread_mutex_lock(&mutex);  //加锁
			printf("hello world\n");
			usleep(10 * 1000);
			printf("world hello\n");
		pthread_mutex_unlock(&mutex); //解锁
		usleep(1);
	}
	int* ret = new int;
	*ret = 100;
	pthread_exit(ret);
}

void *thread_mutex_procedure2(void *arg){
	while(true){
		pthread_mutex_lock(&mutex);	
			printf("***** *****\n");
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(nullptr);
}


int thread_mutex_test(void){
	pthread_t tid[2];
	pthread_mutex_init(&mutex, nullptr);  //初始化互斥量
	pthread_create(&tid[0], nullptr, thread_mutex_procedure1, nullptr);
	pthread_create(&tid[1], nullptr, thread_mutex_procedure2, nullptr);

	pthread_join(tid[0], nullptr);
	pthread_cancel(tid[1]);
	printf("join and cancle end.....\n");

	pthread_mutex_destroy(&mutex);	//销毁互斥量
	return 0;
}


pthread_rwlock_t rwlock;

void *thread_write(void *arg){
	while(true){
		pthread_rwlock_wrlock(&rwlock);	
			printf("??????????????\n");
			usleep(1);
		pthread_rwlock_unlock(&rwlock);
	}
	pthread_exit(nullptr);
}

void *thread_read_first(void *arg){
	while(true){
		pthread_rwlock_rdlock(&rwlock);
			printf("xxxxxxxxxxxxxx\n");
			usleep(1);
			printf("xxxxxxxxxxxxxx\n");
		pthread_rwlock_unlock(&rwlock);
	}
	pthread_exit(nullptr);
}

void *thread_read_second(void *arg){
	while(true){
		pthread_rwlock_rdlock(&rwlock);
			printf("--------------\n");
		pthread_rwlock_unlock(&rwlock);
	}
	pthread_exit(nullptr);
}

int thread_rwlock_test(void){
	pthread_t tid[3];
	pthread_rwlock_init(&rwlock, nullptr);

	pthread_create(&tid[2], nullptr, thread_write, nullptr);
	pthread_create(&tid[0], nullptr, thread_read_first, nullptr);
	pthread_create(&tid[1], nullptr, thread_read_second, nullptr);
	
	usleep(1000 * 1000);
	pthread_rwlock_destroy(&rwlock);
	return 0;
}



int counter{0};
pthread_cond_t conditionVar;

//加锁，操控数据，释放锁，通知
void *thread_producer(void *arg){
	while(true){
		pthread_mutex_lock(&mutex);
		counter += 10;
		printf("++thread_producer counter == %d\n", counter);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&conditionVar);		//操作完数据释放锁后，执行一次唤醒。
		sleep(2);
	}
}

/**
 * 加锁
 * 判断条件：
 * 		不符合：释放锁、陷入等待
 * 			   被唤醒后，获取锁，重复上述流程
 * 		符合：操作数据
 * 释放锁
 * 
*/
void *thread_consumer(void *arg){
	while(true){
		pthread_mutex_lock(&mutex);		//锁定
		time_t start = time(nullptr);
		while(counter == 0){ 	//不满足条件则阻塞且放弃获得的锁
			pthread_cond_wait(&conditionVar, &mutex);  //当条件满足后，再次尝试获取锁，并进行条件判断。
		}
		time_t end = time(nullptr);
		printf("------%ld------\n", end - start);
		while(counter > 0){
			counter--;
			printf("--thread_consumer counter == %d\n", counter);
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(nullptr);
}


/**
 * 初始化锁，初始化环境变量
 * 创建线程执行例程
 * 延迟
 * 终止线程
*/
int producer_customer_test(void){
	pthread_t pid, cid;
	pthread_mutex_init(&mutex, nullptr);
	pthread_cond_init(&conditionVar, nullptr);
	pthread_create(&pid, nullptr, thread_producer, nullptr);
	pthread_create(&cid, nullptr, thread_consumer, nullptr);

	sleep(10);
	pthread_cancel(pid);
	pthread_cancel(cid);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&conditionVar);
	return 0;
}



int main(int argc, char* argv[]){
  	producer_customer_test();
	
    return 0;
}

