add_executable(async 01_async.cpp)
target_link_libraries(async PUBLIC pthread)

add_executable(thread 02_thread.cpp)
target_link_libraries(thread PUBLIC pthread)

add_executable(cppMutex 03_cppMutex.cpp)
target_link_libraries(cppMutex PUBLIC pthread)