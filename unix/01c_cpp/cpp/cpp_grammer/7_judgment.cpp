#include<iostream>
using namespace std;

/*
*选择分支只会执行其中的一个分支
	if(boolean_expression)
	{
		//boolean_expression为真时执行的语句
	}

	if(boolean_expression)
	{
		//boolean_expression为真时执行的语句
	}else
	{
		//boolean_expression为假时执行的语句
	}

	if(boolean_expression 1)
	{
		//boolean_expression 1为真时执行的语句
	}else if(boolean_expression 2){
		//boolean_expression 2为真时执行的语句
	}else if(boolean_expression 3){
		//boolean_expression 3为真时执行的语句
	}else{
		//都不成立时执行的语句
	}
	
*/


/*
switch(expression)
{
	case constant-expression:
		statements;
		break;
	case constant-expression:
		statements;
		break;
	default:
		statements;
}

*switch
	*用于测试一个变量等于多个值的情况，每个值称为一个case
	*expression必须是一个整型或者枚举类型
	*case的constant-expression必须与switch中的变量具有相同的数据类型，且必须是一个常量或者字面量
	*当遇到break时，switch终止，控制流跳转到switch语句后的下一行。

*/

/*
	exp1?exp2:exp3;  ? :表达式的值取决于exp1表达式值的真假。
	
*/
enum color{
	blue,
	green
};
	
int main(void)
{
	enum color z;
	
	int x = 1;
	char y = 'a';
	z = blue;

	const int i = 2;
	
	switch(z)	//()里必须是一个整型或者枚举类型的变量/常量
	{
		case i:	//constant-expression必须是一个常量或者字面量
			cout << "hhh\n";
			break;	//需要添加break语句，否则会一直执行到switch语句末尾
		case 1:
			cout << "sss\n";
			break;
	}


	int j;
	true ? j = 20 : j = 30;
	cout << j << endl;

	return 0;
}





















































