//
// Created by 78472 on 2022/11/27.
//

#include "client.h"


/*
 *  连接服务器，发送消息，接收一次返回并输出到标准输出中
 */
void simpleClient(string& ip, uint16_t port, string& writeMesage){
    //服务器地址
    struct sockaddr_in servaddr{};      //IPV4地址结构
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;      //指定为IPV4地址族
    inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr.s_addr);  //点分十进制ip地址--->网络字节序二进制地址
    servaddr.sin_port = htons(port);    //主机字节序端口号--->网络字节序端口号

    //创建套接字描述符
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "sockfd: " << sockfd << std::endl;

    //阻塞，一直到连接成功建立或者发生错误
    int conRet = connect(sockfd, reinterpret_cast<const struct sockaddr* >(&servaddr), sizeof(servaddr));
    if(conRet == 0){    //成功建立连接
        write(sockfd, writeMesage.c_str(), writeMesage.size());

        char buf[1024];
        ssize_t nRead = read(sockfd, buf,  1024);
        if(nRead <= 0){
            close(sockfd);
            return;
        }
        std::cout << "Response from server: ";
        std::cout.flush();
        write(STDOUT_FILENO, buf, nRead);
    }

    close(sockfd);
}



int main(int argc, char* argv[]){
    string ip = "172.25.240.1";
    uint16_t port = 60000;
    string message = "hello world";

    simpleClient(ip, port, message);

    return 0;
}