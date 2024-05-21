
#include <event2/event.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <thread>
using namespace std;


void print_backend_property(struct event_base *base){
    //确定采用的后端方法
    printf("using libevent with backend method: %s\n", event_base_get_method(base));
    //确定后端方法所具有的特性
    int features = event_base_get_features(base);
    if(features & EV_FEATURE_ET)
        printf("backend fetature: EV_FEATURE_ET\n");
    if(features & EV_FEATURE_O1)
        printf("backend feature: EV_FEATURE_01\n");
    if(features & EV_FEATURE_FDS)
        printf("backend feature: EV_FEATURE_FDS\n");
}


/**
 * 创建event_base:
 *      后端方法：
 *              select、poll、epoll
 *      后端方法具有的属性：
 *              EV_FEATURE_ET、EV_FEATURE_FDS、EV_FEATURE_01
 *              边沿触发、支持任意文件描述符不仅是socket描述符、添加、删除、确定激活事件的时间复杂度为O(1)。
 *      事件可设定的优先级数目
*/
void event_base_create_test(){
    struct event_base *base;
    struct event_config *config;

    //构造一个配置结构
    config = event_config_new();
    //排除select方法
    event_config_avoid_method(config, "select");
    //需要满足的特性
    event_config_require_features(config, EV_FEATURE_ET);
    //创建event_base
    base = event_base_new_with_config(config);
    //释放event_config
    event_config_free(config);

    //打印后端方法及方法具有的属性
    print_backend_property(base);
    
    //设置事件优先级数目
    event_base_priority_init(base, EVENT_MAX_PRIORITIES);
    //释放event_base
    event_base_free(base);
}



struct event_base *base;
struct event *event;

/**
 * event_base_
*/
void printExitStatus(struct event_base *base){
    if(event_base_got_exit(base)){
        printf("exit...\n");
    }else if(event_base_got_break(base)){
        printf("break...\n");
    }
}


/**
 * event所具有的属性：
 *      1. 关联的event_base
 *      2. 关联的文件描述符
 *      3. 触发的条件
 *      4. 关联的回调函数
 *      5. 关联的回调函数中关联的指针参数
 *      6. 设定的优先级
*/
void getEventProperty(struct event* event){
    //返回为事件配置的event_base
    struct event_base *event_base = event_get_base(event);

    //获取为事件配置的文件描述符
    evutil_socket_t fd = event_get_fd(event);

    //返回时间的标志
    short triggerEvents = event_get_events(event);

    //返回事件配置的回调函数
    event_callback_fn fn = event_get_callback(event);

    //返回回调函数中的参数指针
    void* args = event_get_callback_arg(event);

    //获取事件的优先级
    int priority = event_get_priority(event);
}



void cb_func(evutil_socket_t fd, short what, void *arg){
    //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n",
                                    fd,
                                    (what & EV_READ) ? "read" : "",
                                    (what & EV_TIMEOUT) ? "timeout" : "",
                                    data);

    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }
}

void cb_func_eventDel(evutil_socket_t fd, short what, void *arg){
    //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n",
                                    fd,
                                    (what & EV_READ) ? "read" : "",
                                    (what & EV_TIMEOUT) ? "timeout" : "",
                                    data);

    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }

    //执行回调后，卸载事件，不再需要被触发
    event_del(event);
}


void cb_func_loopExit(evutil_socket_t fd, short what, void *arg){
    //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n",
                                    fd,
                                    (what & EV_READ) ? "read" : "",
                                    (what & EV_TIMEOUT) ? "timeout" : "",
                                    data);

    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }

    //执行回调后，卸载事件，不再需要被触发
    event_base_loopexit(base, nullptr);
}


void cb_func_loopBreak(evutil_socket_t fd, short what, void *arg){
    //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n",
                                    fd,
                                    (what & EV_READ) ? "read" : "",
                                    (what & EV_TIMEOUT) ? "timeout" : "",
                                    data);

    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }

    //执行回调后，卸载事件，不再需要被触发
    event_base_loopbreak(base);
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

void test(){
    struct timeval five_seconds = {5, 0};
    base = event_base_new();
    printf("method: %s\n", event_base_get_method(base));

    event = event_new(base, STDIN_FILENO, EV_TIMEOUT | EV_READ | EV_PERSIST, cb_func, (void *)"reading event");
    event_add(event, &five_seconds);

    int i = 0;
    while(true){
        std::cout << "execute time ..." << i++ << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //没有则返回
        event_base_loop(base, EVLOOP_NONBLOCK);
        printExitStatus(base);
        //至少有一个事件发生，执行后，没有更多的事件，则返回
        // event_base_loop(base, EVLOOP_ONCE);
    }

    std::cout << "-----end----" << std::endl;
    printExitStatus(base);
}



int main(int argc, char* argv[]){
   
    test();


    return 0;
}






