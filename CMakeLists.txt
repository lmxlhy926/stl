cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(srcDirectory ipc2 ipc)

foreach(elem ${srcDirectory})
    add_subdirectory(${elem})
endforeach()