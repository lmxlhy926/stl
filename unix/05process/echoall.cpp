
#include <cstdio>
#include <unistd.h>

int main(int argc, char* argv[]){
    for(int i = 0; i < argc; ++i){
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    // for(int i = 0; *(environ + i) != nullptr; ++i){
    //     printf("%s\n", *(environ + i));
    // }

    return 0;
}








