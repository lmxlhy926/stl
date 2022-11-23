/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAXLINE 80
#define SERV_PORT 6666


/*
int select(int nfds,
           fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout);

	int nfds：			       监听集合中的最大描述符加1，指定内核轮询的最大范围，即只会检查这个范围内的文件描述符
	fd_set *readfds：	       指定读描述符监测集合
	fd_set *writefds：	       指定写描述符监测集合
	struct timeval *timeout：  指定阻塞时间：阻塞、立马返回(0)、阻塞固定时间(>0)
	
只要监测集合中有一个监测事件发生就会返回，readfds，writefds都是传入传出参数，
即返回值会更改变量的内容。因此每次传入值时需要重新赋值。


单线程：同时监听多个端口数据，某个端口或某几个端口有数据到达时，进行处理
	1. 刚开始只监测listenfd描述符
	2. 每次轮询最多接受一个连接请求

    **连接请求/数据到达**
	当select返回时，说明有描述符可读，也就是说可能收到了一个新的客户端连接请求，也可能是某个已有连接有数据到达。
	如果是客户端连接请求，则接受请求并将建立的connfd也放入监测集合。
	然后循环检查每个监听的connfd是否有数据到达，如果某个connfd有数据到达则进行处理
	全部处理完毕后，再继续select()进行阻塞监听。
*/


int main(int argc, char *argv[])
{
	int i, maxi, maxfd;
	int listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE]; 	/* FD_SETSIZE 默认为 1024 */
	
	ssize_t n;
	fd_set rset, allset;				//传入集合，监听集合
	
	char buf[MAXLINE];					//存储读取到的数据
	char str[INET_ADDRSTRLEN]; 			//存储ip地址的字符串表示
	
	socklen_t cliaddr_len;							//地址结构的长度
	struct sockaddr_in cliaddr, servaddr;			//客户端地址、服务器地址
	
	//创建监听socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	//设置服务器地址
	bzero(&servaddr, sizeof(servaddr));				
	servaddr.sin_family = AF_INET;					//地址族：标识ipv4, ipv6
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//监听这台服务器上的所有地址
	servaddr.sin_port = htons(SERV_PORT);			//服务器端口号
	
	//绑定
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	//监听
	listen(listenfd, 20); 		/* 默认最大 128 */
	
	maxfd = listenfd; 		//初值最大描述符
	maxi = -1; 				/* client[]的下标 */
	
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1; 	/* 用-1 初始化 client[] */
	
	//构造 select 监控文件描述符集
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset); 	
	
	for ( ; ; ) {
		rset = allset; 	/* 每次循环都要重新设置 select 监控信号集 */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		
		if (nready < 0)
			printf("select error");
		
		//新的客户端连接请求，每次只接受一个请求
		if (FD_ISSET(listenfd, &rset)) { 	
			cliaddr_len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
			printf("received from %s at PORT %d\n", 
			inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
			
			for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = connfd; 	/* 保存 accept 返回的文件描述符到 client[]里 */
					break;
				}
			}
			
			/* 达到 select 能监控的文件个数上限 1024 */
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}
			
			FD_SET(connfd, &allset); 	//更新监控描述符集合
			if (connfd > maxfd)
				maxfd = connfd; 		//更新最大描述符
			if (i > maxi)
				maxi = i; 				//更新 client[]最大下标值 
			
			if (--nready == 0)
				continue; 				//处理完所有的描述符，则继续返回监听
		}
	
		//轮询，检测connfd是否有数据到达
		for (i = 0; i <= maxi; i++) { 		/* 检测哪个 clients 有数据就绪 */
			if ( (sockfd = client[i]) < 0)
				continue;
			
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = read(sockfd, buf, MAXLINE)) == 0) {	//对端客户端关闭连接
					close(sockfd); 				//服务端也关闭连接
					FD_CLR(sockfd, &allset); 	//不再监听该连接
					client[i] = -1;				//清除该连接
					
				} else {
					int j;
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
					write(sockfd, buf, n);
				}
				
				if (--nready == 0)
					break;
			}
		}
	}
	
	close(listenfd);
	return 0;
}