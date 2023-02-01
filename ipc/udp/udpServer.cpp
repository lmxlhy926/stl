
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define SERV_PORT 6666
using namespace std;


/*
	1. 建立一个套接字描述符s
    2. 填充服务器地址
    3. 将套接字文件描述符s绑定到指定的服务器套接字地址
    4. 阻塞接收数据
    5. 处理数据
    6. 向客户端发送处理后的数据
*/
void server(){
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];      //存储数据来源端主机点分十进制ip地址

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //指定服务器ip地址
    struct sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                           //ipv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //表示本机的任意网卡地址
    servaddr.sin_port = htons(SERV_PORT);           //端口号

    bind(sockfd, reinterpret_cast<struct sockaddr*>(&servaddr), sizeof(servaddr));  //绑定服务器地址


    printf("Accepting connections ...\n");
    while (true) {
        struct sockaddr_in cliaddr{};               //存储客户端的地址
        socklen_t cliaddr_len = sizeof(cliaddr);

        //阻塞接收数据，存储客户端地址
        ssize_t nReceive = recvfrom(sockfd, buf, MAXLINE,0, reinterpret_cast<struct sockaddr *>(&cliaddr), &cliaddr_len);
        if (nReceive == -1){
            perror("recvfrom error");
            break;
        }

        printf("received from %s at PORT %d: %s\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr, str, INET_ADDRSTRLEN),
               ntohs(cliaddr.sin_port),
               string(buf, nReceive).c_str());

        for (int i = 0; i < nReceive; i++)
            buf[i] = toupper(buf[i]);

        //向客户端发送数据
        ssize_t nSend = sendto(sockfd, buf, nReceive, 0, reinterpret_cast<struct sockaddr *>(&cliaddr), sizeof(cliaddr));
        if (nSend == -1){
            perror("sendto error");
            break;
        }
    }

    close(sockfd);
}


void server2(){
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];      //存储数据来源端主机点分十进制ip地址

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //指定服务器ip地址
    struct sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                           //ipv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //表示本机的任意网卡地址
    servaddr.sin_port = htons(SERV_PORT);           //端口号

    bind(sockfd, reinterpret_cast<struct sockaddr*>(&servaddr), sizeof(servaddr));  //绑定服务器地址


    printf("Accepting connections ...\n");
    while (true) {
        struct sockaddr_in cliaddr{};               //存储客户端的地址
        socklen_t cliaddr_len = sizeof(cliaddr);

        //阻塞接收数据，存储客户端地址
        ssize_t nReceive = recvfrom(sockfd, buf, MAXLINE,0, reinterpret_cast<struct sockaddr *>(&cliaddr), &cliaddr_len);
        if (nReceive == -1){
            perror("recvfrom error");
            break;
        }

        printf("received from %s at PORT %d: %s\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr, str, INET_ADDRSTRLEN),
               ntohs(cliaddr.sin_port),
               string(buf, nReceive).c_str());

        sleep(2);
    }

    close(sockfd);
}


int main(int argc, char* argv[])
{
	server2();
	return 0;
}