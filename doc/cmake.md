### cmake

---

**Make工具**：unix下的`GNU Make`，QT的`qmake`，微软的`MS nmake`...，这些Make工具遵循不同的规范和标准，所执行的Makefile格式也千差万别。如果软件想跨平台，必须要保证能够在不同平台编译。而如果使用上面的Make工具，就得为每一种标准写一次Makefile。

**CMake**：允许开发者编写一种与平台无关的CMakeList.txt文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化Makefile或工程文件，如Unix的Makefile或Windows的Visual Studio工程。CMake下的几个程序：`cmake`， `ccmake`  ：构建/安装工具；`ctest` ：测试工具；`cpack`：打包工具

**linux下使用CMake生成Makefile并编译的流程**

* 编写CMake配置文件`CMakeLists.txt`
* 执行命令`cmake PATH`或者`ccmake PATH`生成`Makefile`。`ccmake`提供了一个交互式页面，`PATH`是`CMakeLists.txt`所在目录。
* 使用`make`命令进行编译



### CMake BuildSystems

`buildsystem ` 描述了如何使用 `build tool` 自动的从 `source code` 构建 `library ` `executable`. 常见的构建系统：Makefile + make	project file + IDE；使用`CMake language`写的`CMakeLists.txt`来抽象的指定`build system`，CMake通过`generator`产生相应的本地构建系统（即产生相应的构建文件）.

**source tree**：存储源码的最上层目录
**build tree**：包含构建系统文件和构建过程输出的最上层目录
**generator**：选择产生何种构建工具文件



### Generate a Project Buildsystem

每个机器产生特定的`buildsystem`，产生的构建文件在不同机器之间不具有可移植性。每个使用源码软件包的消费者都应该使用CMake产生自己系统的特定的构建文件。一般来说产生的构建文件应该被当做只读文件来对待，应该使用CMake files来完全唯一的指定构建文件。CMake必须能够找到合适的`buildtool`、`compiler`、`linker`和其它需要的工具才能正常运行。

```shell
#cmake [<options>] <path-to-source>
mkdir build;
cd build;
cmake ../SRC	#使用当前目录作为build tree,使用<path-to-source>作为source tree.


#cmake [<options>] <path-to-existing-build>
#使用<path-to-existing-build>作为build tree,从build tree的CMakeCache.txt 			   #中加载source tree.
cd build;
cmake .		


#cmake [<options>] -S <path-to-source> -B <path-to-build>
cmake -S source -B build	#同时制定source tree、build tree


#指定generator, target architecture
#使用cmake --help来查看本机支持的所有generator
cmake .. -G "Visual Studio 16 2019" -A ARM64


#在命令行中设置环境变量
cmake . -DCMAKE_BUILD_TYPE=Debug
cmake . -UMyPackage_DIR		#unset variable
```



### Build a Project

```shell
#cmake --build <dir>             [<options>] [-- <build-tool-options>]
#构建一个已经产生的project binary tree，该命令抽象化了本地构建工具的命令行接口
cmake --build . --target all


#CMake提供的一些built-in targets：all help clean install
#查看支持的target:  cmake --build . --target help
#构建特定的target
cmake --build . --target myexe
```



### Install a project

```shell
#安装一个已经产生的project binary tree
#--install <dir>	Project binary directory to install
cmake --build. --target install <dir> [<options>]

```



























