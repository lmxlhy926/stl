#include <iostream>
#include "httplib/httplib.h"
#include "file.hpp"
#include "time.h"

using namespace std;
using namespace httplib;

void func1(){

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!sssss", "text/plain");
    });

    svr.listen("localhost", 1234);
    std::cout << "--------" << std::endl;
}

void func2(){

    httplib::Client cli("202.98.157.137", 80);

    string body = R"({"hello":"world"})";
    httplib::Headers header;
    header.emplace("User-Agent", "cpp-httplib/0.7");

    if (auto res = cli.Post("/logic-device/edge/getRules", header, body,"application/json")) {
        cout << res->status << endl;
        cout << res->get_header_value("Content-Type") << endl;
        cout << res->body << endl;
    } else {
        cout << "error code: " << res.error() << std::endl;

    }
}


using Handler = std::function<void(const string &, string &)>;
using Handlers = std::vector<std::pair<std::string, Handler>>;
Handlers handlerVector;
void func3(){
    handlerVector.push_back(std::pair<string, Handler>("---", [](const string& s1, string& s2){
        s2 = s1 + "---";
    }));
    handlerVector.push_back(std::pair<string, Handler>("---", [](const string& s1, string& s2){
        s2 = s1 + "***";
    }));
}

void func4(string uri, const string& s1, string& s2){
    for (const auto &x : handlerVector) {
        const auto &pattern = x.first;
        const auto &handler = x.second;

        if(pattern == uri){
            handler(s1, s2);
            return;
        }
    }
}

int main() {
    func1();

    return 0;
}

