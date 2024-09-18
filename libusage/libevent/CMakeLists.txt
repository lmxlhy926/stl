add_executable(libevent_test libevent_test.cpp)
target_link_libraries(libevent_test PRIVATE event)

# add_executable(tcpServerLibevent tcp_server_libevent.cpp)
# target_link_libraries(tcpServerLibevent PRIVATE event)

# add_executable(tcpClientLibevent tcp_client_libevent.cpp)
# target_link_libraries(tcpClientLibevent PRIVATE event)

