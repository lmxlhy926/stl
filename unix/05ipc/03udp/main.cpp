
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

/*
 *  1. 创建socket客户端端点
 *  2. 直接向服务器端点发送数据，发送数据时自动为客户端端点指定ip地址和端口号
 *  3. 接收服务端数据返回
 */
void udpClient(const string &ip, uint16_t port){
    //创建UDP socket客户端
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1){
        std::cout << "create socket failed....." << std::endl;
        return;
    }

    //服务器地址
    struct sockaddr_in serverAddr{};
    memset(&serverAddr, 0, sizeof serverAddr);
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(port);

    /**
     * 指明服务器的地址，向服务器发送数据
    */
    string str = "helloworld";
    sendto(sockfd, str.c_str(), str.size(), 0, reinterpret_cast<sockaddr *>(&serverAddr), sizeof serverAddr);

    //从服务器接收数据
    char buf[1024];
    struct sockaddr_in peerEnd{};
    socklen_t peerEndLength = sizeof peerEnd;
    ssize_t nRead = recvfrom(sockfd, buf, 1024, 0, reinterpret_cast<sockaddr *>(&peerEnd), &peerEndLength);   //接收数据，并存储来源地址
    if(nRead <= 0){
        std::cout << "receive error....." << std::endl;
    }else{
        char addrBuf[INET_ADDRSTRLEN];
        std::cout << "Recevie from " << inet_ntop(AF_INET, &peerEnd.sin_addr, addrBuf, INET_ADDRSTRLEN)
                 << ":" << ntohs(peerEnd.sin_port) << std::endl;

        for (int i = 0; i < nRead; i++)
            buf[i] = ::toupper(buf[i]);
        sendto(sockfd, buf, nRead, 0, reinterpret_cast<sockaddr *>(&peerEnd), peerEndLength);
    }

    close(sockfd);
}



/**
 * UDP协议可以通过connect()设置对端地址，可以使用send()直接向特定的服务器发送数据，不再需要单独指定服务器地址
 */
void udpClient_connnect(const string &ip, uint16_t port){
    //创建UDP socket客户端
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1){
        std::cout << "create socket failed....." << std::endl;
        return;
    }

    //服务器地址
    struct sockaddr_in serverAddr{};
    memset(&serverAddr, 0, sizeof serverAddr);
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(port);

     //设置默认对端地址
    connect(sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));

    /**
     * 指明服务器的地址，向服务器发送数据
    */
    string str = "helloworld";
    for(int i = 0; i < 10; ++i){
        send(sockfd, str.c_str(), str.size(), 0);
    }

    close(sockfd);
}


/*
 *  客户端和服务器之间没有连接
 *  所有的请求都是数据请求，不需要创建专门的连接描述符，直接向对方的地址发送数据即可
 *  updServer:
 *      1. 创建服务端socket
 *      2. 绑定地址、端口号
 *      3. 阻塞读取数据，读取到数据后进行处理，向对端返回数据
 */
void udpServer(uint16_t port){
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1){
        std::cout << "create socket failed..." << std::endl;
        return;
    }

    struct sockaddr_in servaddr{};
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
        std::cout << "bind failed ..." <<std::endl;
        return;
    }

    printf("Accepting connections ...\n");
    while (true) {
        struct sockaddr_in cliaddr{};
        socklen_t cliaddr_len = sizeof(cliaddr);
        //阻塞接收数据，存储客户端地址
        char buf[1024];
        char str[INET_ADDRSTRLEN];
        ssize_t nRead = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (nRead == -1){
            perror("recvfrom error");
            continue;
        }

        printf("received from %s at PORT %d\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
               ntohs(cliaddr.sin_port));

        for (int i = 0; i < nRead; i++){
             buf[i] = ::toupper(buf[i]);
        }
           
        //向客户端发送数据
        ssize_t nWrite = sendto(sockfd, buf, nRead, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        if (nWrite == -1)
            perror("sendto error");
    }

    close(sockfd);
}



int main(int argc, char* argv[]){

    udpClient_connnect("10.9.36.15", 9006);

    // udpServer(9002);


    return 0;
}