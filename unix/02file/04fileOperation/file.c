#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>


/**
 * perror:打印出errno对应的错误信息
 * 		void perror(const char *s);	
 * 		格式：s指向的内容：错误信息
 */
void perror_test(void)
{
	int fd = open("a.txt", O_RDONLY | O_NONBLOCK, 0006);
	if(fd < 0){
		printf("%s\n", strerror(errno));
		perror("open error");
		exit(-1);
	}	

	while(true){
		char buffer[100] = {};
		int nRead = read(fd, buffer, 99);
		printf("errno: %d\n", errno);
		perror("read");
		sleep(2);
		printf("read content: %s", buffer);
	}
}


/**
access: 测试调用程序的用户对指定文件是否具有特定权限。(是用户对文件的权限)
	int access(const char *pathname, int mode);
		pathname：文件名
		mode： R_OK, W_OK, X_OK.  F_OK （读/写/执行/存在）
		返回值：成功0(拥有指定权限)；失败-1(没有指定权限或者遇到其它错误)；
 */
void access_test(void)
{
	//R_OK, W_OK, X_OK, F_OK 
	int ret = access("a.txt", X_OK);
	if(ret != 0)	//拥有指定权限，ret返回0；没有指定权限或者遇到其它错误返回-1；
	{
		printf("do not have responding permission ...\n");
	}
	perror("access");
	return;
}


/**
stat: 获取文件状态
	int stat(const char *pathname, struct stat *buf);
	int lstat(const char *pathname, struct stat *buf);	不穿透符号链接文件，获取的是符号链接文件自身的属性而不是指向的文件的属性。
	int fstat(int fd, struct stat *buf);
	通过文件路径或者文件描述符(文件被open打开)指定一个文件，然后将文件的属性传递到buf指向的结构体。
 */
void stat_test(void)
{
	struct stat st;
	if(stat("a.txt", &st) != 0){
		perror("stat");
		exit(1);
	}

// 文件类型
	if(S_ISREG(st.st_mode)){
		printf("file type: regular file");
	}else if(S_ISDIR(st.st_mode)){
		printf("file type: directory file");
	}else if(S_ISLNK(st.st_mode)){
		printf("file type: link file");
	}else if(S_ISCHR(st.st_mode)){
		printf("file type: char file");	
	}else if(S_ISBLK(st.st_mode)){
		printf("file type: block file");
	}else if(S_ISSOCK(st.st_mode)){
		printf("file type: socket file");
	}else if(S_ISFIFO(st.st_mode)){
		printf("file type: fifo file");
	}

// 用户的rwx权限
	if(st.st_mode & S_IRUSR == S_IRUSR)
		printf("r");
	if(st.st_mode & S_IWUSR == S_IWUSR)
		printf("w");
	if(st.st_mode & S_IXUSR == S_IXUSR)
		printf("x");
	printf("\n");

// 时间属性
	st.st_atime;
	st.st_mtime;
	st.st_ctime;

// 文件大小
	printf("file size == %ld\n",(long int)st.st_size);

	return;
}


/**
chmod：修改文件权限
	int chmod(const char *filename, int mode)
		filename：文件名
		mode：文件权限，8进制数
 */
void chmode_test(){
	if(chmod("a.txt", 00700) != 0){
		perror("chmod");
		exit(-1);
	}
	struct stat filestat;
	if(stat("a.txt", &filestat) != 0){
		perror("stat");
		exit(-1);
	}
	chmod("a.txt", filestat.st_mode & ~__S_IFMT | S_IRWXO);
}


/**
 * 设置进程屏蔽字测试
 * 		mode_t umask(mode_t mask)
 */
void umask_test(){
    creat("a.txt", S_IRWXU | S_IRWXG | S_IRWXO);
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);   //设置进程屏蔽字
   	creat("b.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}


/**
chown：修改文件所有者
	int chown(const char *path, uid_t owner, gid_t group)
		Path：文件路径
		owner：整型值，用户ID
			/etc/passwd
		group：组ID
			/etc/group
 */
void chown_test()
{
	if(chown("a.txt", 1000, 1000) != 0){
		perror("chown");
		exit(-1);
	}
}


/**
文件重命名
	int rename(const char *oldpath, const char *newpath)
 */
void rename_test(){
	if(rename("a.txt", "b.txt") != 0){
		perror("rename");
		exit(-1);
	}
}


/**
truncate：截断文件		
	int truncate(const char *path, off_t length)
		path：文件名
		length：文件的最终大小
			1.比原来小，删掉后边的部分
			2.比原来大，向后拓展
 */
void truncate_test(){
	if(truncate("a.txt", 10) != 0){
		perror("truncate");
		exit(-1);
	}
}


/**
创建目录
	int mkdir(const char *pathname, mode_t mode)
	参数:
		pathname：创建的目录名
		mode：目录权限，8进制数，实际权限：mode & ~umask &0777

 */
void mkdir_test(){
	if(mkdir("dira", 0777) != 0){
		perror("mkdir");
		exit(-1);
	}
}


/*
struct dirent 
{
	ino_t          d_ino;       	//inode number 
	off_t          d_off;       	//not an offset; see NOTES
	unsigned short d_reclen;    	//length of this record
	unsigned char  d_type;      	//type of file; not supported by all filesystem types                              	
	char           d_name[256]; 	//filename
};

打开一个目录
	 DIR *opendir(const char *name)
		参数：目录名
		返回值：成功：返回指向目录的指针； 失败：返回NULL。
		
读取一个目录
	struct dirent* readdir(DIR *dirp)
		参数：opendir的返回值
		返回值：成功：目录项结构体指针； 失败：返回NULL并设置errno; 读完：返回NULL，不设置errno值。
			
关闭目录
	int close(DIR *dir)
		参数：opendir的返回值
*/
int readDir_test(char *filePath)
{
	int total = 0;
	DIR *dir = opendir(filePath);
	if(dir == NULL)
	{
		perror("opendir");
		exit(1);
	}

	struct dirent *direntryPtr = NULL;
	while((direntryPtr = readdir(dir)) != NULL)		//循环读目录里的entry,直到读完为止，读完则返回NULL
	{
		if(strcmp(".", direntryPtr->d_name) == 0 || strcmp("..", direntryPtr->d_name) == 0)
		{
			continue;		
		}

		if(direntryPtr->d_type == DT_REG)
		{
			total++;

		}else if(direntryPtr->d_type == DT_DIR)
		{
			char path[500] = {0};
			snprintf(path, 500, "%s/%s", filePath, direntryPtr->d_name);	//构造目录路径
			total += readDir_test(path);	//递归调用
		}

	}

	closedir(dir);

	return total;
}


/*
获取当前进程的工作目录
	char *getcwd(char *buf, size_t size)
		参数：
			buf：缓冲区，存储当前的工作目录 (返回的字符串以'\0'结尾，存储在buf中)
			size：缓冲区的大小
		返回值：
			成功：指向buf的指针
			失败：NULL (例如buf的长度不够，无法容纳当前的绝对路径名)

修改当前进程的路径
	int chdir(const char *path)
		参数：切换的路径
		0：成功；-1：失败
*/
void getcwd_test()
{
	char buf[256] = {};
	if(getcwd(buf, 256) != NULL){
		printf("before current working directory: %s\n", buf);
	}else{
		perror("getcwd");
		exit(-1);
	}

	if(chdir("/home/lhy/ownproject/stl") != 0){
		perror("chdir");
		exit(-1);
	}

	if(getcwd(buf, 256) != NULL){
		printf("after current working directory: %s\n", buf);
	}else{
		perror("getcwd");
		exit(-1);
	}

	return;
}


/**
复制文件描述符
	int dup(int oldfd);
		oldfd：要复制的文件描述符
		返回值：新的文件描述符(最小的且没被占用的文件描述符)	
		调用成功：有2个文件描述符指向同一个文件
		
	int dup2(int oldfd, int newfd);
		1.如果newfd已经指向了一个文件，则会自动执行close操作。
		2.如果newfd和oldfd指向同一个文件，则不执行操作。
		3.结果，newfd指向和oldfd指向的同一个文件
 */
int dup_test(void)
{
	int newfd = dup(STDIN_FILENO);
	printf("newfd: %d\n", newfd);

	int fd = open("a.txt", O_WRONLY);
	dup2(fd, STDOUT_FILENO);

	fprintf(stdout, "hello world nihao");
}


int main(int argc, char* argv[])
{
	chmode_test();

	return 0;
}


