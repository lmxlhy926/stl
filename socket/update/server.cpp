#include <iostream>
#include <fstream>
#include "httplib.h"


using namespace httplib;
using namespace std;

int main(int argc, char* argv[]){

    httplib::Server svr;
    const size_t DATA_CHUNK_SIZE = 4;

    svr.Get("/stream", [&](const Request &req, Response &res) {
        std::cout << "====>in svr.get<=====" << std::endl;
        //打开文件
        std::shared_ptr<ifstream> ifsptr(new ifstream(R"(D:\ownProject\httpLib_socket\update\dir\a.zip)", std::ios::in));

        res.set_content_provider(
                "text/plain", // Content type
                [=](size_t offset,DataSink &sink) {
                    char c;
                    if(ifsptr->get(c)){
                        sink.write(&c, 1);
                    }else{
                        sink.done();
                    }
                    return true; // return 'false' if you want to cancel the process.
                });
    });

    svr.listen("localhost", 8080);
    std::cout << "----server main----" << std::endl;

    return 0;
}
