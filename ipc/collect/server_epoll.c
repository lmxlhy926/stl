#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 6666
#define OPEN_MAX 1024

/*
int epoll_create(int size) 
	创建一个句柄，size指明该句柄可监听的最大的描述符数量

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
	epfd：epoll_create创建的句柄
	op：
		EPOLL_CTL_ADD：	添加监测的描述符，以及该描述符要监测的事件
		EPOLL_CTL_DEL： 不再监听指定的描述符
		EPOLL_CTL_MOD：	修改指定的描述符要监测的事件
	fd： 	描述符
	event：	指向描述监测事件的结构体
	
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
	epfd：		创建的句柄
	events：	存储返回事件数组的首地址
	maxevents：	存储数组的可容纳数量
	timeout：	等待时间：
					-1：	阻塞；
					0：		立马返回;
					>0：	阻塞特定的时间
					
与poll以及select的区别：
	1. 拆分为3个函数，逻辑更加清晰
			创建句柄：	指定最大监测数
			句柄控制：	注册、删除、修改描述符以及对应的事件
			等待事件：	返回值指示发生的事件的数量，将发生的事件存储到提供的数组中
			
	2. 	除了返回值指示发生的事件的数量外，内核还将发生的事件加入到提供的数组中，这样只需遍历返回数组集合即可。
		无需遍历整个被监听的描述符集合，节省了时间。

*/

int main(int argc, char *argv[])
{
	int i, j, maxi;
	
	char buf[MAXLINE], str[INET_ADDRSTRLEN];
	int client[OPEN_MAX];				//存储建立连接的connfd
	
	struct epoll_event tep;				//设定事件和伴随数据
	struct epoll_event ep[OPEN_MAX];	//存储返回的事件
	
	int listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	Listen(listenfd, 20);
	
	for (i = 0; i < OPEN_MAX; i++)
		client[i] = -1;
	maxi = -1;
	
	//创建一个epoll句柄，指定监听的文件描述符的最大个数
	int efd = epoll_create(OPEN_MAX);
	if (efd == -1)
		perr_exit("epoll_create");
	
	//添加监控事件、与事件相伴随的描述数据
	tep.events = EPOLLIN; 
	tep.data.fd = listenfd;
	
	int res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);	//监听listenfd描述符
	if (res == -1)
		perr_exit("epoll_ctl");
	
	while (1) {
		int nready = epoll_wait(efd, ep, OPEN_MAX, -1); 	 //阻塞监听
		if (nready == -1)
			perr_exit("epoll_wait");
		
		for (i = 0; i < nready; i++) {		//只需遍历返回的事件集合即可，无需遍历全部描述符集合
			if (!(ep[i].events & EPOLLIN))
				continue;
			
			if (ep[i].data.fd == listenfd) {	//连接请求
				struct sockaddr_in cliaddr
				socklen_t clilen = sizeof(cliaddr);
				int connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
				printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
				ntohs(cliaddr.sin_port));
				
				for (j = 0; j < OPEN_MAX; j++) {
					if (client[j] < 0) {
						client[j] = connfd; 	/* save descriptor */
						break;
					}
				}
				
				if (j == OPEN_MAX)
					perr_exit("too many clients");
				
				if (j > maxi)
					maxi = j; 				/* max index in client[] array */
				
				tep.events = EPOLLIN;		//将connfd加入监听
				tep.data.fd = connfd;
				res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
				if (res == -1)
					perr_exit("epoll_ctl");
				
			} else {	//普通数据
				int sockfd = ep[i].data.fd;
				ssize_t n = Read(sockfd, buf, MAXLINE);
				
				if (n == 0) {
					for (j = 0; j <= maxi; j++) {
						if (client[j] == sockfd) {	//清空连接
							client[j] = -1;
							break;
						}
					}
					
					res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);	//不再监听该连接
					if (res == -1)
						perr_exit("epoll_ctl");
					
					Close(sockfd);
					printf("client[%d] closed connection\n", j);
					
				} else {
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
					Writen(sockfd, buf, n);
				}
			}
		}
	}
	
	close(listenfd);
	close(efd);
	
	return 0;
}