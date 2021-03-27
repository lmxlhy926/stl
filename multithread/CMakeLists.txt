
find_package(Threads)
add_library(mthread mthread.cpp mmutex.cpp condition.cpp matomic.cpp threadPoll.cpp)
target_link_libraries(mthread ${CMAKE_THREAD_LIBS_INIT})
