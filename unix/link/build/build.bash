#! /usr/bin/bash

# 链接时一般需要在命令行中指明要额外查找的库目录路径
# 注意链接时查找的库路径和运行时查找的库路径的区别

gcc -c ../staticSource.cpp
ar rcs libstatic.a staticSource.o

gcc --shared -fpic -o libsharedcall.so ../sharedSourceCall.cpp
gcc --shared -fpic -o libshared.so ../sharedSource.cpp -L . -lsharedcall


if [ $1 = "static" ]
then
	gcc -o staticLoad ../execStaticLoad.cpp -L . -lstatic -lshared
fi

if [ $1 = "shared" ]
then
	gcc -rdynamic -o dynamicLoad ../execDynamicLoad.cpp -ldl
fi