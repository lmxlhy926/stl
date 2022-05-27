#include <iostream>
#include <thread>
#include <utility>
#include "socket/httplib/httplib.h"
#include <unistd.h>

using namespace std;
using namespace httplib;

namespace update{

    class httpDownload{
    private:
        const string serverIp_;         //服务器ip
        const int serverPort_;          //服务器端口号

    public:
        httpDownload(string  svrIp, int svrPort) :
                serverIp_(std::move(svrIp)),
                serverPort_(svrPort){};

        bool download(const string& requestUrl, const string& filePath);
    };


    class httpUpload{
    private:
        const string serverIp_;         //服务器ip
        const int serverPort_;          //服务器端口号

    public:
        httpUpload(string  svrIp, int svrPort) :
                serverIp_(std::move(svrIp)),
                serverPort_(svrPort){};

        bool upload(const string& requestUrl, const string& filePath);
    };


}//namespace update




