
add_executable(thread_test thread_test.cpp)
target_link_libraries(thread_test PRIVATE thread)

add_executable(threadPool_test threadPoolTest.cpp)
target_link_libraries(threadPool_test PRIVATE thread)

add_executable(log_test log_test.cpp)
target_link_libraries(log_test PRIVATE log)
target_link_libraries(log_test PRIVATE thread)

add_executable(logFile_test logFileTest.cpp)
target_link_libraries(logFile_test PRIVATE log)
target_link_libraries(logFile_test PRIVATE thread)

