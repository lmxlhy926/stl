
#include <event2/event.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    struct event_config *cfg;
    struct event_base *base;

    cfg = event_config_new();
    event_config_avoid_method(cfg, "select");
    event_config_require_features(cfg, EV_FEATURE_ET);
    event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    base = event_base_new_with_config(cfg);
    event_config_free(cfg);

    int features = event_base_get_features(base);
    printf("using libevent with backend method %s.\n", event_base_get_method(base));
    if(features & EV_FEATURE_ET){
        printf("ev_feature_et\n");

    }else if(features & EV_FEATURE_O1){
        printf("ev_feature_o1\n");

    }else if(features & EV_FEATURE_FDS){
        printf("ev_feature_fds\n");
    }

    event_base_priority_init(base, 3);
    event_base_free(base);

    return 0;
}



