add_executable(tcp main.cpp)

add_executable(tcp_event main_libevent.cpp)
target_link_libraries(tcp_event PRIVATE event)