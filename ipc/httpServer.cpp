
#include "httplib/httplib.h"
#include <string>



int main(int argc, char* argv[]){
    httplib::Server svr;

    svr.Post("/", [](const httplib::Request& request, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
        std::cout << "<----------->" << std::endl;
    });

    svr.set_keep_alive_max_count(5);
    svr.set_keep_alive_timeout(100);

    svr.listen("0.0.0.0", 9999);
}








