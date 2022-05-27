
add_executable(updateServer server.cpp httpud.cpp)
target_include_directories(updateServer PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(updateServer PRIVATE http)
