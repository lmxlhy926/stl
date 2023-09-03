/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 6666
#define OPEN_MAX 1024

int main(int argc, char *argv[])
{
	int i, j, maxi, listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];
	socklen_t clilen;
	struct pollfd client[OPEN_MAX];
	struct sockaddr_in cliaddr, servaddr;


//socket->bind->listen
//-----------------------------------------------------------------------------
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);
//-----------------------------------------------------------------------------

//注册fd和对应的事件
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM; 					/* listenfd监听普通读事件 */

	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1; 							/* 用-1初始化client[]里剩下元素 */
	maxi = 0; 										/* client[]数组有效元素中最大元素下标 */
	
	for ( ; ; ) {
		nready = poll(client, maxi+1, -1); 			/* 阻塞等待，直到监控事件发生或超时 */
		
//有客户端链接请求则创建一个对应的connfd，然后将connfd和对应的事件添加进监测列表中。		
		if (client[0].revents & POLLRDNORM) { 		/* 有客户端链接请求 */

			//accept创建一个socketfd和客户端通信
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
			printf("received from %s at PORT %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));
			
			//将创建的socketfd加入到需监测数组中(fd,事件)
			for (i = 1; i < OPEN_MAX; i++) {
				if (client[i].fd < 0) {
					client[i].fd = connfd; 	/* 找到client[]中空闲的位置，存放accept返回的connfd */
					break;
				}
			}
			if (i == OPEN_MAX)
				perr_exit("too many clients");
			client[i].events = POLLRDNORM; 		/* 设置刚刚返回的connfd，监控读事件 */

			//改变监控数组中需要监控的文件描述符个数
			if (i > maxi)
				maxi = i; 						/* 更新client[]中最大元素下标 */
			//如果只有客户端连接事件则返回到poll阻塞
			if (--nready <= 0)
				continue; 						/* 没有更多就绪事件时,继续回到poll阻塞 */
		}

//如果客户端传来数据则读取数据然后进行处理，最后将处理结果返回给客户端。
		for (i = 1; i <= maxi; i++) { 			/* 检测client[]，除了客户端连接事件有更多的事件发生*/
			if ((sockfd = client[i].fd) < 0)
				continue;
			
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				
				if ((n = Read(sockfd, buf, MAXLINE)) < 0) {		//读错误
				
					if (errno == ECONNRESET) { /* 当收到 RST标志时 */
						/* connection reset by client */
						printf("client[%d] aborted connection\n", i);
						Close(sockfd);
						client[i].fd = -1;
					} else {
						perr_exit("read error");
					}
					
				} else if (n == 0) {	//没有读到内容
					/* connection closed by client */
					printf("client[%d] closed connection\n", i);
					Close(sockfd);
					client[i].fd = -1;
					
				} else {	//正确读到了内容
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
						Writen(sockfd, buf, n);
				}
				
				//如果还有未处理的监测到的事件则继续，如果没有则结束
				if (--nready <= 0)
					break; 				/* no more readable descriptors */				
			}
		}
	}

	
	return 0;
}

