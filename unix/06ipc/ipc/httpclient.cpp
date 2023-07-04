
#include "httplib/httplib.h"
#include <map>
#include <string>
#include <csignal>

using namespace std;
using namespace httplib;

std::map<string, std::unique_ptr<httplib::Client>> clientMap;

void sendRequest(string& ip, int port, string& message){
    string iport;
    iport.append(ip).append(":").append(std::to_string(port));
    if(clientMap.find(iport) == clientMap.end()){
        std::cout << "insert...." << std::endl;
        auto cli = new Client(ip, port);
        cli->set_keep_alive(true);
        clientMap.insert(std::make_pair(iport, cli));
    }
    clientMap.find(iport)->second->Post("/", message, "text/json");
}


//void sendRequest(string& message){
//    string ip = "127.0.0.1";
//    int port = 6666;
//    httplib::Client client(ip, port);
//    client.set_keep_alive(true);
//    httplib::Result result =  client.Post("/", message, "text/json");
//}


void sigHandler(int){
    string message = "hello";
    string ip = "127.0.0.1";
    int port = 9999;
    time_t sendStart = time(nullptr);
    for(int i = 0; i < 100; i++){
        time_t start = time(nullptr);
        sendRequest(ip, port, message);
        time_t end = time(nullptr);
        std::cout << "--interval: " << end - start << std::endl;
    }
    time_t sendEnd = time(nullptr);
    std::cout << "---------->sendEdn: " << sendEnd - sendStart << std::endl;
}


void readHandler(int){
    char buf[1024];
    ssize_t nRead = read(3, buf,  1024);
    if(nRead < 0){
        std::cout << "server socket was closed...." << std::endl;
    }else if(nRead == 0){
        std::cout << "no data was read...." << std::endl;
    }else{
        std::cout << "Response from server: ";
        std::cout << string(buf, nRead) << std::endl;
    }
}

void writeHandler(int){
    ssize_t writeCount = write(3, "hell", 4);
    std::cout << "writeCount: " << writeCount << std::endl;
}

void quitHandler(int){
    shutdown(3, SHUT_RD);
}


void tcpClient(const string& ip, uint16_t port, const string& writeMesage){
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
    std::cout << time(nullptr) << std::endl;
    int conRet = connect(sockfd, reinterpret_cast<const struct sockaddr* >(&servaddr), sizeof(servaddr));
    std::cout << time(nullptr) << std::endl;
    if(conRet == 0){    //成功建立连接，sockfd成为完整打开的文件描述符，可进行读写
        std::cout << "connect successfully....." << std::endl;
        while(true){
            sleep(20);
        }
    }
    std::cout << "connect failed ...." << std::endl;
    close(sockfd);
}


/*
    SHUT_WR: 关闭写操作，向对端发送停止报文
    SHUT_RD: 关闭读操作，依然可以向对端写数据。双方依然处于建立连接状态
 */
int main(int argc, char* argv[]){
    ::signal(SIGUSR1, readHandler);
    ::signal(SIGUSR2, writeHandler);
    ::signal(SIGQUIT, quitHandler);


    tcpClient("172.18.227.77", 9999, string("hell"));
    return 0;
}
