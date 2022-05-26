FILE(GLOB src "*.cpp")
add_library(cppBase STATIC ${src})

add_subdirectory(test)