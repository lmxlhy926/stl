通过ioctl()函数调用

下表列出了网络相关ioctl请求的request参数以及arg地址必须指向的数据类型

| 类别     | request                                                      | 说明                                                         | 数据类型                                                     |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ | :----------------------------------------------------------- |
| 套接口   | SIOCATMARK <br/>SIOCSPGRP <br/>SIOCGPGRP                     | 是否位于带外标记 <br/>设置套接口的进程ID 或进程组ID <br/>获取套接口的进程ID 或进程组ID | int <br/>int <br/>int                                        |
| 文件     | FIONBIN <br/>FIOASYNC <br/>FIONREAD <br/>FIOSETOWN <br/>FIOGETOWN | 设置/ 清除非阻塞I/O 标志 <br/>设置/ 清除信号驱动异步I/O 标志 <br/>获取接收缓存区中的字节数 <br/>设置文件的进程ID 或进程组ID <br/>获取文件的进程ID 或进程组ID | int <br/>int <br/>int <br/>int <br/>int                      |
| 网络接口 | SIOCGIFCONF <br/>SIOCSIFADDR <br/>SIOCGIFADDR <br/>SIOCSIFFLAGS <br/>SIOCGIFFLAGS <br/>SIOCSIFDSTADDR <br/>SIOCGIFDSTADDR <br/>SIOCGIFBRDADDR <br/>SIOCSIFBRDADDR <br/>SIOCGIFNETMASK <br/>SIOCSIFNETMASK <br/>SIOCGIFMETRIC <br/>SIOCSIFMETRIC <br/>SIOCGIFMTU | 获取所有接口的清单 <br/>设置接口地址 <br/>获取接口地址 <br/>设置接口标志 <br/>获取接口标志 <br/>设置点到点地址 <br/>获取点到点地址 <br/>获取广播地址 <br/>设置广播地址 <br/>获取子网掩码 <br/>设置子网掩码 <br/>获取接口的测度 <br/>设置接口的测度 <br/>获取接口MTU | struct ifconf <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq <br/>struct ifreq |
| ARP      | SIOCSARP <br/>SIOCGARP <br/>SIOCDARP                         | 创建/ 修改ARP 表项 <br/>获取ARP 表项 <br/>删除ARP 表项       | struct arpreq <br/>struct arpreq <br/>struct arpreq          |
| 路由     | SIOCADDRT <br/>SIOCDELRT                                     | 增加路径 <br/>删除路径                                       | struct rtentry <br/>struct rtentry                           |

