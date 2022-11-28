//
// Created by 78472 on 2022/11/27.
//

#include "client.h"

void getaddrinfo_getnameinfo(string& host, string& service){
    struct addrinfo *p, *listp, hints{};    //指向单个结构、指向列表、指示结构
    char hostname[100], port[100];
    int rc, flags;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          //IPv4 only
    hints.ai_socktype = SOCK_STREAM;    //Connections only
    /*
     *  host默认是域名或者ip，service默认是服务名或者端口号
     *  通过AI_NUMERICHOST, 或AI_NUMERICSERV可将host限制为点分十进制形式的IP，将service限制为数字端口号
     *  说明：
     *      1. 如果是指定的是域名、服务名，则首先需要转换为主机地址、端口号。域名通过DNS服务转换为主机地址，服务名通过映射转换为端口号。
     *      2. 点分十进制字符串地址转换为网络字节序地址
     *      3. 字符串端口号--> 主机字节序端口号--> 网络字节序端口号
     */
//    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

    //获取地址
    if((rc = getaddrinfo(host.c_str(), service.c_str(), &hints, &listp)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(-1);
    }

    flags = NI_NUMERICHOST | NI_NUMERICSERV;    //显示数字字符串而非域名和服务名
    for(p = listp; p != nullptr; p = p->ai_next){
        getnameinfo(p->ai_addr, p->ai_addrlen, hostname, 100, port, 100, flags);
        printf("hostname: %s, port: %s\n", hostname, port);
    }

    //释放列表
    freeaddrinfo(listp);
}


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

void simpleClient_1(string& ip, uint16_t port, string& writeMesage){
    struct addrinfo hint{};
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_INET;           //IPV4
    hint.ai_socktype = SOCK_STREAM;     //端点用于连接
    hint.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;    //限制为数字形式的ip和端口号

    struct addrinfo *p, *listp;
    int retCode;
    if((retCode = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hint, &listp)) != 0){
        printf("%s\n", gai_strerror(retCode));  //打印错误
        return;
    }

    int clientfd;
    for(p =  listp; p != nullptr; p= p->ai_next){
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
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
        printf("no valid address\n");
        return;
    }else{
        write(clientfd, writeMesage.c_str(), writeMesage.size());

        char buf[1024];
        ssize_t nRead = read(clientfd, buf,  1024);
        if(nRead <= 0){
            close(clientfd);
            return;
        }
        std::cout << "Response from server: ";
        std::cout.flush();
        write(STDOUT_FILENO, buf, nRead);

        close(clientfd);
    }
}



int main(int argc, char* argv[]){
    string ip = "172.25.240.1";
    uint16_t port = 60000;
    string message = "login";

    simpleClient_1(ip, port, message);

    return 0;
}