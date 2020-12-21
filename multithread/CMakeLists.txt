
find_package(Threads)
add_library(mthread mthread.cpp)
target_link_libraries(mthread ${CMAKE_THREAD_LIBS_INIT})
