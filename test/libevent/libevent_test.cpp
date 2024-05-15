
#include <event2/event.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    int i = 0;
    const char **methods = event_get_supported_methods();
    printf("starting libevent %s. avilable methods are: \n", event_get_version());
    while(methods[i] != nullptr){
        printf("%s\n", methods[i++]);
    }


    return 0;
}



