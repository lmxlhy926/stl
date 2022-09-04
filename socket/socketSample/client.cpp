//
// Created by 78472 on 2022/9/4.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERV_PORT 6666
#define MAXLINE 80

int main(int argc, char* argv[]){
    if(argc != 2){
        fputs("usage: %client <message>", stderr);
        exit(1);
    }

    char* str = argv[1];

    //分配请求的服务器地址
    struct sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);     //点分十进制ip地址--->网络字节序二进制地址
    servaddr.sin_port = htons(SERV_PORT);

    //创建套接字描述符
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //阻塞，一直到连接成功建立或者发生错误
    int connectRet = connect(sockfd, reinterpret_cast<const struct sockaddr* >(&servaddr), sizeof(servaddr));
    if(connectRet == 0){
        write(sockfd, str, strlen(str));

        char buf[MAXLINE];
        int n = read(sockfd, buf, MAXLINE);
        std::cout << "Response from server: " << std::endl;
        write(STDOUT_FILENO, buf, n);
    }

    close(sockfd);

    return 0;
}







