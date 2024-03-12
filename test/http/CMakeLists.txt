add_executable(httpTest main.cpp)
target_link_libraries(httpTest PUBLIC pthread)

add_executable(regex regex.c)