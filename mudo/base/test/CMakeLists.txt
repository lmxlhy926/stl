
add_executable(thread_test thread_test.cpp)
target_link_libraries(thread_test PRIVATE muduo_base)

add_executable(base_test base_test.cpp)
target_link_libraries(base_test PRIVATE muduo_base)


