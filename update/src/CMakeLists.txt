
set(libPath ${CMAKE_SOURCE_DIR}/deps/linuxLib)
set(includePath ${CMAKE_SOURCE_DIR}/deps/linuxInclude)

add_executable(server server.cpp)
target_link_directories(server PUBLIC ${libPath})
target_include_directories(server PUBLIC ${includePath})
target_link_libraries(server PRIVATE httplib)
target_link_libraries(server PRIVATE pthread)


add_executable(test test.cpp httpud.cpp)
target_link_directories(test PUBLIC ${libPath})
target_include_directories(test PUBLIC ${includePath})
target_link_libraries(test httplib)
target_link_libraries(test pthread)
target_link_libraries(test unzip)
target_link_libraries(test z)
