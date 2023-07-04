

#include <cstring>

#include <iostream>
#include <string>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

int main(int argc, char* argv[]){
    string ip = "127.0.0.1";
    int port = 6666;
    string writeMesage = "hello world";

//服务器地址
    struct sockaddr_in servaddr{};      //IPV4地址结构
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;      //指定为IPV4地址族
    inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr.s_addr);  //点分十进制ip地址--->网络字节序二进制地址
    servaddr.sin_port = htons(port);    //主机字节序端口号--->网络字节序端口号

    //创建套接字描述符
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("sockfd");
        return -1;
    }

    //阻塞，一直到连接成功建立或者发生错误
    if(connect(sockfd, reinterpret_cast<const struct sockaddr* >(&servaddr), sizeof(servaddr)) < 0){
        perror("connect");
        return -1;
    }

    while(true){    //成功建立连接，sockfd成为完整打开的文件描述符，可进行读写
//        write(sockfd, writeMesage.c_str(), writeMesage.size());
        printf("client...\n");
        sleep(5);

//        char buf[1024];
//        ssize_t nRead = read(sockfd, buf,  1024);
//        if(nRead <= 0){
//            close(sockfd);
//            return 0;
//        }
//        std::cout << "Response from server: ";
//        std::cout.flush();
//        write(STDOUT_FILENO, buf, nRead);
    }


    close(sockfd);
    return 0;
}


