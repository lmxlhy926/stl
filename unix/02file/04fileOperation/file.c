#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>


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



/*
stat:获取文件状态
	int stat(const char *pathname, struct stat *buf);
	int fstat(int fd, struct stat *buf);
	int lstat(const char *pathname, struct stat *buf);	不穿透符号链接文件，获取的是符号链接文件自身的属性而不是指向的文件的属性。
	通过文件路径或者文件描述符(文件被open打开)指定一个文件，然后将文件的属性传递到buf指向的结构体。
	stat既是一个函数也是一个命令。

int access(const char *pathname, int mode);
	pathname：文件名
	mode： R_OK, W_OK, and X_OK.  F_OK （读/写/执行/存在）
	返回值：成功0(拥有指定权限)；失败-1(没有指定权限或者遇到其它错误)；
	功能：测试调用程序的用户对指定文件是否具有特定权限。(是用户对文件的权限)


int chmod(const char *filename, int mode)
	filename：文件名
	mode：文件权限，8进制数


Int chown(const char *path, uid_t owner, gid_t group)
	Path：文件路径
	owner：整型值，用户ID
		/etc/passwd
	group：组ID
		/etc/group

		
Int truncate(const char *path, off_t length)
	paht：文件名
	length：文件的最终大小
		1.比原来小，删掉后边的部分
		2.比原来大，向后拓展
*/


/*
文件重命名
	int rename(const char *oldpath, const char *newpath)
	
修改当前进程的路径
	int chdir(const char *path)
		参数：切换的路径
		0：成功；-1：失败
		
获取当前进程的工作目录
	char *getcwd(char *buf, size_t size)
		参数：
			buf：缓冲区，存储当前的工作目录 (返回的字符串以'\0'结尾，存储在buf中)
			size：缓冲区的大小
		返回值：
			成功：指向buf的指针
			失败：NULL (例如buf的长度不够，无法容纳当前的绝对路径名)
创建目录
	int mkdir(const char *pathname, mode_t mode)
		参数:
			pathname：创建的目录名
			mode：目录权限，8进制数，实际权限：mode & ~umask &0777


*/


/*

打开一个目录
	 DIR *opendir(const char *name)
		参数：目录名
		返回值：成功：返回指向目录的指针； 失败：返回NULL。
		
读取一个目录
	struct dirent* readdir(DIR *dirp)
		参数：opendir的返回值
		返回值：成功：目录项结构体指针； 失败：返回NULL并设置errno;	  						 读完：返回NULL，不设置errno值。
		
struct dirent 
{
	ino_t          d_ino;       	//inode number 
	off_t          d_off;       	//not an offset; see NOTES
	unsigned short d_reclen;    	//length of this record
	unsigned char  d_type;      	//type of file; not supported by all filesystem types                              	
	char           d_name[256]; 	//filename
};
		
关闭目录
	int close(DIR *dir)
		参数：opendir的返回值

*/

/*
复制文件描述符
	int dup(int oldfd);
		oldfd：要复制的文件描述符
		返回值：新的文件描述符(最小的且没被占用的文件描述符)	
	调用成功：
		有2个文件描述符指向同一个文件
		
	NOTE:文件描述符是一个索引下标，指向描述文件的结构体，每个文件只有唯一一个结构体。

	int dup2(int oldfd, int newfd);
		1.如果newfd已经指向了一个文件，则会自动执行close操作。
		2.如果newfd和oldfd指向同一个文件，则不执行操作。
		3.结果，newfd指向和oldfd指向的同一个文件


*/





int main(int argc, char* argv[])
{

	perror_test();

	

	return 0;

}

#if 0




void func5_stat(void)
{
//int stat(const char *pathname, struct stat *buf);
	struct stat st;
	struct stat *stpointer = &st;

	int ret = stat("english.txt", stpointer);
	if(ret == -1)
	{
		perror("stat");
		exit(1);
	}
//文件大小
	printf("file size == %ld\n",(long int)st.st_size);
//文件类型
	if((st.st_mode & S_IFMT) == S_IFREG)
	{
		printf("this is a regular file\n");
	}
//用户的rwx权限
	if(st.st_mode & S_IRUSR)
		printf("r");
	if(st.st_mode & S_IWUSR)
		printf("w");
	if(st.st_mode & S_IXUSR)
		printf("x");
	printf("\n");


	return;
}



void func6_access(void)
{
//R_OK, W_OK, and X_OK, F_OK 
//拥有指定权限，ret返回0；没有指定权限或者遇到其它错误返回-1；
	int ret = access("english.txt", W_OK);
	if(ret == -1)
	{
		perror("access");
		exit(1);
	}

	printf("you can write this file\n");
	
	return;
}


void func7_chmodown(int argc, char *argv[])
{

	if(argc < 3)
		{
			printf("a.out filename mode\n");
			exit(1);
		}
	
		int mode = strtol(argv[2], NULL, 8);	//以8进制形式解析字符串
		printf("mode == %d\n", mode);
		int ret = chmod(argv[1], mode);
		if(ret == -1)
		{
			perror("chmod");
			exit(1);
		}
	
		ret = chown(argv[1], 1000, 1000);
		if(ret == -1)
		{
			perror("chown");
			exit(1);
		}


}



void func8_chdirgetcwd(int argc, char *argv[])
{

	if(argc < 2)
		{
			printf("a.out dir\n");
			exit(-1);
		}
		
		char buf[128];
		int ret;
		char *ret1 = NULL;
	//打印当前进程所在目录路径
		ret1 = getcwd(buf, sizeof(buf));
		if(ret1 == NULL)
		{
			perror("getcwd");
			exit(-1);
		}
		printf("current directory: %s\n", buf);
	//切换目录
		ret = chdir(argv[1]);
		if(ret == -1)
		{
			perror("chdir");
			exit(-1);
		}

		int fd = open("chdir.txt", O_CREAT | O_RDWR, 0777);
		if(fd == -1)
		{
			perror("open");
			exit(-1);
		}
		close(fd);
		
	//打印切换后的目录路径
		getcwd(buf, sizeof(buf));
		printf("current directory: %s\n", buf);

	return;
}



int func9_getfilenum(char *root)
{
	int total = 0;
	DIR *dir = NULL;
	
//打开一个目录
	dir = opendir(root);
	if(dir == NULL)
	{
		perror("opendir");
		exit(1);
	}

	struct dirent *ptr = NULL;
	while((ptr = readdir(dir)) != NULL)		//循环读目录里的entry,知道读完为止，读完则返回NULL
	{
		if(strcmp(".", ptr->d_name) == 0 || strcmp("..", ptr->d_name) == 0)
		{
			continue;		
		}

		if(ptr->d_type == DT_REG)
		{
			total++;
		}

		if(ptr->d_type == DT_DIR)
		{
			char path[1024] = {0};
			snprintf(path, 1024, "%s/%s", root, ptr->d_name);	//构造目录路径
			total += func9_getfilenum(path);	//递归调用
		}

	}

	closedir(dir);

	return total;

}


int func10_dup(void)
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}

	int fd1 = dup(fd);
	char *p = "hellohello";
	write(fd1, p, strlen(p));
	close(fd1);

	char buf[100];
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(buf));
	printf("%s\n", buf);

	close(fd);

}


int func11_dup2(void)
{

	int fd1 = open("temp1", O_RDWR | O_CREAT | O_APPEND, 0664);
	if(fd1 == -1)
	{
		perror("open");
		return -1;
	}

	int fd2 = open("temp2", O_RDWR | O_CREAT | O_APPEND, 0664);
	if(fd2 == -1)
	{
		perror("open");
		return -1;
	}

	dup2(fd1, fd2);	//fd2原先指向temp2，执行dup2操作后，先close temp2然后再指向fd1指向的文件。
	char *p = "helloworld";
	write(fd2, p, strlen(p));
	close(fd2);


	return 0;

}

#endif

