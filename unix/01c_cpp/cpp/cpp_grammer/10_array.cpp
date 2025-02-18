#include<iostream>
using namespace std;


#include<stdio.h>

//数组的声明
int a[10];

int b[2][3];
int c[2][3];
int d[2][3];

//函数中的形式参数含有数组时，以下三种方式的效果是相同的，
//**都是将数组名当做指针对待,因此实参数组中的值可以被改变。
int getAverage(int a[],int size);
int getAverage(int *a,int size);
int getAverage(int a[4],int size);

//返回值为指针的函数
int * getPointer(void);


/*
数组：内存中的一块连续区域，存储一组相同类型的变量的值。
	数组的声明：type arrayName[arraySize];
	数组的初始化：使用{}初始化数组。
	数组的赋值：使用 下标索引对单个元素进行赋值

	数组指针：编译器将数组名看做一个常量指针。

	函数返回指针：C++不支持在函数外返回局部变量的地址，除非定义局部变量为static变量。
*/



int main(void)
{

	int a[10] = {1,2,3};	//数组的初始化
	int b[2][3] = {{1,2,3},{4,5,6}};
	int c[2][3] = {{11,12,13},14,15,16};
	int d[2][3] = {111,112,113,114,115,116};


	for(int i = 0; i < 2; i++)	
		for(int j = 0; j < 3; j++)		
			printf("c[%d][%d] == %d\n",i,j,c[i][j]);					
	
	printf("-----------------\n");
	

	int e[2] = {0,1};
	int *p = NULL;
	p = e;  //编译器将数组名e看做一个常量指针，将此常量值赋值给指针变量p。
	for(int i = 0; i < 2; i++)
	{
		printf("e[%d] == %d\n",i,*(p+i));
		printf("e[%d] == %d\n",i,p[i]);
	}
	printf("-----------------\n");


	int f[3] = {1,2,3};
	//f[10] = 4;  //编译通过，因为C/C++中不执行边界检查。但是运行时会报错。
	printf("average == %d\n",getAverage(f,3));
	printf("f[0] == %d\n",f[0]); 	//f中元素的值被改变
	

	int *pointer = NULL;
	pointer = getPointer();	
	printf("the value is %d\n",*pointer);
	



	return 0;
}


int getAverage(int a[],int size)
{
	int sum,average;
	
	for(int i = 0; i < size; i++)	
		sum += a[i];	
	
	a[0] = 100;  //由于接收的是指针，所以改变了实参数组中的值。
	average = sum/size;

	return average;
}


int * getPointer(void)
{
	static int asss = 300;
	int *p = NULL;
	p = &asss;
	
	return p;
}











































