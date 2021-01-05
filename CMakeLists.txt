cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(element stl_string fundamental callableObject lstream operatoroverload mthread)

include_directories(stl_string)
include_directories(fundamental)
include_directories(callableObject)
include_directories(stream)
include_directories(operatoroverload)
include_directories(multithread)


#link_directories(stl_string)
#link_directories(fundamental)
#link_directories(callableObject)
#link_directories(fileaccess)
#link_directories(operatoroverload)
#link_directories(multithread)

add_subdirectory(stl_string)
add_subdirectory(fundamental)
add_subdirectory(callableObject)
add_subdirectory(stream)
add_subdirectory(operatoroverload)
add_subdirectory(multithread)

add_executable(main main.cpp)
target_link_libraries(main ${element})