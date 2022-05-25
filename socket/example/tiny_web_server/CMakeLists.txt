

include_directories(./../../lib/socklib/)
add_executable(tiny_web_server main.cpp tiny_web_func.cpp)
target_link_libraries(tiny_web_server sock)
