

#include <cstdio>
#include <iostream>

using namespace std;
int main(int argc, char* argv[]){

    char buf[1000]{};

    snprintf(buf, 100, "===>%s--%s--%s\n", "hello", "world", "sss");
    std::cout << buf << std::endl;


    return 0;
}












































