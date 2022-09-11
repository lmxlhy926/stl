/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

#define MAXLINE 80
#define SERV_PORT 6666
#define OPEN_MAX 1024

/*
int poll(struct pollfd *fds, nfds_t nfds, int timeout)
	struct pollfd *fds：	指定一个监听数组的首元素地址
	nfds_t nfds：			指定数组中监听的元素数量
	int timeout：			指定时间：阻塞、立马返回、固定时间
	返回值：					有监听事件发生的描述符的数量
	
	fds指定了数组首元素的地址，nfds指定了要监控数组中前多少个元素。如果数组中元素的fd为-1，则不受监控。
	如果不再监控某个文件描述符时，可以把pollfd中，fd设置为-1，poll不再监控此pollfd，下次返回时，把revents设置为0

和select的区别：
	1. struct pollfd结构将监听事件和返回事件区分开，这样每次监听就不需要重新指定监听描述集合
	2. 可以在一个struct pollfd结构中指定一个描述符的多个监测事件，而selet则需要在不同集合中分别指定。
	
	可以认为poll是select的一个改进增强版，实现相同的功能，但是用法更加简单。
*/

int main(int argc, char *argv[])
{
	int i, j, maxi;
	int listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];
	socklen_t clilen;
	
	struct pollfd client[OPEN_MAX];		//监听描述符数组
	
	struct sockaddr_in cliaddr, servaddr;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	listen(listenfd, 20);
	
	//第一个元素监测连接请求，后面的元素监测建立的连接
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM; 		//监听连接请求
	
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1; 				/* 用-1 初始化 client[]里剩下元素 */
	maxi = 0; 							/* client[]数组有效元素中最大元素下标 */
	
	for ( ; ; ) {
		nready = poll(client, maxi+1, -1); 		/* 阻塞 */
		
		if (client[0].revents & POLLRDNORM) { 	/* 有客户端链接请求 */
			//接受连接请求
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
			printf("received from %s at PORT %d\n",
			inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
			
			for (i = 1; i < OPEN_MAX; i++) {
				if (client[i].fd < 0) {
					client[i].fd = connfd; 				/* 找到 client[]中空闲的位置，存放 accept 返回的 connfd */
					client[i].events = POLLRDNORM; 		//监控新建立的连接
					break;
				}
			}
			
			if (i == OPEN_MAX)
				printf("too many clients");
			
			
			if (i > maxi)
				maxi = i; 			/* 更新 client[]中最大元素下标 */
			
			if (--nready <= 0)
				continue; 			/* 没有更多就绪事件时,继续回到 poll 阻塞 */
		}
		
		for (i = 1; i <= maxi; i++) { 		/* 检测 client[] */
			if ((sockfd = client[i].fd) < 0)
				continue;
			
			if (client[i].revents & (POLLRDNORM | POLLERR)) {	//有数据或错误返回
				if ((n = read(sockfd, buf, MAXLINE)) < 0) {
					if (errno == ECONNRESET) { 			/* 当收到 RST 标志时 */
						/* connection reset by client */
						printf("client[%d] aborted connection\n", i);
						close(sockfd);			//关闭客户端
						client[i].fd = -1;		//不再监听该连接
					} else {
						printf("read error");
					}
					
				} else if (n == 0) {
					/* connection closed by client */
					printf("client[%d] closed connection\n", i);
					close(sockfd);
					client[i].fd = -1;
					
				} else {
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
						write(sockfd, buf, n);
				}
				
				if (--nready <= 0)
					break; /* no more readable descriptors */
			}
		}
	}
	
	close(listenfd);
	
	return 0;
}
