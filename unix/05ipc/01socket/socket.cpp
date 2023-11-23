
#include "sys/socket.h"     //socket
#include "arpa/inet.h"      //inet_pton
#include "netinet/in.h"     //IP地址+port结构体
#include "unistd.h"         

#include <cstring>
#include <cstdio>
#include <cctype>
#include <string>
#include <iostream>
#include <chrono>
#include <future>
#include <thread>

using namespace std;

/**
 * 简单的sokcet客户端
 *      1. 创建一个socket
 *      2. 填充服务器地址
 *      3. 连接服务器
 *      4. 向服务器发送数据，接收服务器数据
 *      5. 关闭连接
*/
int sockClient(string ipAddress, uint16_t port){
    //生成客户端通信用的socket, 建立连接时自动分配ip和port
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //给服务器结构体配置地址
    struct sockaddr_in serveraddr{};    //ipv4服务器地址结构体(ip+port)
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;    //ipv4协议
    inet_pton(AF_INET, ipAddress.c_str(), &serveraddr.sin_addr.s_addr);  //ip地址
    serveraddr.sin_port = htons(port); //端口号

    //和服务器建立连接
    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    //接收/发送消息
    string str = "hello world";
    write(sockfd, str.c_str(), str.size());

    //接收消息
    char buf[1024];
    ssize_t nRead = read(sockfd, buf, 1024);
    std::cout << "client: response from server: ";
    std::cout.flush();
    write(STDOUT_FILENO, buf, nRead);
    
    //关闭连接
    close(sockfd);

    return 0;
}


int sockserver(uint16_t port){
    //创建接收连接请求的socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //填充服务器地址
    struct sockaddr_in serveraddr{};
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    //给listenfd绑定监听地址
    bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    //监听监听描述符
    listen(listenfd, 20);

    printf("server: Accept connections ....\n");
    
    /**
     * 接收客户端的连接请求，创建出用于数据通信的connfd, 并存储请求客户端的地址
    */
    struct sockaddr_in clientaddr{};
    socklen_t clientaddr_len = sizeof(clientaddr);
    int connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddr_len);

    while(true){
        char buf[1024]{};
        ssize_t nRread = read(connfd, buf, 1024);
        if(nRread > 0){
            char ipBuffer[INET_ADDRSTRLEN]{};
            printf("server: received from %s at port %d, content: %s\n",
                    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipBuffer, sizeof(ipBuffer)),
                    ntohs(clientaddr.sin_port),
                    const_cast<char*>(string(buf, nRread).c_str()));

            for(int i = 0; i < nRread; ++i){
                buf[i] = toupper(buf[i]);
            }
            write(connfd, buf, nRread);

        }else if(nRread == 0){
            cout << "server: client closed" << endl;
            close(connfd);
            return 0;

        }else if(nRread == EOF){
            perror("socket read error");
            return 0;
        }      
    }
}



int main(int argc, char* argv[]){

    sockserver(60001);

    return 0;
}





































