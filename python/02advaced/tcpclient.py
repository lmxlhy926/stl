#! /usr/bin/python3

import socket
import sys

"""
创建socket server
"""
# 创建socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 设置属性
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# 主机、端口号
host = "0.0.0.0"
port = 9999
# 绑定地址
serversocket.bind((host, port))
# 开始监听
serversocket.listen(5)

while True :
    # 接收客户端连接，返回客户端地址以及通信端口
    clientsocket, addr = serversocket.accept()
    print("连接地址： {}".format(str(addr)))
    msg = "welcome to runoob " + "\r\n"
    # 发送数据给客户端
    clientsocket.send(msg.encode("utf-8"))
    # 和客户端断开连接
    clientsocket.close()




