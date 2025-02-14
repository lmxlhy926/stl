#include "httplib.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace httplib;

int main(int argc, char* argv[])
{

    httplib::Server svr;

    svr.Get("/content", [&](const Request &req, Response &res) {
        res.set_file_content("/home/lhy/ownproject/stl/unix/test/a.webp");
      });
    
    svr.listen("0.0.0.0", 60000);


    return 0;
}






