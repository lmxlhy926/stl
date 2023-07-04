#include "rio.hpp"
#include "socket.hpp"
#include "tiny_web_func.h"

#include <sys/socket.h>
#include <cstdlib>
#include <netdb.h>


int main(int argc, char **argv){

    int listenfd, connfd;                       //监听端口，连接端口
    char hostname[MAXLINE], port[MAXLINE];      //客户端主机名，端口号
    socklen_t clientlen;                        //客户端套接字地址长度
    struct sockaddr_storage clientaddr{};       //存放套接字地址

    if(argc != 2){
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if((listenfd = open_listenfd(argv[1])) == -1){
        fprintf(stderr, "filed to create listenfd\n");
        exit(1);
    }

    while(true){
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);   //接收客户端连接请求
        getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0); //获取客户端地址信息
        printf("accept connection from (%s %s)\n", hostname, port);
        doit(connfd);   //处理客户端请求
        close(connfd);  //关闭连接端点
    }

}

















