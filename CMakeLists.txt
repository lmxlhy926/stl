cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(srcDirectory container exception fundamental multithread operatoroverload stl_string stream httlib functionObject)
foreach(elem ${srcDirectory})
    add_subdirectory(${elem})
endforeach()