add_executable(tcpmain main.cpp)
add_executable(tcp_client_libevent tcp_client_libevent.cpp)
add_executable(tcp_server_libeent  tcp_server_libevent.cpp)


target_link_libraries(tcpmain PRIVATE event)
target_link_libraries(tcp_client_libevent PRIVATE event)
target_link_libraries(tcp_server_libeent PRIVATE event)

