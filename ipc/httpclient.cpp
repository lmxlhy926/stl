
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
    int conRet = connect(sockfd, reinterpret_cast<const struct sockaddr* >(&servaddr), sizeof(servaddr));
    if(conRet == 0){    //成功建立连接，sockfd成为完整打开的文件描述符，可进行读写
        std::cout << "established...." << std::endl;
        sleep(30);
        write(sockfd, writeMesage.c_str(), writeMesage.size());
        std::cout << "write end...." << std::endl;
        sleep(30);
        while(true){
            std::cout << "---start to read---" << std::endl;
            char buf[1024];
            ssize_t nRead = read(sockfd, buf,  1024);
            if(nRead <= 0){
                std::cout << "read Error......" << std::endl;
                close(sockfd);
                return;
            }
            std::cout << "Response from server: ";
            std::cout << string(buf, nRead) << std::endl;
            sleep(20);
        }
    }

    close(sockfd);
}





int main(int argc, char* argv[]){

    ::signal(SIGQUIT, sigHandler);

    tcpClient("127.0.0.1", 9999, string("hello"));


    while(true){
        sleep(10);
    }

    return 0;
}
