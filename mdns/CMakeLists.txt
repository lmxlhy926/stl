cmake_minimum_required(VERSION 3.13)

project(SiteManagerService)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_subdirectory(src/json)
add_subdirectory(src/commlib)
add_subdirectory(src/mdns)
add_subdirectory(src/sitemanager)

add_executable(sitemgs main.cpp)

if(__ANDROID__)
    link_directories(${PROJECT_SOURCE_DIR}/android-libs)
else()
    link_directories(${PROJECT_SOURCE_DIR}/libs)
endif()

target_link_libraries(sitemgs sitemg mdns json commbase)
