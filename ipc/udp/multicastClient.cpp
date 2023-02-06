//
// Created by 78472 on 2023/2/6.
//


#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>

#define MULTICAST_GROUP "224.0.0.88"
#define PORT 30000

/*
    发送组播数据：
        向特定的组发送数据，和向特定主机发送数据是一样的，只要ip地址为多播地址即可。
 */
using namespace std;
int main(int argc, char* argv[]){
    int clientFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientFd == -1){
        perror("socket");
        exit(-1);
    }

    //组播地址
    struct sockaddr_in multiAddr{};
    multiAddr.sin_family = AF_INET;
    inet_pton(AF_INET, MULTICAST_GROUP, &multiAddr.sin_addr);
    multiAddr.sin_port = htons(PORT);

    string str = "hello multicast";
    while(true){
        sendto(clientFd, str.data(), str.size(), 0,
               reinterpret_cast<struct sockaddr*>(&multiAddr), sizeof(multiAddr));
        std::cout << "send....." << std::endl;
        sleep(3);
    }

    return 0;
}