
#include <cstring>

#include <iostream>
#include <string>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

#include "sockUtil.h"

int main(int argc, char* argv[]){
    int port = 6666;

    int listenfd = open_listenfd_tcp(port);
    if(listenfd == -1){
        printf("Create Listenfd Error.....\n");
        return -1;
    }

    //监听连接请求
    struct pollfd connFdArray[1024];   //监听描述符数组
    connFdArray[0].fd = listenfd;
    connFdArray[0].events = POLLRDNORM;

    //fd设置为-1，poll不再监控此pollfd, 下次返回时把revents设置为0.
    for(int i = 1; i < 1024; ++i){
        connFdArray[i].fd = -1;
    }

    int maxIndex = 0;

    while(true){
        int nReady = poll(connFdArray, maxIndex + 1, -1);
        if(nReady == -1){
            std::cout << "poll Error..." << std::endl;
            return -1;
        }

        //如果是连接请求，则在服务器建立通信端点，并监听该端点是否有数据到达
        if(connFdArray[0].revents & POLLRDNORM){
            std::cout << "connection request....." << std::endl;
            struct sockaddr_in cliAddr{};
            socklen_t cliAddrSize = sizeof(sockaddr_in);
            int connfd = accept(listenfd, reinterpret_cast<struct sockaddr *>(&cliAddr),&cliAddrSize);
            if(connfd == -1){
                printf("Accetp Error.....\n");
            }else{
                char ipStringBuffer[INET_ADDRSTRLEN];
                std::cout << "Request From: " << inet_ntop(AF_INET, &cliAddr.sin_addr.s_addr, ipStringBuffer, INET_ADDRSTRLEN) << " "
                          << ntohs(cliAddr.sin_port) << std::endl;

                for(int i = 1; i < 1024; ++i){
                    if(connFdArray[i].fd == -1){    //存储建立的连接描述符，并监听该描述符
                        connFdArray[i].fd = connfd;
                        connFdArray[i].events = POLLRDNORM;
                        maxIndex = std::max(maxIndex, i);
                        break;
                    }
                    if(i == 1024 - 1){
                        printf("Too many clients, Refused connection.....\n");
                        close(connfd);
                    }
                }
            }

            if(--nReady == 0)
                continue;
        }

        //处理数据请求
        for(int i = 1; i < 1024; ++i){
            if(connFdArray[i].fd == -1)
                continue;

            if(connFdArray[i].revents & POLLRDNORM){    //相对应的客户端有数据到来
                char buf[1024];
                ssize_t nRead = read(connFdArray[i].fd, buf, 1024);
                std::cout << "readData: " << string(buf, nRead) << std::endl;

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


