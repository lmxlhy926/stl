
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

/**
 *  getaddrinfo：主机名、主机地址、服务名、服务端口号的字符串表示---> 套接字地址
 *  getnameinfo：套接字地址 ---> 主机名、主机地址、服务名、服务端口号
 *  
 *  host默认是域名或者ip，service默认是服务名或者端口号
 *  通过AI_NUMERICHOST, 或AI_NUMERICSERV可将host限制为点分十进制形式的IP，将service限制为数字端口号
 *  说明：
 *      1. 如果是指定的是域名、服务名，则首先需要转换为主机地址、端口号。域名通过DNS服务转换为主机地址，服务名通过映射转换为端口号。
 *      2. 点分十进制字符串地址转换为网络字节序地址
 *      3. 字符串端口号--> 主机字节序端口号--> 网络字节序端口号
*/
void domain2decimal(const string& host, const string& service){
    //过滤ip属性
    struct addrinfo hints{};
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          //IPv4 only
    hints.ai_socktype = SOCK_STREAM;    //Connections only

    //获取地址
    int rc, flags;
    char hostname[100], port[100];
    struct addrinfo *p, *listp;
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
void simpleTcpClient(const string& ip, uint16_t port, const string& writeMesage){
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
    if(conRet == 0){    //成功建立连接，sockfd成为完整打开的文件描述符，可进行读写
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


void simpleTcpServer(uint16_t port){
    int listenfd, ret;
    
    //创建描述符,设置地址可被重复利用
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)(&optval), sizeof(int));

    //服务器地址
    struct sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                  //指定为IPV4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //网络字节序的IP地址, 此处为监听本机所有地址
    servaddr.sin_port = htons(port);                //网络字节序的端口号

    //描述符绑定地址
    if(bind(listenfd, reinterpret_cast<const sockaddr *>(&servaddr), sizeof(servaddr)) == -1){
        printf("bind failed\n");
        return;
    }
    //将主动套接字转换为监听套接字，声明该套接字可以接受来自客户端的连接请求
    if(listen(listenfd, 20) == -1){
        printf("listen failed\n");
        return;
    }

    while(true){
        std::cout << "Accepting connections...." << std::endl;
        /**
         * accept函数等待来自客户端的连接请求到达监听描述符listenfd
         * 在addr中填写客户端的套接字地址，并返回一个已连接描述符，这个描述符可被利用UNIX IO函数与客户端通信
         */
        struct sockaddr_in cliaddr{};
        socklen_t cliaddr_len = sizeof(cliaddr);
        int connfd = accept(listenfd, reinterpret_cast<sockaddr*>(&cliaddr), &cliaddr_len);
        if(connfd == -1){
            printf("accept failed\n");
            return;
        }

        char receiveBuf[1024], ipBuf[INET_ADDRSTRLEN];
        ssize_t readCount = read(connfd, receiveBuf, 1024);
        if(readCount <= 0){
            close(connfd);
            return;
        }
        std::cout << "received from " << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipBuf, sizeof(ipBuf))
                  << " at port " << ntohs(cliaddr.sin_port) << std::endl;

        for(int i = 0; i < readCount; ++i){
            receiveBuf[i] = ::toupper(receiveBuf[i]);
        }
        write(connfd, receiveBuf, readCount);

        //关闭和客户端建立的通信端点
        close(connfd);
    }
}


/**
 * @param ip        服务器IP
 * @param port      服务器端口号
 * @return          成功返回大于0的文件描述符，失败返回-1
 * 功能：尝试和指定的服务器建立连接，返回建立连接的文件描述符
 */
int open_clientfd_tcp(string& ip, uint16_t port){
    struct addrinfo hints, *p, *listp;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;           //IPV4
    hints.ai_socktype = SOCK_STREAM;     //端点用于连接
    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;    //限制为数字形式的ip和端口号

    int retCode, clientfd;
    if((retCode = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &listp)) != 0){
        printf("%s\n", gai_strerror(retCode));  //打印错误
        return -1;
    }
    for(p = listp; p != nullptr; p = p->ai_next){
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
    if(p == nullptr){       //列表里没有有效的地址
        return -1;
    }else{
        return clientfd;    //返回成功建立的文件描述符
    }
}



/**
 * @param port   服务器要监听的端口号
 * @return       成功返回大于0的监听描述符，失败返回-1.
 *
 * 功能：
 *     返回监听描述符，监听本机所有网卡接收到的发送到端口port的连接请求
 */
int open_listenfd_tcp(uint16_t port){
    struct addrinfo hint, *p, *listp;
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_INET;           //IPV4
    hint.ai_socktype = SOCK_STREAM;     //用于连接
    hint.ai_flags = AI_NUMERICSERV | AI_PASSIVE;   //端口限制为数字形式，端口为被动端口用作监听

    /**
     * 使用AI_PASSIVE标志并将host参数设置为nullptr, 每个套接字地址结构中的地址字段会被设置为通配符地址
     * 告诉内核这个服务器接收发送到本主机上的所有IP地址的请求
     */
    int retcode;
    if((retcode = getaddrinfo(nullptr, std::to_string(port).c_str(), &hint, &listp)) != 0){
        printf("%s\n", gai_strerror(retcode));
        return -1;
    }

    int listenfd, optval = 1;
    for(p = listp; p != nullptr; p = p->ai_next){
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
            continue;
        }

        /**
         * 使得服务器能够被终止、重启和立即开始接收连接请求
         * 一个重启的服务器默认将在30秒内拒绝客户端的连接请求，这严重地阻碍了调试
        */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)(&optval), sizeof(int));

        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0){
            break;
        }
        close(listenfd);
    }

    freeaddrinfo(listp);

    if(p == nullptr)
        return -1;

    if(listen(listenfd, 20) == -1){
        close(listenfd);
        return -1;
    }

    return listenfd;
}






int main(int argc, char* argv[]){

    domain2decimal("127.0.0.1", "90");

    return 0;
}







