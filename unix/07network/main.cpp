
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <thread>


void socketClient(){
    // 服务器地址
    struct sockaddr_in serverAddr{};
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(35555);

    // 创建端点
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // 连接服务器
    connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    // 和我服务器通信
    
    printf("--->send\n");
    write(sockfd, "hello", 5);
    sleep(5);
    

    // 关闭连接
    close(sockfd);
    return;
}


void socketServer(){
    // 服务器地址
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(35555);

    // 创建服务器连接请求监听端点
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // 监听端点绑定地址
    bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // 监听
    listen(listenfd, 5);

    struct sockaddr_in clientAddress;
    socklen_t clientAddress_len = sizeof(clientAddress);
    int connfd = accept(listenfd, (struct sockaddr*)&clientAddress, &clientAddress_len);

    printf("connfd: %d\n", connfd);

    
    char buf[1024] = {};
    int nread = read(connfd, buf, sizeof(buf));
    printf("received: %s\n", buf);

    return;
}






int main(int argc, char* argv[]){
    std::thread server(socketServer);

    printf("---------------\n");

    std::thread client(socketClient);

    client.join();
    server.join();
    
    return 0;
}



