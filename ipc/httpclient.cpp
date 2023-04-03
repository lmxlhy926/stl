
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

int main(int argc, char* argv[]){

    ::signal(SIGQUIT, sigHandler);

    while(true){
        sleep(10);
    }

    return 0;
}
