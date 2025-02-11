add_executable(test main.cpp httplib.cc)
target_link_libraries(test PRIVATE pthread)
