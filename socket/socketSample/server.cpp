//
// Created by 78472 on 2022/9/4.
//


#include <cstring>
#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


/*
    IPV4地址： 32位无符号整数    （网络字节序）
    端口号：   16位无符号整数    （网络字节序）

    网络字节序和主机字节序之间的转换
        uint32_t htonl(uint32_t hostlong);
        uint16_t htons(uint16_t hostshort);
        uint32_t ntohl(uint32_t netlong);
        uint16_t ntohs(uint16_t netshort);

    点分十进制字符串    <---->    网络字节序的IP地址
        int inet_pton(AF_INENT, const char* src, void *dst);
        const char* inet_ntop(AF_INET, const void* src, char* dst, socklen_t size);

 */


#define MAXLINE 80
#define SERV_PORT 6666

int main(int argc, char* argv[]){
    //创建描述符
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //分配地址
    struct sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                  //指定为IPV4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //网络字节序的IP地址
    servaddr.sin_port = htons(SERV_PORT);           //网络字节序的端口号

    //描述符绑定地址
    bind(listenfd, reinterpret_cast<const sockaddr *>(&servaddr), sizeof(servaddr));

    //将主动套接字转换为监听套接字，声明该套接字可以接受来自客户端的连接请求
    listen(listenfd, 20);

    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    std::cout << "Accepting connections...." << std::endl;
    while(true){
        struct sockaddr_in cliaddr{};
        socklen_t cliaddr_len = sizeof(cliaddr);
        /*
         * accept函数等待来自客户端的连接请求到达监听描述符listenfd
         * 在addr中填写客户端的套接字地址，并返回一个已连接描述符，这个描述符可被利用UNIX IO函数与客户端通信
         */
        int connfd = accept(listenfd, reinterpret_cast<sockaddr*>(&cliaddr), &cliaddr_len);

        int readCount = read(connfd, buf, MAXLINE);
        std::cout << "received from " << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, str, sizeof(str))
                  << " at port " << ntohs(cliaddr.sin_port) << std::endl;

        for(int i = 0; i < readCount; ++i){
            buf[i] = toupper(buf[i]);
        }
        write(connfd, buf, readCount);

    }

    return 0;
}


























