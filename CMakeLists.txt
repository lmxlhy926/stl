cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(element stl_string fundamental callableObject fileaccess)

include_directories(stl_string)
include_directories(fundamental)
include_directories(callableObject)
include_directories(fileaccess)

link_directories(stl_string)
link_directories(fundamental)
link_directories(callableObject)
link_directories(fileaccess)


add_subdirectory(stl_string)
add_subdirectory(fundamental)
add_subdirectory(callableObject)
add_subdirectory(fileaccess)


add_executable(main main.cpp)
target_link_libraries(main ${element})