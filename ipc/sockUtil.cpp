//
// Created by 78472 on 2022/11/27.
//

#include "sockUtil.h"
#include <vector>
#include <algorithm>
using namespace std;

/*
    *  host默认是域名或者ip，service默认是服务名或者端口号
    *  通过AI_NUMERICHOST, 或AI_NUMERICSERV可将host限制为点分十进制形式的IP，将service限制为数字端口号
    *  说明：
    *      1. 如果是指定的是域名、服务名，则首先需要转换为主机地址、端口号。域名通过DNS服务转换为主机地址，服务名通过映射转换为端口号。
    *      2. 点分十进制字符串地址转换为网络字节序地址
    *      3. 字符串端口号--> 主机字节序端口号--> 网络字节序端口号
    */
void domain2decimal(string& host, string& service){
    struct addrinfo *p, *listp, hints{};    //指向单个结构、指向列表、指示结构
    char hostname[100], port[100];
    int rc, flags;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          //IPv4 only
    hints.ai_socktype = SOCK_STREAM;    //Connections only

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


void sampleServer(uint16_t port){
    int listenfd, ret;
    int optval = 1;

    //创建描述符
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //设置地址可被重复利用
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

        struct sockaddr_in cliaddr{};
        socklen_t cliaddr_len = sizeof(cliaddr);
        /*
         * accept函数等待来自客户端的连接请求到达监听描述符listenfd
         * 在addr中填写客户端的套接字地址，并返回一个已连接描述符，这个描述符可被利用UNIX IO函数与客户端通信
         */
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


int open_clientfd(string& ip, uint16_t port){
    struct addrinfo hints, *p, *listp;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;           //IPV4
    hints.ai_socktype = SOCK_STREAM;     //端点用于连接
    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;    //限制为数字形式的ip和端口号


    int retCode;
    if((retCode = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &listp)) != 0){
        printf("%s\n", gai_strerror(retCode));  //打印错误
        return -1;
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

    if(p == nullptr){       //列表里没有有效的地址
        return -1;
    }else{
        return clientfd;    //返回成功建立的文件描述符
    }
}


int open_listenfd(uint16_t port){
    struct addrinfo hint{}, *p, *listp;
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_INET;           //IPV4
    hint.ai_socktype = SOCK_STREAM;     //用于连接
    hint.ai_flags = AI_NUMERICSERV | AI_PASSIVE;   //端口限制为数字形式，端口为被动端口用作监听

    /*
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

        //使得服务器能够被终止、重启和立即开始接收连接请求
        //一个重启的服务器默认将在30秒内拒绝客户端的连接请求，这严重地阻碍了调试
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


void server_select(uint16_t port){
    //1. 创建监听描述符，开始监听
    int listenfd = open_listenfd(port);
    if(listenfd  == -1){
        std::cout << "open_listenfd Error....";
        return;
    }

    //记录数据
    std::vector<int> connfdVec;     //记录已建立连接的描述符
    int maxFd = listenfd;           //初始化监听的最大描述符

    //构造初始监听集合，只监听连接请求
    fd_set readSet, initSet;
    FD_ZERO(&initSet);
    FD_SET(listenfd, &initSet);

    std::cout << "Start to Accept connections...." << std::endl;
    while(true){
        readSet = initSet;  //每次请求需要重新赋值
        int nReady = select(maxFd + 1, &readSet, nullptr, nullptr, nullptr);
        if(nReady == -1){
            std::cout << "select error...." << std::endl;
            return;
        }

        //如果是连接请求，则记录连接描述符
        if(FD_ISSET(listenfd, &readSet)){
            struct sockaddr_in cliAddr{};
            socklen_t clientLenth = sizeof(sockaddr_in);
            char ipStrBuffer[INET_ADDRSTRLEN];

            int connfd = accept(listenfd, reinterpret_cast<sockaddr *>(&cliAddr), &clientLenth);
            if(connfd >= 0){    //成功建立连接
                std::cout << "Request From " << inet_ntop(AF_INET, &cliAddr.sin_addr, ipStrBuffer, INET_ADDRSTRLEN) << "  "
                          << ntohs(cliAddr.sin_port) << std::endl;
                FD_SET(connfd, &initSet);       //监听该连接描述符
                connfdVec.push_back(connfd);   //连接描述符加入遍历列表
                if(connfd > maxFd)             //更新最大描述符
                    maxFd = connfd;

            }else
                std::cout << "accept Error...." << std::endl;

            if(--nReady == 0)
                continue;
        }

        //如果是数据请求，则找到相应的描述符，进行交互
        for(auto pos = connfdVec.begin(); pos != connfdVec.end();){
            if(FD_ISSET(*pos, &readSet)){   //处理该连接端点收到的数据请求
                char buf[1024];
                ssize_t nRead = read(*pos, buf, 1024);
                if(nRead <= 0){
                    std::cout << "read Error or Client closed...." << std::endl;
                    close(*pos);
                    FD_CLR(*pos, &initSet);                 //停止监听该连接节点
                    pos = connfdVec.erase(pos);     //删除会返回下一元素的位置
                    int maxElement = *max_element(connfdVec.begin(), connfdVec.end());
                    maxFd  = listenfd > maxElement ? listenfd : maxElement;
                    continue;

                }else{
                    for(int i = 0; i < nRead; ++i){
                        buf[i] = static_cast<char>(toupper(buf[i]));
                    }
                    write(*pos, buf, nRead);

                    ++pos;
                }

                if(--nReady == 0)
                    break;
            }
        }
    }
}


int main(int argc, char* argv[]){
    string ip = "172.25.240.1";
    uint16_t port = 9000;
    string message = "login";

    server_select(port);

    return 0;
}