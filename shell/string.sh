#! /usr/bin/bash

# 创建字符串
str="hello world"

# 打印字符串
echo $str

# 子字符串
echo ${str:6}
echo ${str:0:5}

# 字符串拼接
str="hello"
str1="${str} world xxxx"
echo $str1

# 字符串遍历
for c in this is a great idea; do
    echo $c
done

# 模式匹配
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



# 基于通配符的匹配

# 删除
str="hello123world"
echo ${str#hello*}              #要求匹配项出现在开头
echo ${str##hello*}             #要求匹配项出现在开头，最长匹配
echo ${str%[[:digit:]]world}    #要求匹配项出现在结尾
echo ${str%%*world}             #要求匹配项出现在结尾，最长匹配

# 替换
str="hello1 hello2 hello3 "
echo ${str/hello/}  #替换第一个匹配项
echo ${str//hello/} #替换所有匹配项
echo ${str/#hello/} #要求匹配项出现在字符串的开头
echo ${str/%hello/} #要求匹配项出现在字符串的结尾

# 分割
str="hello;xxxxx;aa88;hello"
IFS=';' read -a array <<< $str
for elem in ${array[@]};do
    echo $elem
done


# 拼接：变量展开


# 大小写转换
str="hEllo hEllo"
echo ${str^}        #第一个字符大写
echo ${str^^}       #所有字符大写
echo ${str,}        #第一个字符小写
echo ${str,,}       #所有字符小写


