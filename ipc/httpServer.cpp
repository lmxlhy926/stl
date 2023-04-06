
#include "httplib/httplib.h"
#include <string>
using namespace std;

void readHandler(int){
    char receiveBuf[1024];
    ssize_t readCount = read(4, receiveBuf, 1024);
    if(readCount < 0){
        std::cout << "read failed...." << std::endl;
        return;
    }else if(readCount == 0){
        std::cout << "no data to read..." << std::endl;
    }else{
        std::cout << "read content: " << string(receiveBuf, readCount) << std::endl;
    }
}

void writeHandler(int){
    ssize_t writeCount = write(4, "server", 6);
    std::cout << "writeCount: " << writeCount << std::endl;
}

void quitHandler(int){
    shutdown(4, SHUT_RD);
}

void socketServer(){
    int optval = 1;

    //创建描述符
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //设置地址可被重复利用
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)(&optval), sizeof(int));

    //服务器地址
    struct sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                  //指定为IPV4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //网络字节序的IP地址, 此处为监听本机所有地址
    servaddr.sin_port = htons(9999);                //网络字节序的端口号

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
        std::cout << "connfd: " << connfd << std::endl;
        if(connfd == -1){
            printf("accept failed\n");
            continue;
        }

        //连接建立成功

        while(true){
            sleep(20);
        }
    }
}

int main(int argc, char* argv[]){
//    httplib::Server svr;
//
//    svr.Post("/", [](const httplib::Request& request, httplib::Response &res) {
//        res.set_content("Hello World!", "text/plain");
//        std::cout << "<----------->" << std::endl;
//    });
//
//    svr.set_keep_alive_max_count(100);
//    svr.set_keep_alive_timeout(100);
//
//    svr.listen("0.0.0.0", 9999);


    ::signal(SIGUSR1, readHandler);
    ::signal(SIGUSR2, writeHandler);
    ::signal(SIGQUIT, quitHandler);
    socketServer();
    return 0;
}








