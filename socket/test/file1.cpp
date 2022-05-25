
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <iostream>
#include <thread>

#include <cstring>
#include <chrono>


#include "file.hpp"

using namespace std;

/*
 * 连接服务器，发送数据
 */
void file1::func1(){

    string ipaddress = "127.0.0.1"; //字符串格式的ip地址
    unsigned short port = 6000;    //unsigned short类型的端口号

//指定服务器地址（通信域，地址，端口号）
    struct sockaddr_in serverAddress{}; //ipv4地址
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; //ipv4通信域
    inet_pton(AF_INET, ipaddress.c_str(), &serverAddress.sin_addr); //字符串格式地址转换为网络字节序格式地址（32位地址）
    serverAddress.sin_port = htons(port);   //机器字节序的整型端口号转换为网络字节序的整型端口号

    char transform_address[INET_ADDRSTRLEN]{};
    int transform_port{};
    inet_ntop(AF_INET, &serverAddress.sin_addr, transform_address, INET_ADDRSTRLEN);  //网络字节序地址转换为字符串格式
    transform_port = ntohs(serverAddress.sin_port); //网络字节序整型转换为机器字节序整形
    std::cout << "transform_address: " << transform_address << endl;
    std::cout << "transform_port: " << transform_port << endl;

//创建一个socket端口
    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

//连接到服务器
    connect(socketfd, (const struct sockaddr*)&serverAddress, sizeof(serverAddress));

    string str = "hello world";

    while(true){

        char readBuf[1024]{};
        int nbytes = read(socketfd, readBuf, 1024);
        std::cout << "receive: " << readBuf << " ---nbytes: " << nbytes << endl;
        string postMessage = string(readBuf, 0, nbytes) + "---from app";
        send(socketfd, postMessage.c_str(), postMessage.size(), 0);
    }

}


void file1::func2() {

    string ipaddress = "127.0.0.1";
    string host = "6000";

    struct addrinfo hint{};     //用来选择符合特定条件的地址
    struct addrinfo *result;    //结果列表

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_IP;
    hint.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV | AI_CANONNAME;     //控制翻译方式

//将主机名和服务名映射为一个套接字地址
    int ret;
    ret = getaddrinfo(ipaddress.c_str(), host.c_str(), &hint, &result);
    if(ret){
        std::cout << "getaddrinfo error: " << gai_strerror(ret) << std::endl;    //返回执行失败的提示信息
        return;
    }

    for(auto rp = result; rp != nullptr; rp = rp->ai_next){

        int sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sockfd == -1){
            std::cout << "socket create error" << std::endl;
            continue;
        }

    //套接字地址被翻译为一个主机名和服务名
        char getnameinfo_address[INET_ADDRSTRLEN]{};
        char getnameinfo_port[INET_ADDRSTRLEN]{};
        if(!getnameinfo(rp->ai_addr, rp->ai_addrlen, getnameinfo_address, INET_ADDRSTRLEN,
                        getnameinfo_port, INET_ADDRSTRLEN,
                        NI_NUMERICHOST | NI_NUMERICSERV)){
            std::cout << "getnameinfo:address: " << getnameinfo_address << endl;
            std::cout << "getnameinfo:port: " << getnameinfo_port << endl;
            std::cout << "canonname: " << rp->ai_canonname << endl;
        }

        int connectRet = connect(sockfd, rp->ai_addr, rp->ai_addrlen);
        if(connectRet != -1){
            int i = 5;
            for(; i > 0; i--){
                char buf[1024];
                int nbytes = recv(sockfd, buf, 1024, 0);
                string post = string(buf, 0, nbytes) + "---from app\n";
                write(sockfd, post.c_str(), post.size());
            }

        }
    }

    freeaddrinfo(result);
}










































