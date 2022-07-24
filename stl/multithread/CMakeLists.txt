
set(SOURCE
        mthread.cpp
        mmutex.cpp
        condition.cpp
        matomic.cpp)

add_library(multiThread STATIC ${SOURCE})
target_include_directories(multiThread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)