
#include "httplib/httplib.h"
#include <string>



int main(int argc, char* argv[]){
    httplib::Server svr;

    svr.Post("/", [](const httplib::Request& request, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
        std::cout << "contentReceive: " << request.body << std::endl;
    });

    svr.listen("0.0.0.0", 6666);
}








