#include<iostream>
#include<stdio.h>
using namespace std;

/*
数据类型修饰符：signed,unsigned,long,short;
	char:signed,unsigned;
	int:signed,unsigned,long,short;
	double:long

*/

/*
类型限定符：const,volatile,restrict
	const:在程序执行期间不能被修改
	volatile:不要优化声明的变量
	restrict:修饰的指针是唯一访问它所指向的对象的方式

*/

int main(void)
{
	int  a;
	unsigned  b;
	short  c;
	unsigned short d;
	long  e;	
	unsigned long f;
		
	char g;
	unsigned char h;

	long double i;

/*
以char类型举例：
	char占用一个字节，取值范围：0-255；-128-127；
	0-127；
	128  -> 255，
	-128 -> -1；
	最终内存中是以补码形式来存储的
*/

//这三种效果是相同的，在内存中都是0x80
	g = 0x80; //内存中的补码形式
	printf("g == %hhx\n",g);
	g = 128;  //在0-255范围内，以无符号数解析
	printf("g == %hhx\n",g);
	g = -128; //在-128-127范围内，以有符号数解析
	printf("g == %hhx\n",g);
	
//这三种效果是相同的,在内存中都是0xff
	g = 0xff;  //内存中的补码形式
	printf("g == %hhx\n",g);
	g = 255;  //在0-255范围内，以无符号数解析
	printf("g == %hhx\n",g);
	g = -1;   //在-128-127范围内，以有符号数解析
	printf("g == %hhx\n",g);


	
	return 0;
}





















































