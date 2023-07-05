add_executable(eventest1 test1.cpp)
target_link_libraries(eventest1 PRIVATE event)

add_executable(eventTcpServer tcp_server.cpp)
target_link_libraries(eventTcpServer PRIVATE event)

add_executable(eventTcpClient tcp_client.cpp)
target_link_libraries(eventTcpClient PRIVATE event)


