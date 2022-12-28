cmake_minimum_required(VERSION 3.15)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(srcDirectory unix stl)

foreach(elem ${srcDirectory})
    add_subdirectory(${elem})
endforeach()

#INSTALL
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out" CACHE STRING "the path to use for make install" FORCE)
install(TARGETS unixfile DESTINATION bin)
