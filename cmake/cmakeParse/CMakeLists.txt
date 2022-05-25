
cmake_minimum_required(VERSION 3.10.2)  #限定cmake的最小版本号
project(Tutorial)   #指定工程名称


macro(myinstall hello)
    message("argv: ${ARGV}")    #全部参数
    message("argn: ${ARGN}")    #指定形参以外的参数

    set(option OPTIONAL)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs TARGETS)
    cmake_parse_arguments(prefix "${option}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
endmacro()

myinstall(hello OPTIONAL DESTINATION ../.. TARGETS t1 t2)

message("prefix_OPTIONAL; ${prefix_OPTIONAL}")
message("prefix_DESTINATION: ${prefix_DESTINATION}")
message("prefix_TARGETS: ${prefix_TARGETS}")

#set(testfilecontent)
file(READ "testfile" testfilecontent)
message("testfilecontent: ${testfilecontent}")

file(GLOB files RELATIVE ../.. "${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt")
message("files: ${files}")


















