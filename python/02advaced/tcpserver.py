#! /usr/bin/python3

import socket
import sys

# 创建端口号
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 获取主机名
host = socket.gethostname()

# 端口号
port = 9999

# 绑定地址
serversocket.bind((host, port))

# 监听连接请求
serversocket.listen(5)


while True:
    # 阻塞接受连接请求
    clientsocket, addr = serversocket.accept()

    print(f"连接地址： {addr}")
    msg = "欢迎访问菜鸟教程！ " + "\r\n"

    # 向链接端点发送数据
    clientsocket.send(msg.encode("utf-8"))

    # 关闭和客户端连接的端点
    clientsocket.close()















