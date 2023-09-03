#include<iostream>
#include<stdio.h>

using namespace std;

/*
字符串：实质上是以NULL(\0)字符结尾的一维字符数组。
字符串中的字符：以数组下标的方式来引用。

*/




int main(void)
{

	char a[] = "this is good";  //字符数组的初始化。
	//char *a1 = "this is very good";	//c中可以，C++中不行。
	
	cout << a << endl;
	printf("sizeof(a) == %ld\n",sizeof(a));
	

	return 0;
}






