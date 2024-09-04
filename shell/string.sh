#! /usr/bin/bash

# 创建字符串
str="hello world"


# 打印字符串
echo $str


# 子字符串
echo ${str:6}
echo ${str:0:5}


# 字符串拼接
str+="xxxxx"
echo $str

# 字符串遍历
for c in this is a great idea; do
    echo $c
done


# 成员运算符
if [[ "hello" == *ll* ]]; then
    echo "match..."
fi



# 模式判断
if [[ "hello" =~ ^[[:alpha:]]*$ ]]; then
    echo "alpah ..."
fi

if [[ "1234" =~ ^[[:digit:]]*$ ]]; then
    echo "digit match ..."
fi
echo "----------------------------------------------"



# 查找、替换、分割、拼接
# 通配符 ？ * []
str="hello xxxxx aa88 hello"
echo ${str##*[ ]}
echo ${str%%[ ]*}
echo ${str//[[:digit:]][[:digit:]]/}

str="hello;xxxxx;aa88;hello"
IFS=';' read -a array <<< $str
for elem in ${array[@]};do
    echo $elem
done


