cmake_minimum_required(VERSION 3.13)

project(stl)

set(CMAKE_CXX_STANDARD 14)

set(srcDirectory unix)

foreach(elem ${srcDirectory})
    add_subdirectory(${elem})
endforeach()

message("-------here---------")

#INSTALL
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out" CACHE STRING "the path to use for make install" FORCE)
install(TARGETS fileProperty    DESTINATION bin)
#install(TARGETS client      DESTINATION bin)
#install(TARGETS server      DESTINATION bin)


