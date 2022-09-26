//
// Created by 78472 on 2022/9/4.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define SERV_PORT 6666
#define MAXLINE 80


using namespace std;
/*
 * 建立的客户端示例；
 *      连接服务器，发送消息，接收消息
 */
void normalClient(const char* message){
    //分配请求的服务器地址
    struct sockaddr_in servaddr{};              //IPV4地址
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);     //点分十进制ip地址--->网络字节序二进制地址
    servaddr.sin_port = htons(SERV_PORT);

    //创建套接字描述符
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "sockfd: " << sockfd << std::endl;

    //阻塞，一直到连接成功建立或者发生错误
    int connectRet = connect(sockfd, reinterpret_cast<const struct sockaddr* >(&servaddr), sizeof(servaddr));
    if(connectRet == 0){
        write(sockfd, message, strlen(message));

        char buf[MAXLINE];
        ssize_t n = read(sockfd, buf, MAXLINE);
        if(n <= 0){
            close(sockfd);
            return;
        }
        std::cout << "Response from server: " << std::endl;
        write(STDOUT_FILENO, buf, n);
    }

    close(sockfd);
}


/**
 * @param hostname      服务器名
 * @param port          服务器端口号
 * @return              用于和服务器通信的已连接描述符
 */
int open_clientfd(const char* hostname, const char* port){
    struct addrinfo hint{};
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_INET;           //IPV4
    hint.ai_socktype = SOCK_STREAM;     //端点用于连接
    hint.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;    //数字形式的ip和端口号

    struct addrinfo *p, *listp;
    int retCode;
    if((retCode = getaddrinfo(hostname, port, &hint, &listp)) != 0){
        printf("%s\n", gai_strerror(retCode));  //打印错误
        return -1;
    }

    int clientfd;
    for(p =  listp; p != nullptr; p= p->ai_next){
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
            continue;
        }
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) == 0){
            break;
        }

        close(clientfd);    //连接失败后需要关闭该描述符
    }

    //释放地址列表
    freeaddrinfo(listp);

    if(p == nullptr){   //列表里没有有效的地址
        return -1;
    }else
        return clientfd;
}


void client_addrinfo(string& hostname, string& port){
    string message = "helloworld";
    int clientfd;
    if((clientfd = open_clientfd(hostname.c_str(), port.c_str())) == -1)
        return;

    std::cout << "clientfd: " << clientfd << std::endl;

    write(clientfd, message.c_str(), message.size());

    char receiveBuf[MAXLINE];
    ssize_t n = read(clientfd, receiveBuf, MAXLINE);
    if(n <= 0){
        close(clientfd);
        return;
    }

    std::cout << "Response from server: " << std::endl;
    write(STDOUT_FILENO, receiveBuf, n);
    close(clientfd);
}






int main(int argc, char* argv[]){
    std::string ip = "127.0.0.1";
    std::string port = "6666";
    client_addrinfo(ip, port);

    return 0;
}







