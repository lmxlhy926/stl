#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAXLINE 80
#define SERV_PORT 6666


/*
	1. 服务器和客户端无需建立连接
	2. 服务器和客户端对等通信，阻塞接收数据
*/

int main(void)
{
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	printf("Accepting connections ...\n");
	while (1) {
		struct sockaddr_in cliaddr;
		socklen_t cliaddr_len = sizeof(cliaddr);
		
		//阻塞接收数据，存储客户端地址
		n = recvfrom(sockfd, buf, MAXLINE,0, (struct sockaddr *)&cliaddr, &cliaddr_len);
		if (n == -1)
			perror("recvfrom error");
		
		printf("received from %s at PORT %d\n",
		inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
		ntohs(cliaddr.sin_port));
		
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		
		//向客户端发送数据
		n = sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		if (n == -1)
			perror("sendto error");
	}
	
	close(sockfd);
	return 0;
}