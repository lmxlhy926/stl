
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/epoll.h>

using namespace std;


/**
 * getnameinfo：套接字地址 ---> 主机名、主机地址、服务名、服务端口号
*/
string getNameInfo(struct sockaddr* ai_addr, socklen_t addrlen){
    int flags = NI_NUMERICHOST | NI_NUMERICSERV;  //显示数字字符串而非域名和服务名
    char hostname[100]{}, port[100]{};
    getnameinfo(ai_addr, addrlen, hostname, 100, port, 100, flags);
    return string().append(string(hostname)).append(" : ").append(string(port));
}


/**
 *  getaddrinfo：主机名、主机地址、服务名、服务端口号的字符串表示---> 套接字地址
 *  host默认是域名或者ip，service默认是服务名或者端口号
 *  通过AI_NUMERICHOST, 或AI_NUMERICSERV可将host限制为点分十进制形式的IP，将service限制为数字端口号
 *  说明：
 *      1. 如果是指定的是域名、服务名，则首先需要转换为主机地址、端口号。域名通过DNS服务转换为主机地址，服务名通过映射转换为端口号。
 *      2. 点分十进制字符串地址转换为网络字节序地址
 *      3. 字符串端口号--> 主机字节序端口号--> 网络字节序端口号
*/
void domain2decimal(const string& host, const string& service){
    //填充过滤属性
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          //IPv4 only
    hints.ai_socktype = SOCK_STREAM;    //Connections only

    //获取地址列表
    int rc;
    struct addrinfo *p, *listp;
    if((rc = getaddrinfo(host.c_str(), service.c_str(), &hints, &listp)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        return;
    }
    for(p = listp; p != nullptr; p = p->ai_next){
        fprintf(stdout, "%s\n", getNameInfo(p->ai_addr, p->ai_addrlen).c_str());
    }

    //释放列表
    freeaddrinfo(listp);
    return;
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


/*
 *  要创建一个数组，存储已建立连接的描述符。
 *  当select返回时：
 *          1. 如果是到达listenfd的新连接请求，则调用accept函数在服务端创建连接描述符，监听该描述符并将其加入通信描述符集合中
 *          2. 如果是已建立连接的客户端端点发送的数据请求，则找到对应的服务器通信端点，与之进行交互
 *             如果读取到的数据长度为0，则表明客户端关闭了连接，服务器也应该关闭该连接，并从数组和监听集合中剔除该描述符。
 */
void tcpServer_select(uint16_t port){
    //1. 创建监听描述符，开始监听
    int listenfd = open_listenfd_tcp(port);
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
                    close(*pos);                        //关闭服务器端的连接节点
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




/*
 * struct pollfd:
 *      1. fd：      指明要监听的端口， -1代表忽略该项，下次返回时把revents设置为0
 *      2. events：  指明要监听的事件，可以是读、写、或其它事件
 *      3. revents： poll函数返回的监听事件中已发生的事件
 *
 * select和poll对比:
 *      select的缺点:
 *          1. select中用三个形参来描述三种不同的事件，针对每种事件传入待监听的描述符集合，因此有三个描述符集合。
 *          2. select中的参数为传入传出参数，函数每次返回都会覆盖监听集合，所以每次调用需要重新赋值。
 *      poll的优点:
 *          poll中用一个属性来指定，可以一次性指定一个描述符监听的所有事件。
 *          poll中用events, revents将监听事件和返回事件分开，只需进行一次赋值
 */
void tcpServer_poll(uint16_t port){
    int listenfd = open_listenfd_tcp(port);
    if(listenfd == -1){
        printf("Create Listenfd Error.....\n");
        return;
    }

    //监听连接请求
    struct pollfd connFdArray[1024];   //监听描述符数组
    connFdArray[0].fd = listenfd;
    connFdArray[0].events = POLLRDNORM;

    //fd设置为-1，poll不再监控此pollfd, 下次返回时把revents设置为0.
    for(int i = 1; i < 1024; ++i){
        connFdArray[i].fd = -1;
    }

    int maxIndex = 0;

    while(true){
        int nReady = poll(connFdArray, maxIndex + 1, -1);
        if(nReady == -1){
            std::cout << "poll Error..." << std::endl;
            return;
        }

        //如果是连接请求，则在服务器建立通信端点，并监听该端点是否有数据到达
        if(connFdArray[0].revents & POLLRDNORM){
            std::cout << "connection request....." << std::endl;
            struct sockaddr_in cliAddr{};
            socklen_t cliAddrSize = sizeof(sockaddr_in);
            int connfd = accept(listenfd, reinterpret_cast<struct sockaddr *>(&cliAddr),&cliAddrSize);
            if(connfd == -1){
                printf("Accetp Error.....\n");
            }else{
                char ipStringBuffer[INET_ADDRSTRLEN];
                std::cout << "Request From: " << inet_ntop(AF_INET, &cliAddr.sin_addr.s_addr, ipStringBuffer, INET_ADDRSTRLEN) << " "
                          << ntohs(cliAddr.sin_port) << std::endl;

                for(int i = 1; i < 1024; ++i){
                    if(connFdArray[i].fd == -1){    //存储建立的连接描述符，并监听该描述符
                        connFdArray[i].fd = connfd;
                        connFdArray[i].events = POLLRDNORM;
                        maxIndex = std::max(maxIndex, i);
                        break;
                    }
                    if(i == 1024 - 1){
                        printf("Too many clients, Refused connection.....\n");
                        close(connfd);
                    }
                }
            }

            if(--nReady == 0)
                continue;
        }

        //处理数据请求
        for(int i = 1; i < 1024; ++i){
            if(connFdArray[i].fd == -1)
                continue;

            if(connFdArray[i].revents & POLLRDNORM){    //相对应的客户端有数据到来
                char buf[1024];
                ssize_t nRead = read(connFdArray[i].fd, buf, 1024);
                if(nRead <= 0){   //客户端主动关闭连接
                    printf("the peer client close the connection....\n");
                    close(connFdArray[i].fd);
                    connFdArray[i].fd = -1;     //不再监听该端口
                }else{  //正常读取到数据
                    for(int j = 0; j < nRead; ++j){
                        buf[j] = static_cast<char>(toupper(buf[j]));
                    }
                    write(connFdArray[i].fd, buf, nRead);
                }

                if(--nReady == 0)
                    continue;
            }
        }
    }
}


/*
指定监听多少个描述符
int epoll_create(int size)
    创建一个句柄，size指明该句柄可监听的最大的描述符数量

指定监听哪些描述符的哪些事件
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
    epfd：epoll_create创建的句柄
    op：
        EPOLL_CTL_ADD：	添加监测的描述符，以及该描述符要监测的事件
        EPOLL_CTL_DEL： 不再监听指定的描述符
        EPOLL_CTL_MOD：	修改指定的描述符要监测的事件
    fd： 	描述符
    event：	指向描述监测事件的结构体

监听描述符，接收返回事件集合
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
    epfd：		创建的句柄
    events：	存储返回事件数组的首地址
    maxevents：	存储数组的可容纳数量
    timeout：	等待时间：
                    -1：	阻塞；
                    0：		立马返回;
                    >0：	阻塞特定的时间

与poll以及select的区别：
    1. 拆分为3个函数，逻辑更加清晰
            创建句柄：	指定最大监测数
            句柄控制：	注册、删除、修改描述符以及对应的事件
            等待事件：	返回值指示发生的事件的数量，将发生的事件存储到提供的数组中

    2. 	除了返回值指示发生的事件的数量外，内核还将发生的事件加入到提供的数组中，这样只需遍历返回数组集合即可。
        无需遍历整个被监听的描述符集合，节省了时间。
 */
void tcpServer_epoll(uint16_t port){
    int listenfd = open_listenfd_tcp(port);
    if(listenfd == -1){
        printf("Create Listenfd Error.....\n");
        return;
    }

    std::vector<int> connfdVec{};		    //存储建立连接的connfd
    struct epoll_event events{};		    //指定监听事件、用户标识数据
    struct epoll_event backEvents[1024];	//存储返回的事件

    //创建一个epoll句柄，指定监听的文件描述符的最大个数
    int efd = epoll_create(1024);
    if (efd == -1){
        printf("epoll_create\n");
        return;
    }

    //监听listenfd是否有连接请求到达
    events.events = EPOLLIN;
    events.data.fd = listenfd;

    int res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &events);	//监听listenfd描述符
    if (res == -1){
        printf("epoll_ctl\n");
        return;
    }

    while (true) {
        int nready = epoll_wait(efd, backEvents, 1024, -1); 	 //阻塞监听
        if (nready == -1){
            printf("epoll_wait\n");
            return;
        }

        for (int i = 0; i < nready; i++) {		//只需遍历返回的事件集合即可，无需遍历全部描述符集合
            if (!(backEvents[i].events & EPOLLIN))
                continue;

            if (backEvents[i].data.fd == listenfd) {	//连接请求
                struct sockaddr_in cliaddr{};
                socklen_t clilen = sizeof(cliaddr);
                char str[INET_ADDRSTRLEN];

                int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
                printf("received from %s at PORT %d\n",
                       inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                       ntohs(cliaddr.sin_port));

                if (connfdVec.size() >= 1024 - 1){
                    printf("too many clients\n");
                    return;
                }
                connfdVec.push_back(connfd);

                events.events = EPOLLIN;		//将connfd加入监听
                events.data.fd = connfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &events);
                if (res == -1){
                    printf("epoll_ctl\n");
                    exit(-1);
                }

            } else {	//普通数据
                int sockfd = backEvents[i].data.fd;
                char buf[1024];
                ssize_t n = read(sockfd, buf, 1024);

                if (n == 0) {
                    auto pos = find(connfdVec.begin(), connfdVec.end(), sockfd);
                    if(pos != connfdVec.end()){
                        connfdVec.erase(pos);
                    }

                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, nullptr);	//不再监听该连接
                    if (res == -1){
                        printf("epoll_ctl\n");
                        exit(-1);
                    }

                    close(sockfd);

                } else {
                    for (int j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    write(sockfd, buf, n);
                }
            }
        }
    }

    close(listenfd);
    close(efd);
}





int main(int argc, char* argv[]){

    domain2decimal("127.0.0.1", "90");

    return 0;
}







