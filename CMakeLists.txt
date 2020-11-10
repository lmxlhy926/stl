cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(lib stlString)

include_directories(stl_string)

link_directories(stl_string)

add_subdirectory(stl_string)

add_executable(main main.cpp)
target_link_libraries(main ${lib})