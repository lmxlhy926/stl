add_executable(client client.cpp)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/lib/httplib)
target_link_libraries(client PUBLIC http)

add_executable(server server.cpp)
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/lib/httplib)
target_link_libraries(server PUBLIC http)

add_executable(copytest test.cpp)
