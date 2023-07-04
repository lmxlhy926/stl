


#include <event2/event.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
using namespace std;

// 检查event_base支持的后端方法
void checkBackendMethods(void){
    const char**methods = event_get_supported_methods();
    printf("Starting Libevent %s. Available methods are:\n", event_get_version());
    for(int i = 0; methods[i] != nullptr; ++i){
        printf("%s\n", methods[i]);
    }
}

void whatPrint(short what, const char* str){
    printf("%s: ", str);
    if(what & EV_TIMEOUT){
        printf("EV_TIMEOUT ");
    }
    if(what & EV_READ){
        printf("EV_READ ");
    }
    if(what & EV_WRITE){
        printf("EV_WRITE ");
    }
    if(what & EV_SIGNAL){   
        printf("EV_SIGNAL ");
    }
    if(what & EV_PERSIST){  
        printf("EV_PERSIST ");
    }
    if(what & EV_ET){
        printf("EV_ET ");
    }
    printf("\n");
}

// 检测标准输入是否有内容写入，如果有则读取其内容
using stdinCallbackType = void(*)(evutil_socket_t fd, short what, void* arg);

void stdinCallbackFunc1(evutil_socket_t fd, short what, void *arg){
    whatPrint(what, "stdinCallbackFunc1");
    char buf[1024]{};
    int readBytes = read(fd, buf, sizeof(buf));
    std::cout << "func1Read: " << string(buf, readBytes);
    std::cout << "---------" << std::endl;
}

void stdinCallbackFunc2(evutil_socket_t fd, short what, void *arg){
    whatPrint(what, "stdinCallbackFunc2");
    char buf[1024]{};
    int readBytes = read(fd, buf, sizeof(buf));
    std::cout << "func2Read: " << string(buf, readBytes);
    std::cout << "---------" << std::endl;
}

void stdinCallbackFunc3(evutil_socket_t fd, short what, void *arg){
     whatPrint(what, "stdinCallbackFunc3");
    char buf[1024]{};
    int readBytes = read(fd, buf, sizeof(buf));
    std::cout << "func3Read: " << string(buf, readBytes);
    std::cout << "---------" << std::endl;
}
    
    

void checkStdin(){
    //创建event_base
    struct event_base *base = event_base_new();
    
    //创建event_base的关联事件，指明事件的触发条件、指明事件触发后要执行的函数、指定函数的参数。
    struct event *ev1 = event_new(base, STDIN_FILENO, EV_READ | EV_TIMEOUT | EV_PERSIST, stdinCallbackFunc1, nullptr);
    struct event *ev2 = event_new(base, STDIN_FILENO, EV_READ | EV_TIMEOUT | EV_PERSIST, stdinCallbackFunc2, nullptr);
    struct event *ev3 = event_new(base, STDIN_FILENO, EV_READ | EV_TIMEOUT | EV_PERSIST, stdinCallbackFunc3, nullptr);

    //添加事件，使其成为未决事件
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    event_add(ev1, nullptr);
    event_add(ev2, nullptr);
    event_add(ev3, nullptr);

    printf("start to detect stdin...\n");
    //循环监听事件
    event_base_dispatch(base);

    //释放事件内容
    event_free(ev1);
    event_free(ev2);
    event_free(ev3);


    //释放event_base内容
    event_base_free(base);
}



int main(int argc, char* argv[]){
    checkStdin();

    return 0;
}






















