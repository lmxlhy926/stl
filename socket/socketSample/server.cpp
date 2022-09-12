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
#include <sys/poll.h>       //poll相关接口

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
#define POLLMAX 1024
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


/*
 *  要创建一个数组，存储已建立连接的描述符。
 *  当select返回时：
 *          通过和listenfd比对来确定是否是建立连接的请求，如果是，则将该描述符加入数组和监听集合。
 *          通过和数组中的描述符比对，来确定是哪个连接的数据到达，从而实现交互。
 *          如果读取到的数据长度为0，则表明客户端关闭了连接，服务器也应该关闭该连接，并从数组和监听集合中剔除该描述符。
 */
bool server_select(string& port){
    //1. 创建监听描述符，开始监听
    int listenfd = open_listenfd(port.c_str());
    if(listenfd  == -1){
        std::cout << "open_listenfd Error....";
        return false;
    }
    std::cout << "listenfd: " << listenfd << std::endl;

    //记录数据
    std::vector<int> connfdVec;     //记录已建立连接的描述符
    int maxFd = listenfd;   //初始化监听的最大描述符

    //构造初始监听集合，只监听请求连接
    fd_set readSet, initSet;
    FD_ZERO(&initSet);
    FD_SET(listenfd, &initSet);

    std::cout << "Start to Accept connections...." << std::endl;
    while(true){
        readSet = initSet;  //每次请求需要重新赋值
        int nReady = select(maxFd + 1, &readSet, nullptr, nullptr, nullptr);
        std::cout << "wake up, nReady = " << nReady  << std::endl;
        if(nReady == -1){
            std::cout << "select error...." << std::endl;
            return false;
        }

        //如果是连接请求，则记录连接描述符
        if(FD_ISSET(listenfd, &readSet)){
            struct sockaddr_in cliAddr{};
            socklen_t clientLenth = sizeof(sockaddr_in);
            char ipStrBuffer[INET_ADDRSTRLEN];

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

                }else{
                    for(int i = 0; i < nRead; ++i){
                        buf[i] = static_cast<char>(toupper(buf[i]));
                    }
                    write(*pos, buf, nRead);
                }

                if(--nReady == 0)
                    break;

                pos++;
            }
        }
    }
}


/*
 * struct pollfd:
 *      1. 指明要监听的端口， -1代表忽略该项，下次返回时把revents设置为0
 *      2. 指明要监听的事件
 *      3. poll函数返回的监听事件中已发生的事件
 *
 * 和select相比的优点：
 *      1. select中用三个形参来描述三种不同的事件，针对每种事件传入待监听的描述符集合，因此有三个描述符集合。
 *         poll中用一个结构来指定，可以一次性描述符一个描述符监听的所有事件。
 *
 *      2. select中的参数为传入传出参数，函数每次返回都会覆盖监听集合，所以每次调用需要重新赋值。
 *         poll中用events, revents将监听事件和返回事件分开，只需进行一次赋值
 */
bool server_poll(string& port){
    int listenfd = open_listenfd(port.c_str());
    if(listenfd == -1){
        printf("Create Listenfd Error.....\n");
        return false;
    }
    std::cout << "listenfd: " << listenfd << std::endl;

    //监听连接请求
    struct pollfd connFdArray[POLLMAX];   //待监听的描述符数组
    connFdArray[0].fd = listenfd;
    connFdArray[0].events = POLLRDNORM;

    //fd设置为-1，poll不再监控此pollfd, 下次返回时把revents设置为0.
    for(int i = 1; i < POLLMAX - 1; ++i){
        connFdArray[i].fd = -1;
    }

    int maxIndex = 0;

    while(true){
        int nReady = poll(connFdArray, maxIndex + 1, -1);
        if(nReady == -1){
            std::cout << "poll Error..." << std::endl;
            return false;
        }

        //处理连接请求
        if(connFdArray[0].revents & POLLRDNORM){
            std::cout << "connection request....." << std::endl;
            struct sockaddr_in cliAddr{};
            int cliAddrSize = sizeof(sockaddr_in);
            int connfd = accept(listenfd, reinterpret_cast<struct sockaddr *>(&cliAddr), &cliAddrSize);
            if(connfd >= 0){
                char ipStringBuffer[INET_ADDRSTRLEN];
                std::cout << "Request From: " << inet_ntop(AF_INET, &cliAddr.sin_addr, ipStringBuffer, INET_ADDRSTRLEN) << " "
                          << ntohs(cliAddr.sin_port) << std::endl;

                for(int i = 1; i < POLLMAX; ++i){
                    if(connFdArray[i].fd == -1){    //监听建立的连接描述符
                        connFdArray[i].fd = connfd;
                        connFdArray[i].events = POLLRDNORM;
                        maxIndex = std::max(maxIndex, i);
                        break;
                    }
                    if(i == POLLMAX - 1){
                        printf("Too many clients, Refused connection.....\n");
                    }
                }

            }else{
                printf("Accetp Error.....\n");
            }

            if(--nReady == 0)
                continue;
        }

        //处理数据请求
        for(int i = 1; i < POLLMAX; ++i){
            if(connFdArray[i].fd == -1)
                continue;

            if(connFdArray[i].revents & POLLRDNORM){    //相对应的客户端有数据到来
                std::cout << "write request....." << std::endl;
                char buf[1024];
                ssize_t nRead = read(connFdArray[i].fd, buf, 1024);
                if(nRead <= 0){   //客户端主动关闭连接
                    printf("the peer client close the connection....\n");
                    close(connFdArray[i].fd);
                    connFdArray[i].fd = -1;     //不再监听该端口

                }else{  //正常读取到数据
                    for(int j = 0; j < nRead; ++j){
                        buf[j] = static_cast<char>(toupper(buf[j]));
                    }
                    write(connFdArray[i].fd, buf, nRead);
                }

                if(--nReady == 0)
                    continue;
            }

        }
    }
}



int main(int argc, char* argv[]){
    string port = "6666";
//    serv_addrinfo(port);

    server_poll(port);

    return 0;
}


























