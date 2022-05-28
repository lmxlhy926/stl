FILE(GLOB src "*.cpp")
add_library(cppBase STATIC ${src})
target_include_directories(cppBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)