#include<iostream>
using namespace std;

/*
变量的属性：变量的值，变量的存储空间，变量的地址。由实际情况决定具体使用哪个属性。

变量地址：每个变量都要占用内存中的一段连续存储空间，存储空间的首地址就是该变量的地址。
指针变量：指针变量中存储的值不是普通的数值而是一个变量的地址。
&：返回变量的地址
*：返回地址指向的变量中存储的值。

*/



void get(int *s);

int main(void)
{

//指针的定义
	cout << "指针定义" <<endl;
	int *p1 = NULL;
	int var1 = 3;
	p1 = &var1; //将一个变量的地址赋值给指针变量，从而指针变量指向了这个变量
	cout << *p1 << endl;  //访问地址指向的变量
	cout << "----------------------------" <<endl;


/*
NULL:NULL的值为0，类型为(void *).故NULL是一个地址常量值，指向内存中地址为0的内存空间。
意义：
	在操作系统中，地址为0的内存空间是操作系统所保留的，程序是无法访问的。
	指针变量的值为NULL意味着该指针指向一个不可访问的内存位置。
	NULL在指针初始化的时候使用，防止野指针的出现。
空指针判断：
	int *ptr;
	if(ptr)  	//指针不为NULL时执行
	if(!ptr)    //指针为NULL时执行
	
*/
	cout << "NULL" <<endl;
	int *p2 = NULL;
	cout << "p2 == " << p2 << endl;  //指针变量中存储的值为0
	cout << "----------------------------" <<endl;


/*
指针的算术运算：++，--，+，-， 意义：指向上一个/下一个存储位置
指针的比较：> < >= <=  普通数值的比较

*/
	cout << "指针运算" <<endl;
	int var2[3] = {1,2,3};
	int *p3 = NULL;
	int *p4 = NULL;
	p3 = var2;  //var2是指向数组首字节位置的常量地址
	p4 = var2;
	
	for(int i = 0; i < 3; i++)
	{
		cout << "the address of var[" << i <<"] == " << p3 << endl;
		cout << "var[" <<i <<"] == " << *p3 << endl;
		p3++; //指向下一个位置
	}
	cout << "----------" <<endl;
	for(int i = 0; i < 3; i++)	
	{
		cout << "the address of var[" << i <<"] == " << var2+i << endl;
		cout << "var[" <<i <<"] == " << *(var2+i) << endl; //指向下一个位置
	}
	cout << "----------" <<endl;	
	for(int i = 0; p4 <= &var2[2]; p4++)
	{
		cout << "the address of var[" << i <<"] == " << p4 << endl;
		cout << "var[" <<i <<"] == " << *p4 << endl;
		i++;
	}
	cout << "----------" <<endl;	
	cout << "----------------------------" <<endl;

/*
指针vs数组：
	数组的索引访问本质上是通过指针实现的
	在函数的形式参数中创建一个数组变量本质上是创建一个指针变量
	*指针变量要么指向NULL要么指向一个具体的创建好的变量。
*/
	cout << "指针vs数组" <<endl;
	int p5[3] = {0,1,2};
	get(p5);
	cout << "----------------------------" <<endl;


/*
指针数组：本质上是一个数组，数组的元素是指针变量。

*/
	cout << "指针数组" <<endl;
	int var3[3] = {1,2,3};
	int *p6[3] = {NULL,NULL,NULL};
	p6[0] = var3;  //给指针变量赋值
	p6[1] = var3+1;
	p6[2] = var3+2;
	cout << **(p6+0) << endl;  //访问指针变量指向的变量
	cout << **(p6+1) << endl;
	cout << **(p6+2) << endl;
	cout << "----------" <<endl;
	cout << *p6[0] << endl;  //访问指针变量指向的变量
	cout << *p6[1] << endl;
	cout << *p6[2] << endl;
	cout << "----------" <<endl;
	
	const char *names[3]= {"this ","is ","good"};
	const char *names1 = "we are good";
	cout << names1 << endl;  //输出的不是指针变量的值，而是指针所指向的字符串。
	cout << &(*names1) << endl; //输出的不是地址，而是地址所指向的字符串
	cout <<*(names+0); 
	cout <<*(names+1);
	cout <<*(names+2) << endl;
	cout << "----------------------------" <<endl;


/*
二重指针：指针变量指向的变量还是指针类型的
		  二重指针是一种多级间接寻址的形式
pointer -> pointer -> variable
address    address    value
*/
	cout << "二重指针" <<endl;
	int var4 = 5;
	int *ptr = &var4;
	int **ptr1 = &ptr;
	cout << var4 << endl;  //变量本身的值
	cout << *ptr << endl;  //指针指向变量的值
	cout << **ptr1 << endl;//指针指向的变量所指向的变量值。
	cout << "----------------------------" <<endl;






	
	return 0;
}

void get(int s[])  //等同于void get(int *s) 实际上创建的是一个指针变量。
{
	cout << s << endl;
	cout << *s << endl;
	s++;
	cout << s << endl;
	cout << *s << endl;
	s++;
	cout << s << endl;
	cout << *s << endl;
	
	return;
}












































