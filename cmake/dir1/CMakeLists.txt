#in-source build:
#   ccmake .
#   make
#   make install

#out-of-source build
#   ccmake ..
#   make
#   make install

cmake_minimum_required(VERSION 3.10.2)  #限定cmake的最小版本号
project(Tutorial)   #指定工程名称
set(CMAKE_CXX_STANDARD 14)  #指定c++标准

set(MAX_X 100)  #给变量赋值
set(MAX_Y 200)
set(CMAKE_INSTALL_PREFIX ../..) #指定安装目录前缀

option(MATHLIB "use add function" ON)   #GUI交互选项


function(addmathdir)    #函数
    if(MATHLIB)
        add_subdirectory("../mathdir")         #添加子目录
        include_directories("../mathdir")      #添加库头文件所在目录
        set(EXTRA_LIBS ${EXTRA_LIBS} add PARENT_SCOPE)   #添加需要链接的库  #函数作用域
    endif(MATHLIB)
endfunction()


#配置头文件, 用头文件将cmake的配置参数传递到源代码中
#config.h.in中的参数经过cmake中的参数替换后,生成config.h头文件, 然后在源码中包含该头文件
configure_file(
        "../config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)


include_directories("${PROJECT_BINARY_DIR}")    #添加头文件搜索目录
addmathdir()    #函数调用

add_executable(tutorial ../main.cpp)   #生成可执行文件
target_link_libraries(tutorial ${EXTRA_LIBS})   #指定可执行文件所依赖的库











