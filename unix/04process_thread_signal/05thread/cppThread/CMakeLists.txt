add_executable(async            01_async.cpp)
target_link_libraries(async PRIVATE pthread)

add_executable(cppThread        02_thread.cpp)
target_link_libraries(cppThread PRIVATE pthread)

add_executable(cppMutex         03_cppMutex.cpp)
target_link_libraries(cppMutex PRIVATE pthread)

add_executable(cppCondition     04_condition.cpp)
target_link_libraries(cppCondition PRIVATE pthread)