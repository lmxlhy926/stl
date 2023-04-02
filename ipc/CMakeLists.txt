add_subdirectory(udp)
add_subdirectory(httplib)
add_executable(client client.cpp)
add_executable(server server.cpp sockUtil.cpp)

add_executable(httpclient httpclient.cpp)
target_link_libraries(httpclient PRIVATE http)

add_executable(httpserver httpServer.cpp)
target_link_libraries(httpserver PRIVATE http)