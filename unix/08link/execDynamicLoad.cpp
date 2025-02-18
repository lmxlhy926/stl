#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>

typedef void (*funcPtr)(int, int);

int main(int argc, char* argv[]){
    void *handle = dlopen("/home/lhy/ownproject/stl/unix/link/build/libshared.so", RTLD_LAZY);
    if(!handle){
        fprintf(stderr, "%s\n", dlerror());
        exit(-1);
    }

    funcPtr showResultFunc  = reinterpret_cast<funcPtr>(dlsym(handle, "show"));
    char *error;
    if((error = dlerror()) != nullptr){
        fprintf(stderr, "%s\n", error);
        exit(-1);
    }

    showResultFunc(2, 100);

    if(dlclose(handle) < 0){
        fprintf(stderr, "%s\n", dlerror());
        exit(-1);
    }

    return 0;
}












