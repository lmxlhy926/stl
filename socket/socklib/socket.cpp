
#include "socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

int open_clientfd(const char* hostname, const char* port){
    int clientfd;   //连接描述符
    struct addrinfo hints{}, *listp, *p;  //指示消息、列表指针、遍历指针
    int ret;

    memset(&hints, 0, sizeof(struct addrinfo));             //全部置0
    hints.ai_family = AF_INET;                              //ipv4地址
    hints.ai_socktype = SOCK_STREAM;                        //tcp链接
    hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;        //port限制为数字端口号

    if((ret = getaddrinfo(hostname, port, &hints, &listp)) != 0){   //成功则返回0，如果错误则为非0的错误代码
        fprintf(stderr, "get address info error: %s\n", gai_strerror(ret));
        return INVALIDFD;
    }

    for(p = listp; p; p = p->ai_next){  //遍历列表, 最后指向nullptr
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){  //失败返回-1
            continue;
        }
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) == 0){   //成功则为0，出错则为-1.
            break;
        }
        close(clientfd);    //连接失败，需要关闭该端点
    }

    freeaddrinfo(listp);    //释放列表信息

    if(!p)
        return INVALIDFD;  //所有的连接都失败
    else
        return clientfd;  //成功建立的描述符
}


/*
 * 功能：用给定的端口号创建一个监听描述符
 * 参数：服务器的端口号
 * 返回：创建失败返回-1，否则返回创建的监听描述符
 */
int open_listenfd(const char* port){

    struct addrinfo hints{}, *listp, *p;  //指示信息、列表指针、遍历指针
    int listenfd, rc, optval = 1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;   //socket端点为监听端点,端口号只能为数字

    if((rc = getaddrinfo(nullptr, port, &hints, &listp)) != 0){     //不指定ip地址，ip地址为通配符
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        return INVALIDFD;
    }

    for(p = listp; p; p = p->ai_next){
        if((listenfd= socket(p->ai_family, p->ai_socktype, p->ai_protocol)) <0)
            continue;

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const void*>(&optval), sizeof(optval));

        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)  //若成功则为0，若失败则为-1
            break;
        close(listenfd);
    }

    freeaddrinfo(listp);

    if(!p)  //没有生效的地址
        return INVALIDFD;

    if(listen(listenfd, LISTEN_FD_MAX) < 0){  //监听失败
        close(listenfd);
        return INVALIDFD;
    }

    return listenfd;    //返回监听成功地描述符
}


void echo(int fd){
    ssize_t n;        //每次实际读取到的字节数
    char  buf[LINEMAX];  //存储读取到的内容
    rio_t rio;

    rio_readinitb(&rio, fd);
    while((n = rio_readlineb(&rio, buf, LINEMAX)) > 0){
        std::cout << "received: " << n << " bytes, content: " << buf << std::endl;
        rio_writen(fd, buf, n);
    }
}


int test_server(const char *port){
    int listenfd, connfd;   //监听端点，服务端点
    socklen_t clientlen;    //客户端地址长度
    struct sockaddr_storage clientaddr{};  //客户端地址缓冲区
    char client_hostname[1024], client_port[1024];  //主机名，端口号

    if((listenfd = open_listenfd(port)) < 0)  return -1;

    while(true){
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);

        if(connfd > 0){
            int flags = NI_NUMERICHOST | NI_NUMERICSERV;
            getnameinfo((SA *)&clientaddr, clientlen, client_hostname, 1024, client_port, 1024, flags);
            printf("connected to server (%s %s)\n", client_hostname, client_port);
            echo(connfd);
            close(connfd);
        }else{
            return -1;
        }
    }
}















