#include <iostream>
#include <thread>
#include "httplib.h"
#include <unistd.h>

using namespace std;
using namespace httplib;

void downloadAndUpdate(char* argv[]){
    httplib::Client cli("localhost", 8080);
    ofstream ofs(R"(D:\ownProject\httpLib_socket\update\dir\b.zip)", std::ios::app | std::ios::binary);

    std::string body;
    auto res = cli.Get("/stream",
                       [&](const char *data, size_t data_length) {
                           ofs.write(data, data_length);
                           return true;
                       });
    ofs.close();

    //todo 校验下载的包是否完整，如果不完整，重新请求下载。（分块下载的方式，从失败的地方重新开始下载）

    //todo 执行解压缩操作

    //创建新进程，执行脚本，进行升级包的替换
    pid_t pid;
    pid = fork();
    if(pid == 0){   /* 子进程 */
        execve("./update.sh", argv, environ);
        exit(0);
    }
    std::cout << "===>end in client.cpp====" << std::endl;
}


int main(int argc, char* argv[]){
    downloadAndUpdate(argv);

    return 0;
}

















