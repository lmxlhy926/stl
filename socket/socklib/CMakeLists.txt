add_library(sock socket.cpp rio.cpp)
target_include_directories(sock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})