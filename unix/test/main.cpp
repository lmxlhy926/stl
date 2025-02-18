#include "httplib.h"
#include <iostream>
#include <thread>

using namespace httplib;


void client_func(){
    httplib::Client cli("http://127.0.0.1:55555");

    std::cout << "---------" << std::endl;

    auto res = cli.Get("/hi");

    std::cout << "res->status: " << res->status << std::endl;

    std::cout << "res->body: " << res->body << std::endl;
}


void server_func(){
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request& request, httplib::Response& res){
        res.set_content("hello world", "text/plain");
    });

    svr.listen("0.0.0.0", 55555);
}


int main(int argc, char* argv[])
{
    std::thread th1(server_func);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::thread th2(client_func);

    th1.join();
    th2.join();
   
    return 0;
}






