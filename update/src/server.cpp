#include <iostream>
#include <fstream>
#include "httplib.h"

using namespace httplib;
using namespace std;


void downloadAndUpload(const string& downloadPath, const string& uploadPath){
    httplib::Server svr;
    svr.Get("/stream", [&](const Request &req, Response &res) {
        std::cout << "====>in svr.get<=====" << std::endl;

        std::shared_ptr<ifstream> ifsptr(new ifstream(downloadPath, std::ios::in | std::ios::binary));
        res.set_content_provider(
                "text/plain", // Content type
                [=](size_t offset,DataSink &sink) {
                    char data[1024 * 8]{};
                    long readCount;
                    while((readCount = ifsptr->readsome(data, 1024 * 8)) != 0){
                       sink.write(data, readCount);
                    }
                    sink.done();
                    return true; // return 'false' if you want to cancel the process.
                });
    });

    svr.Post("/content_receiver",
             [&](const Request &req, Response &res, const ContentReader &content_reader){
                 std::cout << "===>in /content_receiver" << std::endl;
                 std::shared_ptr<ofstream> ifsptr(new ofstream(uploadPath, std::ios::out | std::ios::binary));
                 content_reader([&](const char *data, size_t data_length) {
                     ifsptr->write(data, data_length);
                     return true;
                 });
             });


    svr.listen("localhost", 8080);
}


int main(int argc, char* argv[]){
    const string downloadPath = R"(/home/lhy/project/byjs/update/middlewareupdate/test/serverBag/update.zip)";
    const string uploadPath = R"(D:\project\byjs\middlewareupdate\out\upload.txt)";

    downloadAndUpload(downloadPath, uploadPath);

    return 0;
}
