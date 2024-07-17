#! /usr/bin/bash

# shell既是一个功能强大的命令行接口，也是一个脚本语言解释器
#       . shell脚本就是普通的文本文件
#       . 设置脚本的权限
#       . 把脚本放置到shell能够找到的地方
#       . 为了能够执行脚本，脚本必须是可读的
#       . 通过使用行继续符（反斜杠-回车符序列）和缩进


# echo输出提示信息：
#       一个带引号的字符串可能包含换行符，因此可以包含多个文本行。shell会持续读取文本直到它遇到右引号。
function echoTest(){
echo "hell
this
    is 
a messsage from echo"
}


# 当shell碰到一个变量的时候，它会自动创建它。
# shell变量命名规则：
#       1. 变量名可由字母、数字字符、下划线字符组成
#       2. 变量名的第一个字符必须是一个字母或一个下划线
#       3. 变量名中不允许出现空格和标点符号
function variable_test(){
    a=z
    b="a string"
    c="a string and $b"
    d=$(ls -l)      #命令结果
    e=$((5 * 7))    #算数运算
    f="\t\ta string\n"  #包含转义字符
    echo $a
    echo $b
    echo $c
    echo $d
    echo $e
    echo -e $f
}

#有用的变量
function usefulVariable(){
    echo $HOSTNAME  #主机名
    echo $(date +"%X %r %Z")    #日期
    echo $USER  #用户名

}


# 一个here document是另外一种I/O重定向形式，我们在脚本文件中嵌入正文文件，然后把它发送给一个命令的标准输入。
#
#command << token
#text
#token
#
# 这里command是一个可以接受标准输入的命令名，token是一个用来指示嵌入文本结束的字符串。
# here document的优点：默认情况下，here documents中的单引号和双引号都会失去它们在shell中的特殊含义。
# 如果我们把重定向操作符从"<<"改为"<<-", shell会忽略在此here document中开头的tab字符。
function hereDocument(){
foo="some text"
cat << _EOF_
$foo
"$foo"
'$foo'
\$foo
_EOF_
}


# 为了使函数调用被识别出是shell函数，而不是被解释为外部程序的名字，在脚本中shell函数定义必须出现在函数调用之前。
# 局部变量：
#       在函数中，通过在变量名之前加上单词local，来定义局部变量。
#       这样就只创建了一个只对其所在shell函数起作用的变量。在这个shell函数之外，这个变量不再存在。
#
# 你的.bashrc文件中的shell函数。shell函数完美的替代了别名，并且实际上是创建个人所用的小命令的首选方法。
# 使用source example.sh加载shell文件到上下文环境中
foo=0
function localVariable_1(){
    local foo   # 声明为局部变量，只在这个shell函数中起作用
    foo=1
    echo "localVariable_1: $foo"
}
function localVariable_2(){
    local foo
    foo=2
    echo "localVariable_2: $foo"
}
function localVariable_test(){
    echo "foo: ${foo}"
    localVariable_1
    echo "foo: ${foo}"
    localVariable_2
    echo "foo: ${foo}"
}



# 测试文件状态
#   文件存在且属于某种类型
#       -e file     （file存在）
#       -b file      (file存在且是一个块设备文件)
#       -c file      (file存在且是一个字符设备文件)
#       -d file      (file存在且是一个目录)
#       -f file      (file存在且是一个普通文件)
#       -L file      (file存在且是一个符号链接)
#       -p file      (file存在且是一个命名管道)
#       -S file      (file存在且是一个网络socket)
#   文件执行权限
#       -r file (有效用户)
#       -w file (有效用户)
#       -x file (有效用户)
#   文件时间比较
#       file1 -ef file2     (file1和file2拥有相同的索引号)
#       file1 -nt file2    （file1新于file2）
#       file1 -ot file2     (file1早于file2)
#   文件其它标志
#       -g file     (file存在并且设置了组ID)
#       -G file     (file存在并且由有效组ID拥有)
#       -k file     (file存在并且设置了它的"sticky bit")
#       -O file     (file存在并且由有效用户ID拥有)
#       -u file     (file存在并且设置了setuid位)
#       -s file     (file存在并且长度大于0)
#       -t fd       (fd是一个定向到终端/从终端定向的文件描述符)

if [ -g a.txt ]
then
    echo "----"
fi
