
int add(int a, int b){
    return a + b;
}

int sub(int a, int b){
    return a - b;
}

// 可以将源代码编译为.o格式的可重定位目标文件，然后打包到.a格式的静态库中
// 被选中的.o格式的可重定位文件会被重定位到可执行文件中
// 随着可执行文件的加载而加载到内存中