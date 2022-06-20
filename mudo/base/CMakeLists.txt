FILE(GLOB src "*.cpp")
add_library(muduo_base STATIC ${src})
target_include_directories(muduo_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectory(test)