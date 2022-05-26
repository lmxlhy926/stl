FILE(GLOB src "*.cpp")
add_library(file STATIC ${src})
target_include_directories(file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)