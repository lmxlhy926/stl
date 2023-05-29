add_executable(async async.cpp)
target_link_libraries(async PUBLIC pthread)

add_executable(thread thread.cpp)
target_link_libraries(thread PUBLIC pthread)