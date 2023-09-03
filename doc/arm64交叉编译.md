```bash
# 查看可以安装的版本
apt-cache search aarch64

# 选择可以安装的版本进行安装
# 此处选择gcc-5-aarch64-linux-gnu以及g++-5-aarch64-linux-gnu进行安装
sudo apt-get install gcc-5-aarch64-linux-gnu　
sudo apt-get install gcc++-5-aarch64-linux-gnu

# 安装一个没有版本号的gcc和g++
sudo apt-get install gcc-aarch64-linux-gnu
sudo apt-get install gcc-aarch64-linux-gnu

# 查看版本
aarch64-linux-gnu-g++ -v
aarch64-linux-gnu-gcc -v
```

