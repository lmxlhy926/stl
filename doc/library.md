### 编译器工作机制

​	编译过程涉及到多个阶段和多个工具，阶段主要包括`预处理`，`编译` ，`汇编`，`链接`，工具包括`预处理器`，`编译器`，`汇编器`, `链接器`。编译过程中涉及到的完整的工具的集合被称为`工具链`。

* preprocessing（展开宏定义和头文件）
* compilation（经过预处理的高级语言源代码转换为特定处理器的machine instructions）
* assembly（将处理器的machine instructions转换为machine code）
* linking（连接object files产生executable）

如果在汇编的过程中，汇编源码中包含对`external function`的调用，assembler会使`external function`的地址为未定义，后面由linker来实际填充该地址。

```shell
#preprocesser	展开宏定义、头文件
	cpp hello.c > hello.i
#compiler		生成处理器的汇编指令
	gcc -Wall -S hello.i
#assembler		生成机器码
	as hello.s -o hello.o
#linker			链接目标文件，生成可执行文件
	gcc hello.o
```



### 单文件编译为可执行程序

​	`gcc -Wall -o hello hello.c`

### 多文件编译为可执行程序

​	`gcc -Wall -o a.out a.c b.c`

### 源文件编译为目标文件

​	`gcc -Wall -c a.c b.c` 

### 链接目标文件为可执行程序

​	`gcc -o a.out a.o b.o`

### 头文件/库文件搜索路径

* 命令行选项 `-I`  `-L` ，从左到右 
* 环境变量： `C_INCLUDE_PATH`  `CPLUS_INCLUDE_PATH`  `LIBRARY_PATH`
* 系统默认目录



#### 静态库和动态库（库本身的依赖）

​	静态库和共享库的一个主要不同在于它们如何处理自己的依赖。链接共享库时不需要指定共享库自身的依赖，例如链接共享库`xyz`只需在链接命令行中加上`-lxyz`即可。链接静态库时需要指定静态库自身依赖的库，例如`xyz`是静态库，则在链接命令行中除了添加`-lxyz`还要添加`xyz`依赖的库例如`-labc -ldef`.



### 静态库

**库**：源代码只编译不链接生成的.o文件(二进制格式代码)的集合的包。

**库的使用**：头文件，库

**命令规则**：libtest.a libxxx.a ：lib为前缀xxx为库名.a为后缀

**库的制作与使用**：

1. 将.c文件生成.o文件: `gcc -c a.c b.c`

2. 将.o文件打包: `ar rcs libtest.a a.o b.o`

3. 查看库包: `nm libtest.a`

4. 链接静态库： `gcc test.c -satic -I ./include -L ./lib -ltest -o app`

​		`-I`：指定头文件路径

​		`-L`：指定库包路径

​		`-l`：指定库包名称



### 动态库

**命名规则**：`libxxx.so`

**制作步骤**：

1.源文件生成.o文件：`gcc a.c b.c -c -fpic`    	 -fpic：生成位置无关代码

2.打包生成的.o文件：`gcc -shared a.o b.o -o libxxx.so`

**库的使用**：1.头文件；2.动态库libxxx.so; 

**链接动态库**：`gcc main.c -shared -I ./include -L ./lib -lxxx` 

#### 加载链接：

**`ld.so`**：动态链接器

动态链接器可以通过运行动态链接程序而被间接调用也可以直接调用。动态链接器找到和加载程序所需的共享库，准备程序的运行然后运行程序。**主要是间接调用**

动态库加载顺序：
	`LD_LIBRARY_PATH`:	`export LD_LIBRARY_PATH=/usr/local/lib`
	`/etc/ld.so.cache`
	`/lib`,  `/usr/lib`

**`ldconfig`**：配置动态链接器的运行时绑定

默认搜寻`/lib`，`/usr/lib`以及配置文件`/etc/ld.so.conf`内所列的目录。从这些目录中搜索格式为`lib***.so.**`的库文件，从而创建出链接器所需的连接和缓存文件。并会将创建的缓存存储在`/etc/ld.so.cache`中，该文件保存已经排好序的动态链接库的名字列表。安装一个新的动态库后需要手动运行ldconfig命令，从而将新加入的库的信息存入缓存文件中。

**添加动态链接库的方法**：

  * 在`/lib`和`/usr/lib`中添加动态链接库时，只需要添加后执行`ldconfig`命令即可。
  * 在除上述以外的目录添加库时，需要将库文件路径追加到`/etc/ld.so.conf`中。一个好的方法是在`/etc/ld.so.conf.d`目录下创建一个`.conf`文件来将用户自己添加的动态库路径都写入到这个文件中。因为`/etc/ld.so.conf`文件中只有一句内容`include /etc/ld.so.conf.d/*.conf`即将`/etc/ld.so.conf.d`目录下的全部`.conf`结尾的文件都包含进去，然后展开它们的内容。最后执行`ldconfig`命令

`ldconfig -v`：显示扫描的目录以及在目录下搜索到的动态链接库的名字。
`ldd`：查询可执行程序的动态依赖库

`/etc/ld.so.conf`：**一个文件**，指示要扫寻的目录。
`/etc/ld.so.conf.d`：**一个文件夹**，里面包含*.conf结尾的文件，每个文件里包含着要搜索的目录。
`/etc/ld.so.cache`：**动态库缓存**





























