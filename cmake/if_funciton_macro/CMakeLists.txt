
cmake_minimum_required(VERSION 3.10.2)  #限定cmake的最小版本号
project(Tutorial)   #指定工程名称

#set-------------------------------------------------------------------
message("----------------set----------------------")

#cmake支持简单变量，变量的值为字符串/字符串队列
#变量的解引用格式：${VAR}
#使用set可以将多个参数放到一个list里，所有其它的命令都是展开这个列表，就好像它们以空格分隔的方式传递到命令里

#设置变量, 只能通过set来定义变量
set(hello true)     #变量值是单个字符串
set(txtfilelist 1.txt 2.txt 3.txt)     #变量值是个list
set(jsonfilelist 1.json;2.json;3.json)  #变量值是个list

message("txtfilelist: ${txtfilelist}")
message("jsonfilelist: ${jsonfilelist}")
message("filelist: ${txtfilelist} ${jsonfilelist}")

foreach(elem ${txtfilelist} ${jsonfilelist})    #变量的展开,等同于如下方式传递参数
    message("elem: ${elem}")
endforeach()

foreach(0.txt 1.txt;2.txt;3.txt 1.json;2.json;3.json)    #第一个参数为变量，其它的为参数列表
    message("expand: ${0.txt}")
endforeach()

message("----------------set----------------------\n\n")
#set-------------------------------------------------------------------


#if-------------------------------------------------------------------
message("----------------if----------------------")

message("===>variable: ${variable}")
if(variable)    #变量未定义，值为空
    message("---if(variable)---")
endif()

if(COMMAND if)
    message("===>command if exists")
endif()

set(variable "")
if(DEFINED variable)    #定义的概念是使用set命令为变量赋值
    message("===>variable is defined..")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/main.cpp")   #指定文件的全路径
    message("===>the file ${CMAKE_SOURCE_DIR}/main.cpp exists....")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/include")
    message("===>the directory ${CMAKE_SOURCE_DIR}/include exists...")
endif()

if(1 LESS 2)
    message("1 less 2...")
endif()

if("abc" STRLESS "ac")  #按字母顺序比较
    message("==>abc STRLESS ac")
endif()

message("----------------if----------------------\n\n")
#if-------------------------------------------------------------------


#while-------------------------------------------------------------------
message("----------------while----------------------")
set(Index 1)
while(${Index} LESS 5)
    message("index==${Index}")
    set(Index 5)
endwhile()


message("----------------while----------------------\n\n")
#while-------------------------------------------------------------------


#function-------------------------------------------------------------------
message("----------------function----------------------")

function(DetermineTime _time)   #_time是变量，容纳传递的实参
    if(DEFINED ${_time})
        message("${_time} is defiend..")
    else()
        message("${_time} is not defiend..")
    endif()
    message("in function time: ${_time}")
    set(${_time} "1234" PARENT_SCOPE)
endfunction()

set(current_time 4567)
DetermineTime(current_time)     #传递的实参, 即字符串

if(DEFINED current_time)
    message(STATUS "current time is defined and the value is : ${current_time}")
endif()

function(func var1 var2 var3)
    message("argv0 arg: ${ARGV0}")  #第一个参数
    message("argv1 arg: ${ARGV1}")  #第二个参数
    message("argv2 arg: ${ARGV2}")  #第三个参数
    message("argc: ${ARGC}")    #参数的总数量
    message("argv: ${ARGV}")    #参数列表
    foreach(elem ${ARGV})
        message("elem: ${elem}")
    endforeach()
endfunction()

func(1 2 3)

message("----------------function----------------------\n\n")
#function-------------------------------------------------------------------


#macro-------------------------------------------------------------------
message("----------------macro----------------------")

macro(assert TEST COMMENT)  #TEST COMMENT是变量, 容纳传入的值
    message("TEST: ${TEST}")

    if(DEFINED ${TEST})
        message("${TEST} is defined..")
    else()
        message("${TEST} is not defined...")
    endif()

    if(NOT ${TEST})
        message("assertion failed: ${COMMENT}")
    endif()

    if(${ARGC} MATCHES 3)
        set(${ARGV2} "argv2")   #宏定义不单独加载栈空间，共用一个栈空间
    endif()

    message("ARGN: ${ARGN}")    # a list of all the arguments after the formal arguments
    message("ARGV: ${ARGV}")    # a list of all the arguments to the macro


endmacro()

set(test true)
assert(test "hello" newaddvariable)

if(DEFINED newaddvariable)
    message("newaddvaribale is defined....${newaddvariable}")
endif()

message("----------------macro----------------------\n\n")
#macro-------------------------------------------------------------------




