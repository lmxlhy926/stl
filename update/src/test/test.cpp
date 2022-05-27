#include "httpud.hpp"
#include "sys/wait.h"
extern  "C" {
#include "miniunz.h"
}

using namespace std;
using namespace update;


/**
 * @param shellPath     可执行脚本名
 * @param argv          命令行参数
 * @return              替换操作是否成功
 */
bool substitute(const char* shellPath, char* argv[]){
    pid_t pid;
    pid = fork();
    if(pid == 0){   /* 子进程执行替换脚本 */
        execve(shellPath, argv, environ);
        return false;
    }
    int status;
    if(waitpid(pid, &status, 0) > 0){   //等待子进程执行完毕
        if(WIFEXITED(status)){
            return true;
        }else{
            return false;
        }
    }
    return false;
}


void downloadtest(char* argv[]){
    string serverIp = "localhost";
    int serverPort = 8080;
    const string requsetUrl = "/stream";
    const string filePath = R"(/home/lhy/project/byjs/update/middlewareupdate/test/midBag/update.zip)";
    const string shellPath = "/home/lhy/project/byjs/update/middlewareupdate/shell/update.sh";

    httpDownload hd(serverIp, serverPort);
    if(hd.download(requsetUrl, filePath)){
        std::cout << "===>download success" << std::endl;
        char *argument[5] = {const_cast<char*>("executable"),
                             const_cast<char*>("-o"),
                             const_cast<char*>(filePath.c_str()),
                             const_cast<char*>("-d"),
                             const_cast<char*>("/home/lhy/project/byjs/update/middlewareupdate/test/midBag")};
        unzip(4, argument);
        substitute(shellPath.c_str(), argv);
    }else{
        std::cout << "===>download failed" << std::endl;
    }
}


void uploadtest(){
    string serverIp = "localhost";
    int serverPort = 8080;
    string requsetUrl = "/content_receiver";
    const string uploadPath = R"(D:\project\byjs\middlewareupdate\out\a.txt)";

    httpUpload hu(serverIp, serverPort);
    if(hu.upload(requsetUrl, uploadPath)){
        std::cout << "===>upload success" << std::endl;
    }else{
        std::cout << "===>upload failed" << std::endl;
    }
}



int main(int argc, char* argv[]){
    downloadtest(argv);

    return 0;
}

