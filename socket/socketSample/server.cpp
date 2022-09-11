//
// Created by 78472 on 2022/9/4.
//


#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#include <netinet/in.h>     //IP协议，IPV4地址
#include <arpa/inet.h>      //inet_pton, inet_ntop, + <netinet/in.h>
#include <netdb.h>          //getaddrinfo, getnameinfo
#include <unistd.h>         //unix基础系统API接口
#include <sys/select.h>     //select接口

using namespace std;

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


void normalServ(){
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

    char receiveBuf[MAXLINE];
    char IPBuf[INET_ADDRSTRLEN];
    std::cout << "Accepting connections...." << std::endl;
    while(true){
        struct sockaddr_in cliaddr{};
        socklen_t cliaddr_len = sizeof(cliaddr);
        /*
         * accept函数等待来自客户端的连接请求到达监听描述符listenfd
         * 在addr中填写客户端的套接字地址，并返回一个已连接描述符，这个描述符可被利用UNIX IO函数与客户端通信
         */
        int connfd = accept(listenfd, reinterpret_cast<sockaddr*>(&cliaddr), &cliaddr_len);

        ssize_t readCount = read(connfd, receiveBuf, MAXLINE);
        if(readCount <= 0){
            return;
        }

        std::cout << "received from " << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, IPBuf, sizeof(IPBuf))
                  << " at port " << ntohs(cliaddr.sin_port) << std::endl;

        for(int i = 0; i < readCount; ++i){
            receiveBuf[i] = toupper(receiveBuf[i]);
        }
        write(connfd, receiveBuf, readCount);

    }
}



/**
 *
 * @param port  标识主机上的一个进程
 * @return      返回监听描述符
 */
int open_listenfd(const char* port){
    struct addrinfo hint{};
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_INET;           //IPV4
    hint.ai_socktype = SOCK_STREAM;     //用于连接
    hint.ai_flags = AI_PASSIVE;         //端口为被动接口, 用作监听端口
    hint.ai_flags |= AI_NUMERICSERV;    //端口号为数字形式不为服务名，跳过服务名到端口号的解析

    /*
     * 使用AI_PASSIVE标志并将host参数设置为nullptr, 每个套接字地址结构中的地址字段会被设置为通配符地址
     * 告诉内核这个服务器接收发送到本主机上的所有IP地址的请求
     */
    struct addrinfo *p, *listp;
    int retcode;
    if((retcode = getaddrinfo(nullptr, port, &hint, &listp)) != 0){
        printf("%s\n", gai_strerror(retcode));
        return -1;
    }

    int listenfd, optval = 1;
    for(p = listp; p != nullptr; p = p->ai_next){
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
            continue;
        }

        //使得服务器能够被终止、重启和立即开始接收连接请求
        //一个重启的服务器默认将在30秒内拒绝客户端的连接请求，这严重地阻碍了调试
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)(&optval), sizeof(int));

        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0){
            break;
        }
        close(listenfd);
    }

    freeaddrinfo(listp);

    if(p == nullptr)
        return -1;

    if(listen(listenfd, 20) < 0){
        close(listenfd);
        return -1;
    }

    return listenfd;
}


void serv_addrinfo(string& port){
    int listenfd = open_listenfd(port.c_str());
    std::cout << "listenfd :" << listenfd << std::endl;
    if(listenfd == -1)
        return;

    char receiveBuf[MAXLINE];
    char IPBuf[INET_ADDRSTRLEN];
    std::cout << "Accepting connections...." << std::endl;
    while(true){
        struct sockaddr_in cliaddr{};
        socklen_t cliaddr_len = sizeof(cliaddr);

        /*
         * accept函数等待来自客户端的连接请求到达监听描述符listenfd
         * 在addr中填写客户端的套接字地址，并返回一个已连接描述符，这个描述符可被利用UNIX IO函数与客户端通信
         */
        int connfd = accept(listenfd, reinterpret_cast<sockaddr*>(&cliaddr), &cliaddr_len);
        if(connfd < 0){
            printf("Accept Error.....\n");
        }

        ssize_t readCount = read(connfd, receiveBuf, MAXLINE);
        if(readCount <= 0){
            close(connfd);
            return;
        }

        std::cout << "received from " << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, IPBuf, sizeof(IPBuf))
                  << " at port " << ntohs(cliaddr.sin_port) << std::endl;

        for(int i = 0; i < readCount; ++i){
            receiveBuf[i] = toupper(receiveBuf[i]);
        }
        write(connfd, receiveBuf, readCount);
    }
}


bool server_select(string& port){
    //1. 创建监听描述符，开始监听
    int listenfd = open_listenfd(port.c_str());
    if(listenfd  == -1){
        std::cout << "open_listenfd Error....";
        return false;
    }
    std::cout << "listenfd: " << listenfd << std::endl;

    //记录数据
    std::vector<int> connfdVec;
    int maxFd = listenfd;
    struct sockaddr_in cliAddr{};
    socklen_t clientLenth = sizeof(sockaddr_in);
    char ipStrBuffer[INET_ADDRSTRLEN];

    //2. 监听是否有数据到来
    fd_set readSet, initSet;
    FD_ZERO(&initSet);
    FD_SET(listenfd, &initSet);

    std::cout << "Start to Accept connections...." << std::endl;
    while(true){
        readSet = initSet;
        int nReady = select(maxFd + 1, &readSet, nullptr, nullptr, nullptr);
        std::cout << "wake up, nReady = " << nReady  << std::endl;
        if(nReady == -1){
            std::cout << "select error...." << std::endl;
            return false;
        }

        //如果是连接请求，则记录连接描述符
        if(FD_ISSET(listenfd, &readSet)){
            int connfd = accept(listenfd, reinterpret_cast<sockaddr *>(&cliAddr), &clientLenth);
            if(connfd >= 0){
                std::cout << "Request From " << inet_ntop(AF_INET, &cliAddr.sin_addr, ipStrBuffer, INET_ADDRSTRLEN) << "  "
                                             << ntohs(cliAddr.sin_port) << std::endl;
                FD_SET(connfd, &initSet);   //监听该连接描述符
                connfdVec.push_back(connfd);   //连接描述符加入遍历列表
                if(connfd > maxFd)             //更新最大描述符
                    maxFd = connfd;

            }else
                std::cout << "accept Error...." << std::endl;

            if(--nReady == 0)
                continue;
        }

        //如果是数据请求，则找到相应的描述符，进行交互
        for(auto pos = connfdVec.begin(); pos != connfdVec.end(); ){
            if(FD_ISSET(*pos, &readSet)){
                std::cout << "data arrived at " << *pos << std::endl;
                char buf[1024];
                ssize_t nRead = read(*pos, buf, 1024);
                if(nRead <= 0){
                    std::cout << "read Error or Client closed...." << std::endl;
                    close(*pos);
                    FD_CLR(*pos, &initSet);   //停止监听该连接节点
                    pos = connfdVec.erase(pos);     //删除会返回下一元素的位置
                    int maxElement = *max_element(connfdVec.begin(), connfdVec.end());
                    maxFd  = listenfd > maxElement ? listenfd : maxElement;
                    continue;
                }

                for(int i = 0; i < nRead; ++i){
                    buf[i] = static_cast<char>(toupper(buf[i]));
                }
                write(*pos, buf, nRead);

                if(--nReady == 0)
                    break;
                pos++;
            }
        }
    }
}



int main(int argc, char* argv[]){
    string port = "6666";
//    serv_addrinfo(port);

    server_select(port);

    return 0;
}


























