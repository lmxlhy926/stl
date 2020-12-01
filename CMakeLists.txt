cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(lib stlString basic_class callableObject)

include_directories(stl_string)
include_directories(fundamental)
include_directories(callableObject)

link_directories(stl_string)
link_directories(fundamental)
link_directories(callableObject)


add_subdirectory(stl_string)
add_subdirectory(fundamental)
add_subdirectory(callableObject)

add_executable(main main.cpp)
target_link_libraries(main ${lib})