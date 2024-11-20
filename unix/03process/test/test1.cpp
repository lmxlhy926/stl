#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    printf("%s process pid: %d\n", argv[0], getpid());
    
    for(int i = 0; i < argc; ++i){
        printf("arg[%d]: %s\n", i, argv[i]);
    }

    for(int i = 0; environ[i] != nullptr; ++i){
        printf("environ[%d]: %s\n", i, environ[i]);
    }

    return 0;
}










