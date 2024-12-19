#! /usr/bin/python3

import socket
import sys


# 创建socket端点
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 返回当前主机名
host = socket.gethostname()

# 主机主机端口号
port = 9999

# 连接服务器
s.connect((host, port))

# 从服务器读取数据
msg = s.recv(1024)

# 关闭端口
s.close()

print(msg.decode("utf-8"))


