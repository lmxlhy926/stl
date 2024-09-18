#include<iostream>
using namespace std;

/*
*三种循环类型
*condition：conditon是一个布尔表达式：把任何非零和非空的值假定为true，把零或null假定为false.

	while(condition)
	{
		statements;
	}

	do
	{
		statements;
	}while(condition)


	for(init; condition; increment)
	{
		statements;
	}
*/


/*
循环控制语句
*break:跳出当前循环
	1.break出现在一个循环内时，循环会立即终止，程序流执行紧接着循环的下一条语句.
	2.它可用于终止switch语句中的一个case.
	
*continue:跳到当前循环末尾
	跳到循环的末尾，开始下一次循环

*goto:跳转到同一函数内的被标记的语句
	很好的作用是退出深嵌套循环。
*/

int main(void)
{
	int label = 3;
	cout << "1";
	goto label;
	cout << "2";
	cout << "3";

label:
	cout << "label\n";
	cout << label <<endl;
	
	return 0;
}









