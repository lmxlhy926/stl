
add_executable(exception main.cpp mexception.cpp)
target_link_libraries(exception PRIVATE pthread)