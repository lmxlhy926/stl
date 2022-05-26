add_executable(cfile_test cfileTest.cpp)
target_link_libraries(cfile_test PRIVATE file)

add_executable(cplusFile_test cplusFileTest.cpp)
target_link_libraries(cfile_test PRIVATE file)

