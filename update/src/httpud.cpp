
#include "httpud.hpp"

namespace update{

    bool httpDownload::download(const string& requestUrl, const string& filePath){
        remove(filePath.c_str());
        httplib::Client cli(serverIp_, serverPort_);
        //以追加方式打开文件
        ofstream ofs(filePath, std::ios::app | std::ios::binary);
        if(!ofs.is_open()){
            std::cout << "failed to open " << filePath << std::endl;
            return false;
        }
        auto res = cli.Get(requestUrl.c_str(),
                           [&](const char *data, size_t data_length) {
                               ofs.write(data, static_cast<long>(data_length));
                               return true;
                           });

        if(res != nullptr && (res->status == 200))  return true;
        return false;
    }


    bool httpUpload::upload(const string& requestUrl, const string& filePath){
        std::shared_ptr<ifstream> ifsptr(new ifstream(filePath, std::ios::in | std::ios::binary));
        if(!ifsptr->is_open()){
            std::cout << "===>failed to open " << filePath << std::endl;
            return false;
        }
        httplib::Client cli(serverIp_, serverPort_);
        auto res = cli.Post(requestUrl.c_str(),
                            [&](size_t offset, DataSink &sink) {
                                char data[1024 * 4];
                                long readCount;
                                while((readCount = ifsptr->readsome(data, 1024 * 4)) != 0){
                                    sink.write(data, readCount);
                                }
                                sink.done();
                                return true; // return 'false' if you want to cancel the request.
                            },
                            "text/plain");
        if((res != nullptr) && res->status == 200)  return true;
        return false;
    }

}





