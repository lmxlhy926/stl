

add_executable(MakeTable multi.cpp) #生成可执行程序

#生成的文件, 以及生成文件所依赖的方法
add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/multi.h
        COMMAND MakeTable ${PROJECT_BINARY_DIR}/multi.h
        DEPENDS MakeTable
)

include_directories(${PROJECT_BINARY_DIR})  #添加头文件搜索目录

add_library(add SHARED add.cpp ${PROJECT_BINARY_DIR}/multi.h)   #生成库

#安装文件
install(FILES "${PROJECT_BINARY_DIR}/multi.h" DESTINATION include)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)