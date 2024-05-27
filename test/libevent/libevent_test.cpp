
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <unistd.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

using namespace std;

/**
 * select:  EV_FEATURE_FDS
 * poll:    EV_FEATURE_FDS
 * epoll:   EV_FEATURE_ET | EV_FEATURE_01
*/
void print_backend_property(struct event_base *base){
    //确定采用的后端方法
    printf("using libevent with backend method: %s\n", event_base_get_method(base));
    //确定后端方法所具有的特性
    int features = event_base_get_features(base);
    if(features & EV_FEATURE_ET)
        printf("backend feature: EV_FEATURE_ET\n");
    if(features & EV_FEATURE_O1)
        printf("backend feature: EV_FEATURE_01\n");
    if(features & EV_FEATURE_FDS)
        printf("backend feature: EV_FEATURE_FDS\n");
}


/**
 * event_base_loopexit
 * event_base_loopbreak
 *      判断是哪个函数导致的退出
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
void printEventProperty(struct event* event){
    //返回为事件配置的event_base
    struct event_base *event_base = event_get_base(event);

    //获取为事件配置的文件描述符
    evutil_socket_t fd = event_get_fd(event);

    //返回触发条件
    short triggerEvents = event_get_events(event);

    //返回事件配置的回调函数
    event_callback_fn fn = event_get_callback(event);

    //返回回调函数中的参数指针
    void* args = event_get_callback_arg(event);

    //获取事件的优先级
    int priority = event_get_priority(event);


    printf("event_base pointer: %p\n", event_base);
    printf("fd: %d\n", fd);
    printf("trigger events: %s %s %s %s %s %s\n",   (triggerEvents & EV_TIMEOUT) ? "EV_TIMEOUT" : "",
                                                    (triggerEvents & EV_READ) ? "EV_READ" : "",
                                                    (triggerEvents & EV_WRITE) ? "EV_WRITE" : "",
                                                    (triggerEvents & EV_SIGNAL) ? "EV_SIGNAL" : "",
                                                    (triggerEvents & EV_PERSIST) ? "EV_PERSIST" : "",
                                                    (triggerEvents & EV_ET) ? "EV_ET" : "");
    printf("callback pointer: %p\n", fn);
    printf("callback args: %p\n", args);
    printf("priority: %d\n", priority);
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

//回调函数
void cb_func(evutil_socket_t fd, short what, void *arg){
    //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n", fd,
                                                        (what & EV_READ) ? "read" : "",
                                                        (what & EV_TIMEOUT) ? "timeout" : "",
                                                         data);

    //如果是读取事件导致的触发，则读取文件
    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }
}


//执行后，卸载事件
void cb_func_eventDel(evutil_socket_t fd, short what, void *arg){
   //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n", fd,
                                                        (what & EV_READ) ? "read" : "",
                                                        (what & EV_TIMEOUT) ? "timeout" : "",
                                                         data);

    //如果是读取事件导致的触发，则读取文件
    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }

    //执行回调后，卸载事件，不再需要被触发
    event_del(event);
}


//执行后，退出监听
void cb_func_loopExit(evutil_socket_t fd, short what, void *arg){
    //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n", fd,
                                                        (what & EV_READ) ? "read" : "",
                                                        (what & EV_TIMEOUT) ? "timeout" : "",
                                                         data);

    //如果是读取事件导致的触发，则读取文件
    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }

    //执行回调后，卸载事件，不再需要被触发
    struct timeval wait_seconds = {3, 0};
    event_base_loopexit(base, &wait_seconds);
}


//执行后，退出监听
void cb_func_loopBreak(evutil_socket_t fd, short what, void *arg){
   //回调函数中指定的参数转换
    const char *data = reinterpret_cast<const char *>(arg);
    printf("got an event on socket  %d : %s %s [%s]\n", fd,
                                                        (what & EV_READ) ? "read" : "",
                                                        (what & EV_TIMEOUT) ? "timeout" : "",
                                                         data);

    //如果是读取事件导致的触发，则读取文件
    if(what & EV_READ){
        char buffer[1024]{};
        read(fd, buffer, 1024);
        printf("readContent: %s\n", buffer);
    }

    //执行回调后，卸载事件，不再需要被触发
    event_base_loopbreak(base);
}



/**
 * event_base_dispatch(base)
 *      阻塞等待event被触发，如果没有挂载的event则退出
 *      如果不指定EV_PERSIST，则event被触发一次后就会自动卸载，除非在回调里再次调用event_add。
*/
void event_base_dispatch_test(){
    //创建event_base
    struct event_config *config = event_config_new();
    event_config_avoid_method(config, "select");
    event_config_avoid_method(config, "poll");
    event_config_require_features(config, EV_FEATURE_ET);
    base = event_base_new_with_config(config);
    event_config_free(config);
    print_backend_property(base);

    //创建、添加事件
    struct timeval five_seconds = {5, 0};
    event = event_new(base, STDIN_FILENO, EV_TIMEOUT | EV_READ  | EV_PERSIST, cb_func, (void *)"reading event");
    event_add(event, &five_seconds);
    printEventProperty(event);

    //阻塞监听
    event_base_dispatch(base);
}



/**
 * event_base_loop(base, EVLOOP_ONCE);
 *      如果有挂载的event，则等待event被触发，至少执行一次后，如果没有其它的被触发event则退出。
 *      如果没有挂载的event，则直接退出
*/
void event_base_loop_evloop_once_test(){
    struct event_config *config = event_config_new();
    event_config_avoid_method(config, "select");
    event_config_avoid_method(config, "poll");
    event_config_require_features(config, EV_FEATURE_ET);
    base = event_base_new_with_config(config);
    event_config_free(config);
    print_backend_property(base);

    //创建、添加事件
    struct timeval five_seconds = {5, 0};
    event = event_new(base, STDIN_FILENO, EV_TIMEOUT | EV_READ  | EV_PERSIST, cb_func, (void *)"reading event");
    event_add(event, &five_seconds);
    printEventProperty(event);

    //阻塞监听,至少执行一次后，如果没有触发的event则退出
    event_base_loop(base, EVLOOP_ONCE);
}


/**
 * event_base_loop(base, EVLOOP_NONBLOCK);
 *      如果有被触发的event，则执行。如果没有被触发的event，则退出。
*/
void event_base_loop_evloop_nonblock_test(){
    struct event_config *config = event_config_new();
    event_config_avoid_method(config, "select");
    event_config_avoid_method(config, "poll");
    event_config_require_features(config, EV_FEATURE_ET);
    base = event_base_new_with_config(config);
    event_config_free(config);
    print_backend_property(base);

    //创建、添加事件
    struct timeval five_seconds = {5, 0};
    event = event_new(base, STDIN_FILENO, EV_TIMEOUT | EV_READ  | EV_PERSIST, cb_func, (void *)"event happened");
    event_add(event, &five_seconds);
    printEventProperty(event);

    //阻塞监听,至少执行一次后，如果没有触发的event则退出
    int i = 0;
    while(true){
        printf("try count: %d\n", i++);    
        event_base_loop(base, EVLOOP_NONBLOCK);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


/**
 * 不再监听event_base时，释放相关的event和event_base。
*/
void event_base_loopexit_free_test(){
    struct event_config *config = event_config_new();
    event_config_avoid_method(config, "select");
    event_config_require_features(config, EV_FEATURE_ET);
    base = event_base_new_with_config(config);
    event_config_free(config);
    print_backend_property(base);

    struct timeval five_seconds = {5, 0};
    event = event_new(base, STDIN_FILENO, EV_READ | EV_TIMEOUT | EV_PERSIST, cb_func_loopExit, (void*)"event happened");
    event_add(event, &five_seconds);

    event_base_dispatch(base);
    event_free(event);
    event_base_free(base);
    printf("-----------finish-------------\n");
}


#define MAXBYTES 1024

/**
 * 监听标准输入，标准输入有数据后，将数据写入到bufferevent
*/
void cmd_msg_cb(evutil_socket_t stdinfd, short what, void *arg){
    struct bufferevent *bev = reinterpret_cast<struct bufferevent*>(arg);
    puts("get msg from stdin: ");
    char buffer[MAXBYTES];
    ssize_t readNum = read(stdinfd, buffer, sizeof(buffer));
    bufferevent_write(bev, buffer, readNum);
}


/**
 * 当监测到bufferevent从socket读取数据到缓冲区后，会调用读取回调函数。
 * 此函数从缓冲区中读取数据，然后将读取内容输出到标准输出
*/
void read_buf_cb(struct bufferevent *bev, void *cbarg){
    char buf[MAXBYTES];
    ssize_t readNum = bufferevent_read(bev, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, readNum);
}


/**
 * bufferevent监测到某些事件发生后，会调用此回调函数
*/
void event_cb(struct bufferevent *bev, short event, void *cbarg){
    struct event_base *base = reinterpret_cast<struct event_base *>(cbarg);
    if(event & BEV_EVENT_READING)
        puts("BEV_EVENT_READING");

    if(event & BEV_EVENT_WRITING)
        puts("BEV_EVENT_WRITING");

    if(event & BEV_EVENT_ERROR)
        puts("BEV_EVENT_ERROR"); 
    
    if(event & BEV_EVENT_TIMEOUT){
        puts("BEV_EVENT_TIMEOUT");
    }
        
    if(event & BEV_EVENT_CONNECTED)
        puts("BEV_EVENT_CONNECTED");

    if(event & BEV_EVENT_EOF){
        puts("BEV_EVENT_EOF");
        event_base_loopexit(base, nullptr);
    }
}


/**
 * 注册event、阻塞等待event发生。
*/
#define SERVER_IP       "192.168.43.240"
#define SERVER_PORT     60000
void main_loop(){
    struct event_base *base;
    struct bufferevent *bev;    //bufferevent事件
    struct event *ev_stdin;     //普通的event事件

    //创建event_base
    base = event_base_new();

    //创建并使能buffer_event
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    bev = bufferevent_socket_new(base, clientfd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
    //设置回调函数
    bufferevent_setcb(bev, read_buf_cb, nullptr, event_cb, (void *)base);
    /**
     * 设置读写超时
     *      只有在读取或者写入的时候才会计算超时。也就是说，如果bufferevent的读取被禁止，则读取超时被禁止。
     *      类似的如果写入被禁止，或者没有数据写入，则写入超时被禁止。
     *      
     *      读取或者写入超时发生时，相应的读取或者写入操作被禁止，然后超时事件回调被调用。
     *      bufferevent的读取或者写入操作被禁止时
     * 
    */
    // const struct timeval timeout = {5, 0};
    // bufferevent_set_timeouts(bev, &timeout, &timeout);

    //建立连接
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    int connectRet = bufferevent_socket_connect(bev, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(connectRet == 0){
        printf("connect successful ...\n");
    }

    //使能读取
    bufferevent_enable(bev, EV_READ);

    //监测标准输入
    ev_stdin = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, cmd_msg_cb, (void *)bev);
    event_add(ev_stdin, nullptr);

    //阻塞执行
    event_base_dispatch(base);

    //释放资源：释放bufferevent、event、event_base
    bufferevent_free(bev);
    event_free(ev_stdin);
    event_base_free(base);
    puts("exit now ...");
}


int main(int argc, char* argv[]){
   
    main_loop();


    return 0;
}






