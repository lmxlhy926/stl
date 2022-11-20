
add_executable(select_server select_server.c)
add_library(collect STATIC
            select_server.c
            server_epoll.c
            socket_pool.c
            udp-server.c)