#include "httplib.h"

using namespace httplib;

int main(int argc, char* argv[])
{

    httplib::Server svr;

    svr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
        auto numbers = req.matches[1];
        res.set_content(numbers, "text/plain");
    });

    svr.listen("0.0.0.0", 60000);


    return 0;
}






