
#include <event2/event.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <thread>
using namespace std;

//创建event_base
void event_base_create(){
    struct event_config *cfg;
    struct event_base *base;

    //生成一个配置event_base的结构
    cfg = event_config_new();
    //调用函数配置event_config
    //禁用指定的策略、指定指定的特性、设置特殊标志
    event_config_avoid_method(cfg, "select");
    event_config_require_features(cfg, EV_FEATURE_ET);
    event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    //创建event_base
    base = event_base_new_with_config(cfg);
    //释放event_config
    event_config_free(cfg);


    //获取最终采用的方法
    printf("using libevent with backend method %s.\n", event_base_get_method(base));
    //获取目前所采用方法具有的特性
    int features = event_base_get_features(base);
    if(features & EV_FEATURE_ET){
        printf("ev_feature_et\n");

    }else if(features & EV_FEATURE_O1){
        printf("ev_feature_o1\n");

    }else if(features & EV_FEATURE_FDS){
        printf("ev_feature_fds\n");
    }

    //设置event_base优先级
    event_base_priority_init(base, 3);
    event_base_free(base);
}


struct event_base *base;
struct event *ev;

void cb_func(evutil_socket_t fd, short what, void *arg){
    const char *data = reinterpret_cast<const char*>(arg);
    printf("got an event on socket %d: %s %s [%s]\n", 
                                    fd,
                                    (what & EV_TIMEOUT) ? "timeout" : "",
                                    (what & EV_READ) ? "read" : "",
                                    data);
    if(what & EV_READ){
        char buffer[1024];
        ssize_t size  = read(fd, buffer, 1024);
        std::cout << "readContent: " << string(buffer) << std::endl;
    }
    // event_del(ev);
}


/**
 * event_base_loop()
 *      不含任何标志：
 *                  阻塞等待，有事件发生时则执行。无事件发生，则继续阻塞等待
 *      EVLOOP_ONCE: 
 *                   没有事件发生，则阻塞等待。有事件发生，则执行。没有事件发生，则退出等待。
 *                   即至少执行一次后，才会返回
 *      EVLOOP_NONBLOCK：
 *                   没有事件发生则返回。有事件发生则执行。
 * 
*/


int main(int argc, char* argv[]){
    struct timeval five_seconds = {5, 0};
    base = event_base_new();
    printf("method: %s\n", event_base_get_method(base));

    ev = event_new(base, 0, EV_TIMEOUT | EV_READ | EV_PERSIST, cb_func, (void *)"reading event");
    event_add(ev, &five_seconds);
    
    int i = 0;
    while(true){
        std::cout << "execute time %d ..." << i++ << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //没有则返回
        event_base_loop(base, EVLOOP_NONBLOCK);
        //至少有一个事件发生，执行后，没有更多的事件，则返回
        event_base_loop(base, EVLOOP_ONCE);
    }
    
    std::cout << "-----end----" << std::endl;
    return 0;
}






