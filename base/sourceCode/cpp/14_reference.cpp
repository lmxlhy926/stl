#include<iostream>
using namespace std;

/*
引用：①变量的别名
	  ②指针的另一种形式
主要用于函数的形式参数列表中，可以直接传递变量名。在函数中对变量的操作会
改变函数外变量的值。是指针操作变量的一种简化写法。
*/



void func1(int& a, int& b);
int& func3();
int* func4();

int main(void)
{

/*
引用的本质：
	c++编译器在编译过程中使用常量指针作为引用的内部实现，
	因此引用占用的空间大小和指针相同。
	Type& ref = val;  
	Type* const ref = &val;
*/

/*
变量的引用：
	使用引用操作变量的值相当于使用指向该变量的指针操作该变量的值
	&不是求地址运算，只是起到标识的作用
	类型标识符是指目标变量的类型
	必须在声明引用变量时进行初始化
	引用初始化之后不能改变
	不能有NULL引用。必须确保引用是和一块合法的存储单元关联。
*/
	
	cout << "变量的引用" << endl;
	int a = 10;
	int& b = a;  // 编译器自动转换为 int* const b = &a;
	b = 20;      // 编译器自动转换为 *b = 20;
	cout << "a == " << a << endl;
	cout << "b == " << b << endl;
	cout << "----------------------------" << endl;

	

//数组的引用：
	cout << "数组的引用" << endl;
	typedef int array[10];
	int arr[10];
	array& arr1 = arr;    	 //数组引用的写法1
	int (&arr2)[10] = arr;   //数组引用的写法2
	int (*const arr3)[10] = &arr;  //引用的本质形式
		
	for(int i = 0; i < 10; i++)
		arr1[i] = i;
	arr2[6] = 600;	
	cout << "arr[5] == " << arr[5] << endl;
	cout << "arr[6] == " << arr[6] << endl;
	(*arr3)[6] = 700;
	cout << "arr[6] == " << arr[6] << endl;
	cout << "----------------------------" << endl;


/*
函数的引用：
	当引用被用作函数形式参数时，在函数内对引用的修改将对函数外的参数产生改变。
	本质上和使用指针是一样的，但其语法更简单。(编译器做了额外的翻译工作)
	如果从函数中返回了一个引用，必须像从函数中返回一个指针一样对待。
	当函数返回一个引用时，引用关联的内存一定要存在，因此不能返回局部变量的引用。
*/
	cout << "函数的引用" << endl;
	int var1 = 20;
	int var2 = 30;
	func1(var1,var2);
	cout << "var1 == " << var1 << endl;
	cout << "var2 == " << var2 << endl;

	cout << "func3() == " << func3() << endl;
	func3() = 40;	 //此时函数名相当于一个引用，此例中即是a的别名。
	cout << "func3() == " << func3() << endl;

	*func4() = 100;  
	cout << "static a1 == " << *func4() << endl;
	cout << "----------------------------" << endl;


/*
指针变量的引用：
*/
	cout << "指针变量的引用" << endl;
	int x = 100;
	int *a4 = &x;
	int*& a5 = a4;
	cout << "x == " << x << endl;
	*a5 = 200;
	cout << "x == " << x << endl;
	

	
	return 0;
}



void func1(int& a, int& b) //本质上的实现方式是func2
{
	int temp = a;
	a = b;
	b = temp;
	return;
}

void func2(int *a, int *b)  
{
	int temp = *a;
	*a = *b;
	*b = temp;
	return;
}

int& func3()//返回a的引用
{
	static int a = 30;
	return a; //相当于执行了 int& x =a; return x;	
}

int* func4()	
{
	static int a1 = 31;
	return &a1;
}

































